clearvars -except db
close all;

% https://developers.google.com/transit/gtfs/reference/?hl=fr

if ~exist('db', 'var')
  db = load_ratp_database;
end
date = [fullfile('..', 'sample_data')];
file_stations = fullfile(date, 's.csv');
file_connections = fullfile(date, 'c.csv');
make_gtfs_files(db, file_stations, file_connections);

pts = [table2array(db.stops(:,'stop_lat')), table2array(db.stops(:,'stop_lon'))];
figure;
plot(pts(:,2), pts(:,1), '.')
hold on;
plot(pts(575,2), pts(575,1), '.r') %pas campo
axis equal

%% Graph

gen_c = readtable(file_connections, ...
  'Format', '%u32 %u32 %u32');

from_to_array = table2array(gen_c(:,1:2));
stops_array = [double(table2array(db.stops(:,{'stop_id'}))), ...
    table2array(db.stops(:,{'stop_lat', 'stop_lon'}))];
rand_scale = 6E-4;
%rand_scale = 0;
stops_array(:,2:3) = stops_array(:,2:3)+rand(size(stops_array(:,2:3)))*rand_scale;

uniq_from_to = unique(from_to_array, 'rows');

[~, loc1] = ismember(uniq_from_to(:,1), stops_array(:,1));
[~, loc2] = ismember(uniq_from_to(:,2), stops_array(:,1));

%% Drawing graph, showing some interesting nodes
fh = figure;
symbol = 'o';
for ind = 1:size(uniq_from_to,1)
  plot([stops_array(loc1(ind),3), stops_array(loc2(ind),3)], ...
     [stops_array(loc1(ind),2), stops_array(loc2(ind),2)], [symbol, 'r'], ...
     'MarkerSize',0.1);
  patch('XData', [stops_array(loc1(ind),3), stops_array(loc2(ind),3)], ...
    'YData', [stops_array(loc1(ind),2), stops_array(loc2(ind),2)], 'LineWidth', 0.3);
  hold on;
end
axis equal;

