function [] = make_gtfs_files(db, fn1, fn2);

pts = [table2array(db.stops(:,'stop_lat')), table2array(db.stops(:,'stop_lon'))];
plot(pts(:,2), pts(:,1), '.')
hold on;
plot(pts(575,2), pts(575,1), '.r') %campo
axis equal

stops = db.stops(:,[4, 1]);
transfers = db.transfers(:,[1, 2, 4]);

old_idx_stop = table2array(stops(:,2));
stops(:,2) = array2table((1:size(stops,1))', 'VariableNames', {'stop_id'});

route_to_trips = table2array(db.trips(:,{'route_id', 'trip_id'}));
stop_times = table2array(db.stop_times(:,1:4));

bad_routes = [];
for route = table2array(db.routes(:,'route_id'))'
  trip = route_to_trips(find(route==route_to_trips(:,1),1),2);
  if isempty(trip)
    bad_routes = [bad_routes, route];
  else

  end
end

% writetable(allstops, fn1, 'WriteVariableNames', false, 'QuoteStrings', false)
% writetable(transfers, 'fn2, 'WriteVariableNames', false)
