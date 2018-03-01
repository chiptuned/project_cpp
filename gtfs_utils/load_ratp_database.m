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

  db.agency = readtable(fullfile(folder_db, 'agency.txt'));
  db.stops = readtable(fullfile(folder_db, 'stops.txt'));
  db.routes = readtable(fullfile(folder_db, 'routes.txt'));
  db.trips = readtable(fullfile(folder_db, 'trips.txt'));
  db.stop_times = readtable(fullfile(folder_db, 'stop_times.txt'));
  db.calendar = readtable(fullfile(folder_db, 'calendar.txt'));
  db.calendar_dates = readtable(fullfile(folder_db, 'calendar_dates.txt'));
  % db.fare_attributes = [];
  % db.fare_rules = [];
  % db.shapes = [];
  % db.frequencies = [];
  db.transfers = readtable(fullfile(folder_db, 'transfers.txt'));
  % db.feed_info = [];

  fprintf('Database loaded in %.0f seconds.\n', toc(tstart))
