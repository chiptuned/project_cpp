all :
	printf "R\"\"(" > c_hacked.csv && cat c.csv >> c_hacked.csv && printf ")\"\"" >> c_hacked.csv
	printf "R\"\"(" > connections_hacked.csv && cat connections.csv >> connections_hacked.csv && printf ")\"\"" >> connections_hacked.csv
	printf "R\"\"(" > s_hacked.csv && cat s.csv >> s_hacked.csv && printf ")\"\"" >> s_hacked.csv
	printf "R\"\"(" > stations_hacked.csv && cat stations.csv >> stations_hacked.csv && printf ")\"\"" >> stations_hacked.csv

clean_files :
	rm -f *_hacked.csv
