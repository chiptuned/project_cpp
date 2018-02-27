clearvars -except db
close all;

% https://developers.google.com/transit/gtfs/reference/?hl=fr

if ~exist('db', 'var')
  db = load_ratp_database;
end

make_gtfs_files(db, 'stations.csv', 'connections.csv');
