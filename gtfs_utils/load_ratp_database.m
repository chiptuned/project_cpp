function db = load_ratp_database()
  tstart = tic;
  %% Getting files
  folder_db = 'db';
  db_name = 'RATP_GTFS_FULL.zip';
  cond_download = ~exist(db_name, 'file'); % or files don't exist in db

  if cond_download
    % download, make a dir, unzip file
    url = 'http://dataratp.download.opendatasoft.com/RATP_GTFS_FULL.zip';
    websave(db_name, url);
    mkdir(folder_db)
    unzip(db_name, folder_db)
  end

  %% Making database variable

  % Any db in GTFS should have those :
  % agency.txt
  % stops.txt
  % routes.txt
  % trips.txt
  % stop_times.txt
  % calendar.txt
  % calendar_dates.txt
  % fare_attributes.txt
  % fare_rules.txt
  % shapes.txt
  % frequencies.txt
  % transfers.txt
  % feed_info.txt

  % Formats are crafted to match RATP GTFS database.
  db.agency = readtable(fullfile(folder_db, 'agency.txt'), ...
    'Format', '%u %s %s %s %s %s');
  db.stops = readtable(fullfile(folder_db, 'stops.txt'), ...
    'Format', '%u32 %u8 %q %q %f64 %f64 %u8 %u8');
  db.routes = readtable(fullfile(folder_db, 'routes.txt'), ...
    'Format', '%u32 %u8 %q %q %q %u8 %q %q %q');
  db.trips = readtable(fullfile(folder_db, 'trips.txt'), ...
    'Format', '%u32 %u32 %u64 %q %q %u8 %q');
  % for stop_times, it is faster to store duration in char array then cast
  db.stop_times = readtable(fullfile(folder_db, 'stop_times.txt'), ...
    'Format', '%u64 %q %q %u32 %u8 %q %q');
  times = array2table(seconds(duration(db.stop_times{:,2:3})), ...
    'VariableNames',db.stop_times.Properties.VariableNames(2:3));
  db.stop_times = [db.stop_times(:,1), times, db.stop_times(:,4:end)];

  db.calendar = readtable(fullfile(folder_db, 'calendar.txt'), ...
    'Format', '%u32 %u8 %u8 %u8 %u8 %u8 %u8 %u8 %u32 %u32');
  db.calendar_dates = readtable(fullfile(folder_db, 'calendar_dates.txt'), ...
    'Format', '%u32 %u32 %u8');
  % db.fare_attributes = [];
  % db.fare_rules = [];
  % db.shapes = [];
  % db.frequencies = [];
  db.transfers = readtable(fullfile(folder_db, 'transfers.txt'), ...
    'Format', '%u32 %u32 %u8 %u16');
  % db.feed_info = [];

  fprintf('Database loaded in %.0f seconds.\n', toc(tstart))
