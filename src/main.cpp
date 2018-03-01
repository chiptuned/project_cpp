#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <fstream>
#include <cstring>
#include <limits>
#include <algorithm>

#include "Network.hpp"
#include "grade.hpp"

int main(int argc, char** argv){
  travel::Network network;
  travel::Generic_class* ptr_network = &network;

  bool set_connections = false;
  bool set_stations = false;
  bool start_station = false;
  bool end_station = false;

  try{
    std::string stations_filename;
    std::string connections_filename;
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
          ptr_network->set_start(static_cast<unsigned int>(std::stoi(std::string(argv[i+1]))));
        }else if(strcmp(argv[i], "end") == 0){
          std::cout << "Arg " << i << ": " << argv[i] << std::endl;
          end_station = true;
          ptr_network->set_end(static_cast<unsigned int>(std::stoi(std::string(argv[i+1]))));
        }
      }
    }

    if(!(set_connections && set_stations && start_station && end_station)){
      throw("usage (case sensitive): ./prog stations \"StationsFile.csv\" connections \"ConnectionsFile.csv\" start \"StartStation\" end \"EndStation\"");
    }else{
      std::cout << "\nnice\n" << std::endl;
      std::list<travel::Station> l;
      std::unordered_map<unsigned int, travel::Station*> ml;
      std::list<travel::Connection> c;
      std::unordered_map<unsigned int, travel::Connection*> mc;

      ptr_network->read_stations(stations_filename.c_str());
      ptr_network->read_connections(connections_filename.c_str());
      // ptr_network->display_stations();
      // ptr_network->display_connections();
      ptr_network->compute_travel();
      // ptr_network->display_graph();
      ptr_network->display_travel();

      /*
      ptr_network->read_stations(stations_filename.c_str(),&l,&ml);
      ptr_network->read_connections(connections_filename.c_str(),ml,&c,&mc);
      ptr_network->compute_travel();
      ptr_network->display_travel();

      ptr_network->read_stations(stations_filename);
      ptr_network->read_connections(connections_filename);
      ptr_network->compute_travel();
      ptr_network->display_travel();

      ptr_network->read_stations(stations_filename,&l,&ml);
      ptr_network->read_connections(connections_filename,ml,&c,&mc);
      ptr_network->compute_travel();
      ptr_network->display_travel();

      network.read_stations(stations_filename.c_str());
      network.read_connections(connections_filename.c_str());
      network.compute_travel();
      network.display_travel();

      network.read_stations(stations_filename.c_str(),&l,&ml);
      network.read_connections(connections_filename.c_str(),ml,&c,&mc);
      network.compute_travel();
      network.display_travel();

      network.read_stations(stations_filename);
      network.read_connections(connections_filename);
      network.compute_travel();
      network.display_travel();

      network.read_stations(stations_filename,&l,&ml);
      network.read_connections(connections_filename,ml,&c,&mc);
      network.compute_travel();
      network.display_travel();*/

      // grade_max(ptr_network);
    }
  }catch(const char* err_mess){
    std::cerr << "ERROR:\n\t" << err_mess << std::endl;
  }

  return 0;
}
