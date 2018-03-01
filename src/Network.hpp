#pragma once

#include <iostream> // cout
#include <fstream> // ifstream

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

      // Discard header's lines
      while(ifs.good() && !ifs.eof()){
        char check = ifs.peek();
        if(check == '#'){
          char header_line[1024];
          ifs.getline(header_line,1024,'\n');
        }else{
          break;
        }
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

      // Discard header's lines
      while(ifs.good() && !ifs.eof()){
        char check = ifs.peek();
        if(check == '#'){
          char header_line[1024];
          ifs.getline(header_line,1024,'\n');
        }else{
          break;
        }
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
      this->display_travel(this->travel.second, this->graph_hashmap);
    }

    virtual void display_travel(unsigned int _end){
      this->display_travel(_end, this->graph_hashmap);
    }

    virtual void display_travel(unsigned int _end, const std::unordered_map<unsigned int, Node*>& _graph_hashmap){
      Node* cursor1 = std::min_element(_graph_hashmap.begin(),
                                       _graph_hashmap.end(),
                                       [](const std::pair<unsigned int, Node*>& a, const std::pair<unsigned int, Node*>& b){
                                         return (a.second->cost < b.second->cost);
                                       })->second;
      Node* cursor2 = _graph_hashmap.at(_end);

      std::cout << "\nBest way from " << cursor1->connection->stop->name << " to " << cursor2->connection->stop->name << " is:\n";
      std::list<Node*> way;
      way.push_front(cursor2);
      while(cursor1 != cursor2){
        cursor2 = _graph_hashmap.at(cursor2->from_id);
        way.push_front(cursor2);
      }

      Node* previous_node;
      int last_cost = 0;
      bool first = true;

      for(auto it = way.begin(); it != way.end(); ++it){
        if(first && std::next(it) != way.end()){
          if((*it)->connection->stop->line_id == (*(std::next(it)))->connection->stop->line_id){
            first = false;
            std::cout << "\t<Take line " << (*it)->connection->stop->line_id << ">" << std::endl;
            std::cout << "\t\tFrom " << (*it)->connection->stop->name;
            last_cost = (*it)->cost;
          }else{
            std::cout << "\tWalk to " << (*(std::next(it)))->connection->stop->name << " (" << (*(std::next(it)))->cost << " secs)" << std::endl;
          }
        }else if(!first && previous_node->connection->stop->line_id != (*it)->connection->stop->line_id){
          std::cout << " to " << previous_node->connection->stop->name << " (" << previous_node->cost-last_cost << " secs)" << std::endl;
          last_cost = previous_node->cost;
          std::cout << "\tWalk to " << (*it)->connection->stop->name << " (" << (*it)->cost-last_cost << " secs)" << std::endl;
          last_cost = (*it)->cost;
          if(std::next(it) != way.end()){
            if((*it)->connection->stop->line_id == (*(std::next(it)))->connection->stop->line_id){
              std::cout << "\t<Take line " << (*it)->connection->stop->line_id << ">" << std::endl;
              std::cout << "\t\tFrom " << (*it)->connection->stop->name;
            }else{
              std::cout << "\tWalk to " << (*it)->connection->stop->name << " (" << (*(std::next(it)))->cost << " secs)" << std::endl;
            }
          }
        }else if(!first && std::next(it) == way.end()){
          std::cout << " to " << (*it)->connection->stop->name << " (" << (*it)->cost-last_cost << " secs)" << std::endl;
          last_cost = (*it)->cost;
        }

        previous_node = *it;
      }
      std::cout << "After " << previous_node->cost << " secs, you have reached your destination!" << std::endl;
    }

    /**********************************************
      Compute algorithms
    */
    virtual void compute_travel(){
      this->compute_travel(this->travel.first, &this->connections, &this->graph, &this->graph_hashmap);
    }

    virtual void compute_travel(unsigned int _start){
      this->compute_travel(_start, &this->connections, &this->graph, &this->graph_hashmap);
    }

    virtual void compute_travel(std::list<Connection>* _connections, std::list<Node>* _graph, std::unordered_map<unsigned int, Node*>* _graph_hashmap){
      this->compute_travel(this->travel.first, _connections, _graph, _graph_hashmap);
    }

    virtual void compute_travel(unsigned int _start, std::list<Connection>* _connections, std::list<Node>* _graph, std::unordered_map<unsigned int, Node*>* _graph_hashmap){
      this->dijkstra(_start, _connections, _graph, _graph_hashmap);
    }

    virtual void compute_and_display_travel(){
      this->compute_and_display_travel(this->travel.first, this->travel.second, &this->connections, &this->graph, &this->graph_hashmap);
    }

    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end){
      this->compute_and_display_travel(_start, _end, &this->connections, &this->graph, &this->graph_hashmap);
    }

    virtual void compute_and_display_travel(std::list<Connection>* _connections, std::list<Node>* _graph, std::unordered_map<unsigned int, Node*>* _graph_hashmap){
      this->compute_and_display_travel(this->travel.first, this->travel.second, _connections, _graph, _graph_hashmap);
    }

    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end, std::list<Connection>* _connections, std::list<Node>* _graph, std::unordered_map<unsigned int, Node*>* _graph_hashmap){
      this->dijkstra(_start, _connections, _graph, _graph_hashmap);
      this->display_graph();
      this->display_travel(_end, *_graph_hashmap);
    }

    /**********************************************
      Protected methods
    */
  protected:
    // compute the dijkstra algorithm
    void dijkstra(unsigned int _start, std::list<Connection>* _connections, std::list<Node>* _graph, std::unordered_map<unsigned int, Node*>* _graph_hashmap){
      std::list<Node > tmp_graph;
      _graph_hashmap->clear();
      _graph->clear();

      // a checker si le trajet n'est pas possible aka first nexiste pas.... (dans set travel ? mais probleme a la construction)
      for(auto&& it: *_connections){
        Node new_node;
        new_node.connection = &it;
        new_node.cost = ((it.stop->id == _start) ? 0 : INF);
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

        _graph->push_back(*min_elem);
        _graph_hashmap->insert(std::pair<unsigned int, Node*>(_graph->back().connection->stop->id, &_graph->back()));
        tmp_graph.erase(min_elem);
      }
    }
  };
};
