CXX=g++
CXXFLAGS=-std=c++11
CFLAGS=-Wall -Wextra -Werror -pedantic -pedantic-errors -O3
SRC= *.cpp
ETU=Grade.hpp Grade.o Generic*
OBJ=$(SRC:.cpp=.o)

all: depends a.out

a.out: $(OBJ)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $^ -o $@
	tar -cf archive_etudiant.tar $(ETU)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $^

depends:
	$(CXX) -MM $(SRC) $(CXXFLAGS) $(CFLAGS) > .depends

clean:
	rm -f $(OBJ) .depends

.PHONY: all clean depends
