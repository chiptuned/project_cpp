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
    _os << "Node " << _node.connection->stop->name << " (" << _node.connection->stop->id << "): cost " << _node.cost << " from " << _node.from_id;
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
    virtual void set_travel(unsigned int _start, unsigned int _end){
      this->travel = std::pair<unsigned int, unsigned int>(_start, _end);
    }

    virtual void set_travel(std::pair<unsigned int, unsigned int> _travel){
      this->travel = _travel;
    }

    virtual void set_start(unsigned int _start){
      this->travel.first = _start;
    }

    virtual void set_end(unsigned int _end){
      this->travel.second = _end;
    }

    virtual void read_stations(const std::string& _filename){
      this->read_stations(_filename.c_str(), &this->stations, &this->stations_hashmap);
    }

    virtual void read_stations(const char* _filename){
      this->read_stations(_filename, &this->stations, &this->stations_hashmap);
    }

    virtual void read_stations(const std::string& _filename, std::list<Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap){
      this->read_stations(_filename.c_str(), _stations, _stations_hashmap);
    }

    virtual void read_stations(const char* _filename, std::list<Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap){
      std::ifstream ifs(_filename, std::ifstream::in);
      _stations->clear();
      _stations_hashmap->clear();
      if(_stations == &this->stations){
        this->connections.clear();
        this->connections_hashmap.clear();
        this->graph.clear();
        this->graph_hashmap.clear();
      }

      while(ifs.good() && !ifs.eof()){
        travel::Station new_station;
        char tmp[256];

        ifs.getline(tmp, 256, ';');
        if(strcmp(tmp, "") != 0){
          new_station.name = tmp;
          ifs.getline(tmp, 256, ';');
          if(strcmp(tmp, "") != 0){
            new_station.id = static_cast<unsigned int>(std::stoi(std::string(tmp)));
            ifs.getline(tmp, 256, '\n');
            if(strcmp(tmp, "") != 0){
              new_station.line_id = static_cast<unsigned int>(std::stoi(std::string(tmp)));
              _stations->push_back(new_station);
              _stations_hashmap->insert(std::pair<unsigned int, travel::Station*>(_stations->back().id, &_stations->back()));
            }
          }
        }else{
          ifs.getline(tmp,256,'\n');
        }
      }

      ifs.close();
    }

    virtual void read_connections(const std::string& _filename){
      this->read_connections(_filename.c_str(), this->stations_hashmap, &this->connections, &this->connections_hashmap);
    }

    virtual void read_connections(const char* _filename){
      this->read_connections(_filename, this->stations_hashmap, &this->connections, &this->connections_hashmap);
    }

    virtual void read_connections(const std::string& _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap){
      this->read_connections(_filename.c_str(), _stations_hashmap, _connections, _connections_hashmap);
    }

    virtual void read_connections(const char* _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap){
      std::ifstream ifs(_filename, std::ifstream::in);
      _connections->clear();
      _connections_hashmap->clear();
      if(_connections == &this->connections){
        this->graph.clear();
        this->graph_hashmap.clear();
      }

      while(ifs.good() && !ifs.eof()){
        char a[256],b[256],w[256];

        ifs.getline(a, 256, ';');
        ifs.getline(b, 256, ';');
        ifs.getline(w, 256, '\n');
        if(strcmp(a, "") != 0 && strcmp(b, "") != 0 && strcmp(w, "") != 0){
          auto first = _stations_hashmap.find(static_cast<unsigned int>(std::stoi(std::string(a))));
          auto second = _stations_hashmap.find(static_cast<unsigned int>(std::stoi(std::string(b))));

          if(first == _stations_hashmap.end() || second == _stations_hashmap.end()){
            throw("Unknown Station");
          }else{
            auto knownConnection = _connections_hashmap->find(first->second->id);

            if(knownConnection == _connections_hashmap->end()){
              travel::Connection new_connection;
              new_connection.stop = first->second;
              new_connection.neighbors.push_back(std::pair<Station*,int>(second->second, std::stoi(w)));
              _connections->push_back(new_connection);
              _connections_hashmap->insert(std::pair<unsigned int, Connection*>(this->connections.back().stop->id, &this->connections.back()));
            }else{
              knownConnection->second->neighbors.push_back(std::pair<Station*,int>(second->second, std::stoi(w)));
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
      std::cout << std::endl << "List Nodes:" << std::endl;
      for(auto&& it: this->graph){
        std::cout << it << std::endl;
      }
    }

    virtual void display_travel(){
      Node* cursor1 = graph_hashmap.at(this->travel.first);
      Node* cursor2 = graph_hashmap.at(this->travel.second);

      std::cout << "\nBest way from " << cursor1->connection->stop->name << " to " << cursor2->connection->stop->name << " is:\n";
      std::list<Node*> way;
      way.push_front(cursor2);
      while(cursor1 != cursor2){
        cursor2 = this->graph_hashmap.at(cursor2->from_id);
        way.push_front(cursor2);
      }

      Node* previous;
      bool first = true;
      int lastCost = 0;
      for(auto&& it: way){
        if(first){
          first = false;
          std::cout << "\t<Take line " << it->connection->stop->line_id << ">" << std::endl;
          std::cout << "\t\tFrom " << it->connection->stop->name;
        }else if(previous->connection->stop->line_id != it->connection->stop->line_id){
          std::cout << " to " << previous->connection->stop->name << " (" << previous->cost-lastCost << " secs)" << std::endl;
          lastCost = previous->cost;
          std::cout << "\tWalk " << it->cost-lastCost << " secs" << std::endl;
          lastCost = it->cost;
          std::cout << "\t<Take line " << it->connection->stop->line_id << ">" << std::endl;
          std::cout << "\t\tFrom " << it->connection->stop->name;
        }
        previous = it;
      }
      std::cout << " to " << previous->connection->stop->name << " (" << previous->cost-lastCost << " secs)" << std::endl;
      std::cout << "After " << previous->cost << " secs, you have reached your destination!" << std::endl;
    }

    /**********************************************
      Compute algorithms
    */
    virtual void compute_travel(){
      this->compute_travel(this->travel.first, this->travel.second);
    }

    virtual void compute_travel(unsigned int _start, unsigned int _end){
      this->travel = std::pair<int,int>(_start, _end);
      this->dijkstra();
    }

    virtual void compute_and_display_travel(){
      this->compute_and_display_travel(this->travel.first, this->travel.second);
    }

    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end){
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
    void dijkstra(){
      std::list<Node > tmp_graph;
      this->graph_hashmap.clear();
      this->graph.clear();

      // a checker si le trajet n'est pas possible aka first nexiste pas.... (dans set travel ? mais probleme a la construction)
      for(auto&& it: this->connections){
        Node new_node;
        new_node.connection = &it;
        new_node.cost = ((it.stop->id == this->travel.first) ? 0 : INF);
        new_node.from_id = new_node.connection->stop->id;
        tmp_graph.push_back(new_node);
      }

      while(!tmp_graph.empty()){
        auto min_elem = std::min_element(tmp_graph.begin(),
                                         tmp_graph.end(),
                                         [](const Node& a, const Node& b){
                                           return a.cost < b.cost;
                                         });

        // auto current_connection = connections_hashmap.find(min_elem->connecton->stop->id);

        for(auto&& it: tmp_graph){
          for(auto&& it2: min_elem->connection->neighbors){//current_connection->second->neighbors){
            if(it.connection->stop->id == it2.first->id){
              auto weight = min_elem->cost+it2.second;
              if(it.cost > weight){
                it.cost = weight;
                it.from_id = min_elem->connection->stop->id;
              }
            }
          }
        }

        this->graph.push_back(*min_elem);
        this->graph_hashmap.insert(std::pair<unsigned int, Node*>(this->graph.back().connection->stop->id, &this->graph.back()));
        tmp_graph.erase(min_elem);
      }
    }
  };
};
