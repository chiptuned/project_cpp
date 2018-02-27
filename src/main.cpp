
#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <fstream>
#include <cstring>
#include <limits>
#include <algorithm>

#include "Generic_class.hpp"

int main(int argc, char** argv){

  int start, end;
  std::list<Station> Stations;
  std::list<Connection> Connections;

  bool setConnections = false;
  bool setStations = false;
  bool startStation = false;
  bool endStation = false;

  try{
    for(int i = 1; i < argc; i+=2){
      if(strcmp(argv[i], "Stations") == 0){
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        setStations = true;
        readStations(argv[i+1], Stations);
      }else if(setStations == true && strcmp(argv[i], "Connections") == 0){
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        setConnections = true;
        readConnections(argv[i+1], Stations, Connections);
      }else if(strcmp(argv[i], "start") == 0){
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        startStation = true;
        start = std::stoi(std::string(argv[i+1]));
      }else if(strcmp(argv[i], "end") == 0){
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
        endStation = true;
        end = std::stoi(std::string(argv[i+1]));
      }
    }

    if(!(setConnections && setStations && startStation && endStation)){
      throw("usage: ./prog StationsFile.csv ConnectionsFile.csv StartStation");
    }else{
      std::cout << "\nnice\n" << std::endl;

      std::cout << std::endl << "List Stations:" << std::endl;
      for(auto it: Stations){
        std::cout << it << std::endl;
      }
      std::cout << std::endl << "List Connections:" << std::endl;
      for(auto it: Connections){
        std::cout << it << std::endl;
      }

      std::cout << std::endl << "List Noeuds:" << std::endl;
      Node* cursor1;
      Node* cursor2;
      auto graph = dijkstra(Connections, start);
      for(auto&& it: graph){
        if(std::get<0>(it)->id == start){
          cursor1 = &it;
        }else if(std::get<0>(it)->id == end){
          cursor2 = &it;
        }
        std::cout << it << std::endl;
      }

      std::cout << "\n\nBest way to " << std::get<0>(*cursor1)->name << " => " << std::get<0>(*cursor2)->name << " is:\n\t";
      std::list<Station*> way;
      way.push_front(std::get<0>(*cursor2));
      while(cursor2 != cursor1){
        for(auto&& it: graph){
          if(std::get<0>(it)->id == std::get<2>(*cursor2)){
            cursor2 = &it;
            way.push_front(std::get<0>(*cursor2));
            break;
          }
        }
      }

      for(auto&& it: way){
        std::cout << it->name << " => ";
      }
      std::cout << "Vous êtes arrivé!" << std::endl;
    }
  }catch(const char* errMess){
    std::cerr << "ERROR:\n\t" << errMess << std::endl;
  }

  return 0;
}
