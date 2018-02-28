#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <fstream>
#include <cstring>
#include <limits>
#include <algorithm>

#include "Network.hpp"

int main(int argc, char** argv){
  travel::Network network;
  travel::Generic_class* ptr_network = &network;

  bool set_connections = false;
  bool set_stations = false;
  bool start_station = false;
  bool end_station = false;

  try{
    if(argc % 2 == 1){
      for(int i = 1; i < argc; i+=2){
        if(strcmp(argv[i], "stations") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          set_stations = true;
          ptr_network->read_stations(argv[i+1]);//, Stations);
        }else if(set_stations == true && strcmp(argv[i], "connections") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          set_connections = true;
          ptr_network->read_connections(argv[i+1]);//, Stations, Connections);
        }else if(strcmp(argv[i], "start") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          start_station = true;
          ptr_network->set_start(std::stoi(std::string(argv[i+1])));
        }else if(strcmp(argv[i], "end") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          end_station = true;
          ptr_network->set_end(std::stoi(std::string(argv[i+1])));
        }
      }
    }

    if(!(set_connections && set_stations && start_station && end_station)){
      throw("usage (case sensitive): ./prog stations \"StationsFile.csv\" connections \"ConnectionsFile.csv\" start \"StartStation\" end \"EndStation\"");
    }else{
      std::cout << "\nnice\n" << std::endl;

      ptr_network->display_stations();
      ptr_network->display_connections();
      ptr_network->compute_travel();
      ptr_network->compute_travel(5,2);
    }
  }catch(const char* err_mess){
    std::cerr << "ERROR:\n\t" << err_mess << std::endl;
  }

  return 0;
}
