CXX=g++
CXXFLAGS=-std=c++11
CFLAGS=-Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -Wno-overlength-strings
SRC= *.cpp
ETU=Grade.hpp Grade.o Generic*
DATA= ../sample_data/connections.csv ../sample_data/stations.csv \
	../sample_data/c.csv ../sample_data/s.csv
OBJ=$(SRC:.cpp=.o)

all: depends a.out

a.out: $(OBJ)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $^ -o $@
	cp -r ../sample_data/ data/
	zip -r ../archive_etudiant.zip $(ETU) data/s.csv data/c.csv \
	 	data/connections.csv data/stations.csv
	rm -rf data/

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $^

depends:
	$(CXX) -MM $(SRC) $(CXXFLAGS) $(CFLAGS) > .depends

clean_files:
	rm -f $(OBJ) .depends a.out

clean: clean_files
	rm -f ../archive_etudiant.zip

.PHONY: all clean depends
