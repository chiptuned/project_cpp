#pragma once

#include "Generic_class.hpp"

namespace travel{
  /**********************************************
     UTILS
   */
  // overload operator<< for a Station
  std::ostream& operator<<(std::ostream& _os, travel::Station& _stop){
    _os << "Station: " << _stop.name << " labeled " << _stop.id << " (Line: " << _stop.line_id << ")";
    return _os;
  }

  // overload operator<< for a Connection
  std::ostream& operator<<(std::ostream& _os, travel::Connection& _connection){
    _os << "The stations: " << *(_connection.stop) << " is connected to:\n";
    for(auto&& it: _connection.neighbors){
      _os << "\t" << *(it.first) << " with a weight of " << it.second << " secs" << std::endl;
    }
    return _os;
  }

  // overload operator<< for a Node
  std::ostream& operator<<(std::ostream& _os, const travel::Node& _node){
    _os << "Node " << std::get<0>(_node)->name << " (" << std::get<0>(_node)->id << "): cost " << std::get<1>(_node) << " from " << std::get<2>(_node);
    return _os;
  }

  /**********************************************
     Network class
   */
  class Network: public travel::Generic_class{
    /**********************************************
      Public methods
     */
  public:
    /**********************************************
      Constructor & destructor
     */
    Network(): Generic_class(){
    }

    Network(int _start, int _end): Generic_class(_start, _end){
    }

    virtual ~Network(){
    }

    /**********************************************
      Setters
     */
    Constructor & destructor
    virtual void set_travel(int _start, int _end){
      this->travel = std::pair<int, int>(_start, _end);
    }

    virtual void set_start(int _start){
      this->travel.first = _start;
    }

    virtual void set_end(int _end){
      this->travel.second = _end;
    }

    virtual void read_stations(std::string _filename){
      this->read_stations(_filename.c_str());
    }

    virtual void read_stations(char* _filename){
      std::ifstream ifs(_filename, std::ifstream::in);
      this->stations.clear();
      this->stations_hashmap.clear();
      this->connections.clear();
      this->connections_hashmap.clear();
      this->graph.clear();
      this->graph_hashmap.clear();

      while(ifs.good() && !ifs.eof()){
        travel::Station new_station;
        char tmp[256];

        ifs.getline(tmp, 256, ';');
        if(strcmp(tmp, "") != 0){
          new_station.name = tmp;
          ifs.getline(tmp, 256, '\n');
          if(strcmp(tmp, "") != 0){
            new_station.id = std::stoi(std::string(tmp));
            this->stations.push_back(new_station);
            this->stations_hashmap.insert(std::make_pair<int, Station*>(this->stations.back().id, &this->stations.back()));
          }
        }else{
          ifs.getline(tmp,256,'\n');
        }
      }

      ifs.close();
    }

    virtual void read_connections(std::string _filename){
      this->read_connections(_filename.c_str());
    }

    virtual void read_connections(char* _filename){
      std::ifstream ifs(_filename, std::ifstream::in);
      this->connections.clear();
      this->connections_hashmap.clear();
      this->graph.clear();
      this->graph_hashmap.clear();

      while(ifs.good() && !ifs.eof()){
        char a[256],b[256],w[256];

        ifs.getline(a, 256, ';');
        ifs.getline(b, 256, ';');
        ifs.getline(w, 256, '\n');
        if(strcmp(a, "") != 0 && strcmp(b, "") != 0 && strcmp(w, "") != 0){
          auto first = this->stations_hashmap.find(std::stoi(std::string(a)));
          auto second = this->stations_hashmap.find(std::stoi(std::string(b)));

          if(first == this->stations_hashmap.end() || second == this->stations_hashmap.end()){
            throw("Unknown Station");
          }else{
            auto currentConnection = this->connections_hashmap.find(first->second->id);

            if(currentConnection == this->connections_hashmap.end()){
              travel::Connection new_connection;
              new_connection.stop = first->second;
              new_connection.neighbors.push_back(std::pair<Station*,int>(second->second,std::stoi(w)));
              this->connections.push_back(new_connection);
              this->connections_hashmap.insert(std::make_pair<int, Connection*>(this->connections.back().stop->id, &this->connections.back()));
            }else{
              currentConnection->second->neighbors.push_back(std::pair<Station*,int>(second->second,std::stoi(w)));
            }
          }
        }
      }

      ifs.close();
    }

    /**********************************************
      Display
     */
    virtual void display_stations(){
      std::cout << std::endl << "List Stations:" << std::endl;
      for(auto it: this->stations){
        std::cout << it << std::endl;
      }
    }

    virtual void display_connections(){
      std::cout << std::endl << "List Connections:" << std::endl;
      for(auto it: this->connections){
        std::cout << it << std::endl;
      }
    }

    virtual void display_graph(){
      for(auto&& it: this->graph){
        std::cout << it << std::endl;
      }
    }

    virtual void display_travel(){
      Node* cursor1 = graph_hashmap.at(this->travel.first);
      Node* cursor2 = graph_hashmap.at(this->travel.second);

      std::cout << "\n\nBest way to " << std::get<0>(*cursor1)->name << " => " << std::get<0>(*cursor2)->name << " is:\n\t";
      std::list<Station*> way;
      way.push_front(std::get<0>(*cursor2));
      while(cursor1 != cursor2){
        cursor2 = this->graph_hashmap.at(std::get<2>(*cursor2));
        way.push_front(std::get<0>(*cursor2));
      }

      for(auto&& it: way){
        std::cout << it->name << " => ";
      }
      std::cout << "You have reached your destination!" << std::endl << std::endl;
    }

    /**********************************************
      Compute algorithms
     */
    virtual void compute_travel(){
      this->compute_travel(this->travel.first, this->travel.second);
    }

    virtual void compute_travel(int _start, int _end){
      this->travel = std::pair<int,int>(_start, _end);
      this->dijkstra();
      this->display_graph();
      this->display_travel();
    }

    /**********************************************
      Protected methods
     */
  protected:
    // compute the dijkstra algorithm
    virtual void dijkstra(){
      std::list<Node > tmp_graph;
      this->graph_hashmap.clear();
      this->graph.clear();

      for(auto&& it: this->connections){
        Node new_node;
        std::get<0>(new_node) = it.stop;

        // a checker si le trajet n'est pas possible
        if(it.stop->id == this->travel.first){
          std::get<1>(new_node) = 0;
          std::get<2>(new_node) = std::get<0>(new_node)->id;
        }else{
          std::get<1>(new_node) = INF;
          std::get<2>(new_node) = std::get<0>(new_node)->id;
        }
        tmp_graph.push_back(new_node);
      }

      while(!tmp_graph.empty()){
        auto min_elem = std::min_element(tmp_graph.begin(),
                                         tmp_graph.end(),
                                         [](Node a, Node b){
                                           return std::get<1>(a) < std::get<1>(b);
                                         });

        auto current_connection = connections_hashmap.find(std::get<0>(*min_elem)->id);

        for(auto&& it: tmp_graph){
          for(auto&& it2: current_connection->second->neighbors){
            if(std::get<0>(it)->id == it2.first->id){
              auto weight = std::get<1>(*min_elem)+it2.second;
              if(std::get<1>(it) > weight){
                std::get<1>(it) = weight;
                std::get<2>(it) = std::get<0>(*min_elem)->id;
              }
            }
          }
        }

        this->graph.push_back(*min_elem);
        this->graph_hashmap.insert(std::make_pair<int, Node*>(std::get<0>(this->graph.back())->id, &this->graph.back()));
        tmp_graph.erase(min_elem);
      }
    }
  };
};
