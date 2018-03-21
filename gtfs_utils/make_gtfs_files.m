function [] = make_gtfs_files(db, fn1, fn2, keep_gps, keeping_lines);
% maybe add a gps coord toogle?
verbose = 1;

msg1 = ['BEWARE, to work with the RATP database, you need at least ', ...
    '3,3Go of free memory (including db, which is the RATP ', ...
    'database, and should be ~1,2Go)'];
msg2 = ['This function is tested on MATLAB R2017a and optimized ', ...
    'for MATLAB R2018a.'];
warning(msg1);
warning(msg2);

% do something nicer with this, like put an argument keeping_lines:
% if it's a string, if it's metro, rer, bus, keep the good lines
% if its a cell array keep the lines in the variable
metro_lines = [{'1'}; {'2'}; {'3'}; {'4'}; {'5'}; {'6'}; {'7'}; {'8'}; ...
{'9'}; {'10'}; {'11'}; {'12'}; {'13'}; {'14'}; {'3B'}; {'7B'}];
if nargin < 5
  keeping_lines = 'metro';
end
if nargin < 4
  keep_gps = 0;
end
if nargin < 3
  fn1 = 'stations.csv';
  fn2 = 'connections.csv';
end
if nargin < 1
  error('arguments needed. type "help make_gtfs_files".')
end

if strcmp(keeping_lines,'metro')
  keeping_lines = metro_lines;
end

% seconds added for each duration of connections to avoid some zeros
offset_connect = 30;

if verbose
  keeping_lines'
  offset_connect
end


%% All connections from trips
t_start_t_connections = tic;

% there is at least two adjacent lines with the same trip, let's
% do connections that way
connect_trips = [db.stop_times{1:end-1,1}, db.stop_times{2:end,1}];
good_trips = find(connect_trips(:,1)==connect_trips(:,2));
connect_trips = connect_trips(good_trips,:);
nb_connect = numel(good_trips);
all_connections = [db.stop_times{good_trips,3}, db.stop_times{good_trips+1,3}];
times = [db.stop_times{good_trips,2}, db.stop_times{good_trips+1,2}];
db.stop_times = [];

% transform times into seconds is expansive, so we will do it after
% the last reduction of this matrix
t_generation_connections = toc(t_start_t_connections);
if verbose
  t_generation_connections
end


%% All stops from connections
t_start_t_stops = tic;

% we suppose that each stop constitutes the same route for each trip
[stops_idx, iB] = unique(all_connections);
% all_stops is constituted of :
% stop_id, route_name_short, stop_name_short, stop_name_full, route_name_full
all_stops = cell(numel(stops_idx), 5);
all_stops(:,1) = num2cell(stops_idx);
iB = mod(iB, nb_connect);
iB(iB == 0) = nb_connect;

% from connect we can recuperate the trip_id for each stop
% so we need to find the route (db.trips), then its name (db.routes), and
% the name of the stop (db.stops)
trip_id = connect_trips(iB,1);
[~, loc] = ismember(trip_id,db.trips{:,3});
route_id = db.trips{loc,1};

[~, loc] = ismember(route_id,db.routes{:,1});
all_stops(:,[2,5]) = table2cell(db.routes(loc,3:4));

[~, loc] = ismember(stops_idx,db.stops{:,1});
all_stops(:,3:4) = table2cell(db.stops(loc,3:4));

% remove all_stop commas
all_stops(:,2:end) = strrep(all_stops(:,2:end),',', '');
all_stops = all_stops(:,[3,1,2,4,5]);
if keep_gps
  all_stops(:,6:7) = table2cell(db.stops(loc,5:6));
end
t_generation_stops = toc(t_start_t_stops);
if verbose
  t_generation_stops
end


%% Same stops as instant connections NOTE(vincent): not fully tested
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
  reps = stops_id(idx_sort(ind));
  next_id = stops_id(idx_sort(ind+1));
  is_same = stops_id(idx_sort(sorted_unique(ind))) == ...
    stops_id(idx_sort(sorted_unique(ind+1)));
  while is_same
    if ind == (numel(idx_sort) - 1)
      break;
    else
      ind = ind + 1;
    end
    reps = [reps, next_id];
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
t_generation_same_stops = toc(t_start_t_same_stops);
if verbose
  t_generation_same_stops
end

%% Transfers
t_start_t_transfers = tic;
all_transfers = [uint32(table2array(db.transfers(:,[1, 2, 4])));
    uint32(table2array(db.transfers(:,[2, 1, 4])))];
