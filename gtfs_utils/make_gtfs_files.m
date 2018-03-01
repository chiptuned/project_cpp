function [] = make_gtfs_files(db, fn1, fn2);

%% Compute all connections from trips, and there stops
t_start_t_connections = tic;
route_to_trips = db.trips{:,{'route_id', 'trip_id'}};
stop_times = [uint64(db.stop_times{:,1}), seconds(db.stop_times{:,2:3}), ...
  db.stop_times{:,4}];
stops_id = db.stops{:,1};
stops_name = table2cell(db.stops(:,{'stop_name', 'stop_desc'}));
routes_name = table2cell(db.routes(:,{'route_short_name', 'route_long_name'}));

bad_routes = [];
cpt = 0;
curr_c = 0;
n_r = size(db.routes, 1);
% let's say that we will have maximum 10 times more connections than
% stops, for preallocation purposes
all_connections = zeros(10*size(db.stops,1), 3, 'uint32');
all_stops_from_connections = cell(10*size(db.stops,1), 5);

if exist('wb', 'var')
  delete(wb)
end
wb = waitbar(0,'Compute connections for each route...');

%% FIXME just have to parse trips, then attach route to their stop
for route = db.routes{:,'route_id'}'
  cpt = cpt + 1;

  % we take the first trip to compute their connections
  t1 = tic;
  trip = route_to_trips(find(route==route_to_trips(:,1),1),2);
  a = toc(t1);

  % % longer task :
  % % we can take a trip that start around 10 am, else one with the most connections
  % t2 = tic;
  % trips = uint64(route_to_trips(find(route==route_to_trips(:,1)),2));
  % numel_stop_times = zeros(numel(trips), 1);
  % for ind = 1:numel(trips)
  %   idx_stop_times = find(stop_times(:,1)==trips(ind));
  %   numel_stop_times(ind) = numel(idx_stop_times);
  %   if abs(double(stop_times(idx_stop_times(1),2))/3600-10) < 1
  %     trip = trips(ind);
  %     break;
  %   elseif ind == numel(trips)
  %     [~, idx_max] = max(numel_stop_times);
  %     trip = trips(idx_max);
  %   end
  % end
  % a = toc(t2);

  % a route can be empty (wip, depreceated, etc)
  if isempty(trip)
    bad_routes = [bad_routes, route];
  else
    % find the stops and their duration, we only take the start time and
    % stop
    % FIXME too long. We should scrape the stop_times first, then
    % find the route? maybe need to sort and it would be long?
    idx_stop_times = find(stop_times(:,1)==trip);

    % relevant stops are in chronological order (in stop times)
    revelevant_stops = stop_times(idx_stop_times,[2,4]);
    nb_c = size(revelevant_stops,1)-1;

    % connections
    route_connect = [revelevant_stops(1:end-1,2), revelevant_stops(2:end,2), ...
      revelevant_stops(2:end,1) - revelevant_stops(1:end-1,1)];

    % stops attached to connections
    st = revelevant_stops(1:end,2);
    [~, st_id] = ismember(st, stops_id);
    st_from_connect = [num2cell(stops_id(st_id)), ...
      repmat(routes_name(cpt,1), nb_c+1, 1), ...
      stops_name(st_id,:), ...
      repmat(routes_name(cpt,2), nb_c+1, 1)];

    % NOTE : it is a faster way to store connection and write all
    % of them in one dlmwrite. For RATP set, it is 13 seconds faster, but
    % you need to preallocate memory (but it takes >>13 seconds to compute
    % the exact amount of connections)
    all_connections(curr_c+(1:nb_c),:) = uint32(route_connect);
    all_stops_from_connections(curr_c+cpt-numel(bad_routes)+(0:nb_c),:) = st_from_connect;
    curr_c = curr_c + nb_c;
  end
  if mod(cpt, ceil(n_r/200))
    waitbar(cpt/n_r, wb);
  end
end
delete(wb)
all_connections = all_connections(1:curr_c,:);
% seconds added for each duration of connections to avoid some zeros
offset_connect = 30;
all_connections(:,3) = all_connections(:,3) + offset_connect;

all_stops_from_connections = all_stops_from_connections(1:curr_c+cpt,:);
[~, idx_unique] = unique(cell2mat(all_stops_from_connections(:,1)));
all_stops = all_stops_from_connections(idx_unique,:);
t_generation_connections = toc(t_start_t_connections)




%% Compute same stops as an instant connection
t_start_t_same_stops = tic;
stops_gps_str = join(arrayfun(@(a) num2str(a, '%02.16f\n'), ...
  table2array(db.stops(:,[5,6])), 'UniformOutput', 0));
stops_id = table2array(db.stops(:,1));
[~, ~, idx_unique_stop] = unique(stops_gps_str);
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
t_generation_same_stops = toc(t_start_t_same_stops)

%% Export files
delete(fn1)
fid = fopen(fn1,'w');
fprintf(fid,'%s, %s\n', '# uint_s_id, string_short_line, string_name_station', ...
'string_adress_station, string_desc_line');

% remove commas
all_stops(:,2:end) = strrep(all_stops(:,2:end),',', '');

formatSpec = '%d,%s,%s,%s,%s\n';
for ind = 1:size(all_stops,1)
    fprintf(fid,formatSpec,all_stops{ind,:});
end
fclose(fid);

delete(fn2)
fid = fopen(fn2,'w');
fprintf(fid,'# from_stop_id, to_stop_id, min_transfer_time\n');
fclose(fid);
dlmwrite(fn2, all_connections, '-append', 'precision','%d')
dlmwrite(fn2, all_same_stops, '-append', 'precision','%d')
dlmwrite(fn2, uint32(table2array(db.transfers(:,[1, 2, 4]))), ...
  '-append', 'precision','%d')
