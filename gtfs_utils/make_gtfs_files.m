function [] = make_gtfs_files(db, fn1, fn2);

%% All connections from trips
t_start_t_connections = tic;

% there is at least two adjacent lines with the same trip, let's
% do connections that way
nb_connect = size(db.stop_times,1)-1;
connect = zeros(nb_connect, 5, 'uint64');
connect(:,1:2) = [db.stop_times{1:end-1,4}, db.stop_times{2:end,4}];
connect(:,3) = db.stop_times{2:end,3} - db.stop_times{1:end-1,3};
connect(:,4:5) = [db.stop_times{1:end-1,1}, db.stop_times{2:end,1}];
% remove connections that don't have the same trip
connect(connect(:,4)~=connect(:,5),:) = [];

% unique connections and their different trips
[~, iB, iA] = unique(connect(:,1:2), 'rows');
idx_times_to_merge = accumarray(iA, (1:numel(iA)).', [], @(r){sort(r)});

% connection time by averaging connection time of all differents trips
all_connections = uint32(connect(iB,1:3));
for ind = 1:numel(idx_times_to_merge)
  all_connections(ind,3) = round(mean(connect(idx_times_to_merge{ind},3)));
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
t_generation_same_stops = toc(t_start_t_same_stops)


%% Export files

% remove all_stop commas
all_stops(:,2:end) = strrep(all_stops(:,2:end),',', '');

delete(fn1)
fid = fopen(fn1,'w');

% adapt all_stops to cpp code format
% fprintf(fid,'%s, %s\n', '# uint_s_id, string_short_line, string_name_station', ...
% 'string_adress_station, string_desc_line'); % all_stops
% formatSpec = '%u32, %s, %s, %s, %s\n';
fprintf(fid,'%s\n', '# string_name_station, uint_s_id, string_short_line');
all_stops = [all_stops(:,[3,1,2])];
formatSpec = '%s, %d, %s\n';

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