all_transfers = unique(all_transfers, 'rows');
t_generation_transfers = toc(t_start_t_transfers);
if verbose
  t_generation_transfers
end


%% Preparing tables, and removing unwanted lines
t_start_remove_lines = tic;
if verbose
  fprintf('Removing unwanted lines, this takes time...\n')
end

% We want to export a table of connections,  but the times for each connection
% isn't computed yet. So we separately compute the good connections and add the
% correct times after.
full_connect = [all_connections; all_same_stops(:,1:2); all_transfers(:,1:2)];

times_stop_tran = [all_same_stops(:,3); all_transfers(:,3)];
n_t_connect = size(times,1);

% Removing stations and connections of unwanted lines
removing_stops = ~ismember(all_stops(:,3), keeping_lines);
keeping_ids = cell2mat(all_stops(~removing_stops,2));
all_stops(removing_stops,:) = [];

removing_c = ~ismember(full_connect(:,1), keeping_ids) | ...
  ~ismember(full_connect(:,2), keeping_ids);

full_connect(removing_c,:) = [];
times_stop_tran(removing_c((n_t_connect+1):end),:) = [];
times(removing_c(1:n_t_connect),:) = [];
n_t_connect = size(times,1);

% static discard of unwanted connections :
% Simplon 1668 2478 - Chateau Rouge 2033 2152
% Clignancourt 1742 2420 - Marcadet 1665 2535
% Dupleix 1926 2251 - Cambronne 2016 2135
% they were identified and checked it doesn't break the graph
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
times_stop_tran(connections_to_remove((n_t_connect+1):end),:) = [];
times(connections_to_remove(1:n_t_connect),:) = [];
t_remove_lines = toc(t_start_remove_lines);
if verbose
  t_remove_lines
end


%% Now we can compute the seconds between two times (in a string format)
t_start_duration_computation = tic;

if version('-date') > datetime(2017,10,31)
  % optimized method for MATLAB R2018a
  times = seconds(duration(times));
else
  if verbose
    fprintf('Entering non-optimized duration calculation, this can take a while...\n')
  end
  tmp = datetime(times,'InputFormat','HH:mm:ss');
  times = hour(tmp)*3600+minute(tmp)*60+second(tmp);
end

timediff = times(:,2) - times(:,1) + offset_connect;
times_stop_tran = [timediff;times_stop_tran];
t_duration_computation = toc(t_start_duration_computation);
if verbose
  t_duration_computation
end


%% Removing courses out of a specific timeframe, and averaging course duration
t_start_remove_timeset_and_averaging = tic;
idx_valid_dates = (times(:,1) > 18*3600) & (times(:,1) < 18*3600+5*60);
full_connect(~idx_valid_dates,:) = [];
times_stop_tran(~idx_valid_dates,:) = [];

% unique connections and their different trips
[~, iB, iA] = unique(full_connect, 'rows');
idx_times_to_merge = accumarray(iA, (1:numel(iA)).', [], @(r){sort(r)});

% connection time by averaging connection time of all differents trips
new_t_connect = [array2table(full_connect(iB,:)), ...
  array2table(zeros(size(full_connect(iB,1))), 'VariableNames', {'transfer_time'});];
for ind = 1:numel(idx_times_to_merge)
  new_t_connect{ind,3} = round(mean(times_stop_tran(idx_times_to_merge{ind})));
end
t_remove_timeset_and_averaging = toc(t_start_remove_timeset_and_averaging);
if verbose
  t_remove_timeset_and_averaging
end


%% Export files
t_start_t_export_data = tic;
table_connections = new_t_connect;
table_stops = cell2table(all_stops);

table_connections.Properties.VariableNames = {'uint32_from_stop_id', ...
  'uint32_to_stop_id', 'uint32_min_transfer_time'};

var_names_table_stops = {'string_name_station', ...
  'uint32_s_id', 'string_short_line', 'string_adress_station', ...
  'string_desc_line'};
if keep_gps
  var_names_table_stops = [var_names_table_stops, ...
    'double_stop_lat', 'double_stop_lon'];
end
table_stops.Properties.VariableNames = var_names_table_stops;

t_export_data = toc(t_start_t_export_data);
if verbose
  t_export_data
end


%% Write files
t_start_t_csv_write = tic;
delete(fn1)
delete(fn2)
writetable(table_stops, fn1)
writetable(table_connections, fn2)
t_generation_csv_write = toc(t_start_t_csv_write);
if verbose
  t_generation_csv_write
end
