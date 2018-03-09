function [] = make_gtfs_files(db, fn1, fn2, cmode);

%% All connections from trips
t_start_t_connections = tic;

% there is at least two adjacent lines with the same trip, let's
% do connections that way
nb_connect = size(db.stop_times,1)-1;
connect = zeros(nb_connect, 6, 'uint64');
connect(:,1:2) = [db.stop_times{1:end-1,4}, db.stop_times{2:end,4}];
connect(:,3) = db.stop_times{2:end,3} - db.stop_times{1:end-1,3};
connect(:,4:5) = [db.stop_times{1:end-1,1}, db.stop_times{2:end,1}];
connect(:,6) = db.stop_times{1:end-1,3};
% remove connections that don't have the same trip
connect(connect(:,4)~=connect(:,5),:) = [];

if cmode > 1
  all_connections = uint32(connect(:, 1:3));
  connection_dates = double(connect(:, 6));
else
  % unique connections and their different trips
  [~, iB, iA] = unique(connect(:,1:2), 'rows');
  idx_times_to_merge = accumarray(iA, (1:numel(iA)).', [], @(r){sort(r)});

  % connection time by averaging connection time of all differents trips
  all_connections = uint32(connect(iB,1:3));
  for ind = 1:numel(idx_times_to_merge)
    all_connections(ind,3) = round(mean(connect(idx_times_to_merge{ind},3)));
  end
end

% seconds added for each duration of connections to avoid some zeros
offset_connect = 30;
all_connections(:,3) = all_connections(:,3) + offset_connect;
t_generation_connections = toc(t_start_t_connections)


%% All stops from connections
t_start_t_stops = tic;
% we suppose that each stop constitutes the same route for each trip
[stops_idx, iB] = unique(uint32([connect(:,1), connect(:,2)]));
% all_stops is constituted of :
% stop_id, route_name_short, stop_name_short, stop_name_full, route_name_full
all_stops = cell(numel(stops_idx), 5);
all_stops(:,1) = num2cell(stops_idx);
iB = mod(iB, size(connect,1));
iB(iB == 0) = size(connect,1);

% from connect we can recuperate the trip_if for each stop
% so we need to find the route (db.trips), then its name (db.routes), and
% the name of the stop (db.stops)
trip_id = connect(iB,4);
[~, loc] = ismember(trip_id,db.trips{:,3});
route_id = db.trips{loc,1};

[~, loc] = ismember(route_id,db.routes{:,1});
all_stops(:,[2,5]) = table2cell(db.routes(loc,3:4));

[~, loc] = ismember(stops_idx,db.stops{:,1});
all_stops(:,3:4) = table2cell(db.stops(loc,3:4));
t_generation_stops = toc(t_start_t_stops)

if cmode == 0
  all_stops = all_stops(:,[3,1,2]);
else
  % remove all_stop commas
  all_stops(:,2:end) = strrep(all_stops(:,2:end),',', '');

  all_stops = all_stops(:,[3,1,2,4,5]);
end

%% Same stops as instant connections FIXME(vincent): not fully tested
t_start_t_same_stops = tic;
stops_id = table2array(db.stops(:,1));
[~, ~, idx_unique_stop] = unique(db.stops(:,[5,6]), 'rows');
[sorted_unique, idx_sort] = sort(idx_unique_stop);
% let's say that we will have maximum 100 times more instant connections than
% stops, for preallocation purposes
% NOTE: this can grow geometrically if a lot of the same stop occurs, could
% be not enough. If n same stop, n*n-1 instant connections have to be done.
all_same_stops = zeros(100*size(db.stops,1), 3, 'uint32');

cpt = 0;
for ind = 1:(numel(idx_sort) - 1)
  curr_id = stops_id(idx_sort(ind));
  next_id = stops_id(idx_sort(ind+1));
  is_same = stops_id(idx_sort(sorted_unique(ind))) == ...
    stops_id(idx_sort(sorted_unique(ind+1)));
  reps = [curr_id];
  while is_same
    if ind == (numel(idx_sort) - 1)
      break;
    else
      ind = ind + 1;
    end
    reps = [reps, next_id];
    curr_id = stops_id(idx_sort(ind));
    next_id = stops_id(idx_sort(ind+1));
    is_same = stops_id(idx_sort(sorted_unique(ind))) == ...
      stops_id(idx_sort(sorted_unique(ind+1)));
  end
  if numel(reps) > 1
    conn = combnk(reps,2);
    nbconn = size(conn,1)*2;
    all_same_stops(cpt+(1:nbconn),1:2) = [conn; fliplr(conn)];
    cpt = cpt + nbconn;
  end
end
all_same_stops = all_same_stops(1:cpt,:);
all_same_stops = unique(all_same_stops, 'rows');
if cmode > 1
  % this does nothing, because nan is a floating point concept :
  connection_date_same_stops = nan(size(all_same_stops(:,1)));
end
t_generation_same_stops = toc(t_start_t_same_stops)


%% Transfers
t_start_t_transfers = tic;
all_transfers = [uint32(table2array(db.transfers(:,[1, 2, 4])));
    uint32(table2array(db.transfers(:,[2, 1, 4])))];
all_transfers = unique(all_transfers, 'rows');
if cmode > 1
  connection_date_transfers = nan(2*size(all_transfers(:,1),1),1);
end
t_generation_transfers = toc(t_start_t_transfers)

%% Export files
t_start_t_export_data = tic;
table_stops = cell2table(all_stops);
table_connections = array2table([all_connections; all_same_stops; all_transfers]);

if cmode > 1
  table_dates = array2table([connection_dates; ...
    connection_date_same_stops; ...
    connection_date_transfers]);

  if cmode > 2
    keeping_lines = [{'1'}; {'2'}; {'3'}; {'4'}; {'5'}; {'6'}; {'7'}; {'8'}; ...
    {'9'}; {'10'}; {'11'}; {'12'}; {'13'}; {'14'}; {'3B'}; {'7B'}]

    removing_stops = ~ismember(all_stops(:,3), keeping_lines);
    keeping_ids = cell2mat(all_stops(~removing_stops,2));
    table_stops(removing_stops,:) = [];

    full_connect = table2array(table_connections);
    removing_c = ~ismember(full_connect(:,1), keeping_ids) | ...
      ~ismember(full_connect(:,2), keeping_ids);
    table_connections(removing_c,:) = [];
    table_dates(removing_c,:) = [];
  end

  if cmode > 3

    if cmode == 5
      % static discard of unwanted connections :
      % Simplon 1668 2478 - Chateau Rouge 2033 2152
      % Clignancourt 1742 2420 - Marcadet 1665 2535
      % Dupleix 1926 2251 - Cambronne 2016 2135
      % there were identified and TODO() checked it doesn't break the graph
      full_connect(removing_c,:) = [];
      connections_to_remove = ...
          ismember(full_connect(:,1:2), [1668 2033], 'rows') | ...
          ismember(full_connect(:,1:2), [2478 2152], 'rows') | ...
          ismember(full_connect(:,1:2), [1742 1665], 'rows') | ...
          ismember(full_connect(:,1:2), [2420 2535], 'rows') | ...
          ismember(full_connect(:,1:2), [1926 2016], 'rows') | ...
          ismember(full_connect(:,1:2), [2251 2135], 'rows') | ...
          ismember(full_connect(:,1:2), [2033 1668], 'rows') | ...
          ismember(full_connect(:,1:2), [2152 2478], 'rows') | ...
          ismember(full_connect(:,1:2), [1665 1742], 'rows') | ...
          ismember(full_connect(:,1:2), [2535 2420], 'rows') | ...
          ismember(full_connect(:,1:2), [2016 1926], 'rows') | ...
          ismember(full_connect(:,1:2), [2135 2251], 'rows');
      full_connect(connections_to_remove,:) = [];
      table_connections(connections_to_remove,:) = [];
      table_dates(connections_to_remove,:) = [];
    end


    array_dates = table2array(table_dates);
    idx_valid_dates = (array_dates > 18*3600) & (array_dates < 18*3600+5*60) ...
      | isnan(array_dates);
    table_connections(~idx_valid_dates,:) = [];

    % unique connections and their different trips
    [~, iB, iA] = unique(table_connections(:,1:2), 'rows');
    idx_times_to_merge = accumarray(iA, (1:numel(iA)).', [], @(r){sort(r)});

    % connection time by averaging connection time of all differents trips
    new_t_connect = table_connections(iB,:);
    for ind = 1:numel(idx_times_to_merge)
      new_t_connect{ind,3} = round(mean(table_connections{idx_times_to_merge{ind},3}));
    end
    table_connections = new_t_connect;
    % At this point table_dates is irrelevant

    table_connections.Properties.VariableNames = {'uint32_from_stop_id', ...
      'uint32_to_stop_id', 'uint32_min_transfer_time'};
  else
    table_connections = [table_connections, table_dates];
    table_connections.Properties.VariableNames = {'uint32_from_stop_id', ...
      'uint32_to_stop_id', 'uint32_min_transfer_time', 'string_connection_date'};
  end

  table_dates.Properties.VariableNames = {'string_connection_date'};
  table_stops.Properties.VariableNames = {'string_name_station', ...
    'uint32_s_id', 'string_short_line', 'string_adress_station', ...
    'string_desc_line'};
elseif cmode == 1
  table_stops.Properties.VariableNames = {'string_name_station', ...
    'uint_s_id', 'string_short_line', 'string_adress_station', ...
    'string_desc_line'};
  table_connections.Properties.VariableNames = {'uint_from_stop_id', ...
    'uint_to_stop_id', 'uint_min_transfer_time'};
else
  table_stops.Properties.VariableNames = {'string_name_station', ...
    'uint_s_id', 'string_short_line'};
  table_connections.Properties.VariableNames = {'uint_from_stop_id', ...
    'uint_to_stop_id', 'uint_min_transfer_time'};
end
t_generation_export_data = toc(t_start_t_export_data)

t_start_t_csv_write = tic;
delete(fn1)
delete(fn2)
writetable(table_stops, fn1)
writetable(table_connections, fn2)
t_generation_csv_write = toc(t_start_t_csv_write)
