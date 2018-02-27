#pragma once

#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <tuple>

#define INF std::numeric_limits<int>::max()

typedef struct __station{
  std::string name; // how is called the station
  unsigned int id; // identify the station with a unique number
  unsigned int line_id; // identify the line of the station
  friend std::ostream& operator<<(std::ostream& _os, const struct __station& _stop){
    _os << "Station: " << _stop.name << " labeled " << _stop.id << " (Line: " << _stop.line_id << ")";
    return _os;
  }
}Station; // a station is a stop for the net represented by a "name", an "id" and a "line"

typedef struct __connection{
  Station* stop; // reference station of the connection
  std::list<std::pair<Station*, int> > neighbors; // list of pair of station and static cost to join this station from the reference station
  friend std::ostream& operator<<(std::ostream& _os, const struct __connection& _connection){
    _os << "The stations: " << *(_connection.stop) << " is connected to:\n";
    for(auto&& it: _connection.neighbors){
      _os << "\t" << *(it.first) << " with a weight of " << it.second << " secs" << std::endl;
    }
    return _os;
  }
}Connection; // a connection represent a "station" connected to a "list of other stations with a cost" to join it

typedef std::tuple<Station*, int, int> Node; // a node is a triplet formed by a "station" and the "dynamic cost" to join it from a "previous station"

std::ostream& operator<<(std::ostream& _os, const Node& _node){
  _os << "Node " << std::get<0>(_node)->name << " (" << std::get<0>(_node)->id << "): cost " << std::get<1>(_node) << " from " << std::get<2>(_node);
  return _os;
}

class Generic_class{

public:

protected:
};


void readStations(char* _filename, std::list<Station>& _list){
  std::ifstream ifs(_filename, std::ifstream::in);

  while(ifs.good() && !ifs.eof()){
    Station newStation;
    char tmp[256];

    ifs.getline(tmp, 256, ';');
    if(strcmp(tmp, "") != 0){
      newStation.name = tmp;
      ifs.getline(tmp, 256, '\n');
      if(strcmp(tmp, "") != 0){
        newStation.id = std::stoi(std::string(tmp));
        // newStation.affiche();
        _list.push_back(newStation);
      }
    }else{
      ifs.getline(tmp,256,'\n');
    }
  }

  ifs.close();
}

void readConnections(char* _filename, std::list<Station>& _Stations, std::list<Connection>& _Connections){
  std::ifstream ifs(_filename, std::ifstream::in);

  while(ifs.good() && !ifs.eof()){
    char a[256],b[256],w[256];

    ifs.getline(a, 256, ';');
    ifs.getline(b, 256, ';');
    ifs.getline(w, 256, '\n');
    if(strcmp(a, "") != 0 && strcmp(b, "") != 0 && strcmp(w, "") != 0){
      Station* first = NULL;
      Station* second = NULL;
      for(auto&& it: _Stations){
        if(it.id == std::stoi(std::string(a))){
          first = &it;
          break;
        }
      }

      for(auto&& it: _Stations){
        if(it.id == std::stoi(std::string(b))){
          second = &it;
          break;
        }
      }

      if(first == NULL || second == NULL){
        throw("Unknown Station");
      }else{
        bool found = false;
        for(auto&& it: _Connections){
          if(it.stop->id == first->id){
            found = true;
            it.neighbors.push_back(std::pair<Station*,int>(second,std::stoi(w)));
            break;
          }
        }
        if(found == false){
          Connection newConnection;
          newConnection.stop = first;
          newConnection.neighbors.push_back(std::pair<Station*,int>(second,std::stoi(w)));
          _Connections.push_back(newConnection);
        }
      }
    }
  }

  ifs.close();
}

std::list<Node > dijkstra(const std::list<Connection>& _connections, int _start){
  std::list<Node > graph;
  std::list<Node > blackList;

  for(auto&& it: _connections){
    Node newNode;
    std::get<0>(newNode) = it.stop;

    if(it.stop->id == _start){
      std::get<1>(newNode) = 0;
      std::get<2>(newNode) = std::get<0>(newNode)->id;
    }else{
      std::get<1>(newNode) = INF;
      std::get<2>(newNode) = std::get<0>(newNode)->id;
    }
    graph.push_back(newNode);
  }

  while(!graph.empty()){
    auto minElem = std::min_element(graph.begin(),
                                    graph.end(),
                                    [](Node a, Node b){
                                      return std::get<1>(a) < std::get<1>(b);
                                    });

    Connection curConnection;
    for(auto&& it: _connections){
      if(it.stop->id == std::get<0>(*minElem)->id){
        curConnection = it;
        break;
      }
    }

    for(auto&& it: graph){
      for(auto&& it2: curConnection.neighbors){
        if(std::get<0>(it)->id == it2.first->id){
          auto weight = std::get<1>(*minElem)+it2.second;
          if(std::get<1>(it) > weight){
            std::get<1>(it) = weight;
            std::get<2>(it) = std::get<0>(*minElem)->id;
          }
        }
      }
    }

    blackList.push_back(*minElem);
    graph.erase(minElem);
  }

  return blackList;
}
