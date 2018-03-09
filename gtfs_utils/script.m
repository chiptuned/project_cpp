clearvars -except db
close all;

% https://developers.google.com/transit/gtfs/reference/?hl=fr

if ~exist('db', 'var')
  db = load_ratp_database;
end
date = [fullfile('..', 'sample_data')];
file_stations = fullfile(date, 's.csv');
file_connections = fullfile(date, 'c.csv');
make_gtfs_files(db, file_stations, file_connections, 5);

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
stops_array(:,2:3) = stops_array(:,2:3)+rand(size(stops_array(:,2:3)))*rand_scale;

uniq_from_to = unique(from_to_array, 'rows');

[~, loc1] = ismember(uniq_from_to(:,1), stops_array(:,1));
[~, loc2] = ismember(uniq_from_to(:,2), stops_array(:,1));

%% Drawing graph, showing some interesting nodes
fh = figure;
symbol = '.';
for ind = 1:size(uniq_from_to,1)
   plot([stops_array(loc1(ind),3), stops_array(loc2(ind),3)], ...
     [stops_array(loc1(ind),2), stops_array(loc2(ind),2)], [symbol, 'k'], ...
     'MarkerSize',5);
  hold on;
  patch('XData', [stops_array(loc1(ind),3), stops_array(loc2(ind),3)], ...
    'YData', [stops_array(loc1(ind),2), stops_array(loc2(ind),2)], 'LineWidth', 0.1);
end
axis equal;

% 2117 et 2404
db.stops([465,735,6191,11605,389,300,375],:)
% Fixed with a good hour choice (18h to 18h05)
plot(pts(465,2), pts(465,1), [symbol, 'b'])
plot(pts(6191,2), pts(6191,1), [symbol, 'b'])
plot(pts(735,2), pts(735,1), [symbol, 'b'])
% Fixed : Line 4 : clignancourt simplon marcadet chateau rouge, connections?
plot(pts(11605,2), pts(11605,1), [symbol, 'b'])
plot(pts(389,2), pts(389,1), [symbol, 'b'])
% Fixed : Line 6 : dupleix to cambronne?
plot(pts(300,2), pts(300,1), [symbol, 'b'])
plot(pts(375,2), pts(375,1), [symbol, 'b'])
