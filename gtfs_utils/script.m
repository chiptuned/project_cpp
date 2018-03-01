clearvars -except db
close all;

% https://developers.google.com/transit/gtfs/reference/?hl=fr

if ~exist('db', 'var')
  db = load_ratp_database;
end

make_gtfs_files(db, 'stations.csv', 'connections.csv');

pts = [table2array(db.stops(:,'stop_lat')), table2array(db.stops(:,'stop_lon'))];
plot(pts(:,2), pts(:,1), '.')
hold on;
plot(pts(575,2), pts(575,1), '.r') %campo
axis equal
