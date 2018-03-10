#include <iostream>
#include <string>

#include "Network.hpp"
#include "Grade.hpp"

int main(int argc, char** argv){

  bool set_connections = false;
  bool set_stations = false;
  bool start_station = false;
  bool end_station = false;

  try{
    std::string stations_filename;
    std::string connections_filename;
    uint64_t start = 1722, end = 2062;
    if(argc % 2 == 1){
      for(int i = 1; i < argc; i+=2){
        if(strcmp(argv[i], "stations") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          set_stations = true;
          stations_filename = std::string(argv[i+1]);
        }else if(strcmp(argv[i], "connections") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          set_connections = true;
          connections_filename = std::string(argv[i+1]);
        }else if(strcmp(argv[i], "start") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          start_station = true;
          start = static_cast<uint64_t>(std::stoll(std::string(argv[i+1])));
        }else if(strcmp(argv[i], "end") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          end_station = true;
          end = static_cast<uint64_t>(std::stoll(std::string(argv[i+1])));
        }
      }

      if(!(set_connections && set_stations && start_station && end_station)){
        throw("usage (case sensitive): ./prog stations \"StationsFile.csv\" connections \"ConnectionsFile.csv\" start \"StartStation\" end \"EndStation\"");
      }else{
        (void) start;
        (void) end;
        travel::Network network(stations_filename, connections_filename);
        // network.compute_and_display_travel(start, end);

        travel::evaluate_small.dijkstra(network);

        std::cout << "Nice" << std::endl;
      }
    }
  }catch(const char* err_mess){
    std::cerr << "ERROR:\n\t" << err_mess << std::endl;
  }catch(const std::string& err_mess){
    std::cerr << "ERROR:\n\t" << err_mess << std::endl;
  }

  return 0;
}
