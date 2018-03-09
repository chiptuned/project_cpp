#pragma once

#include <iostream>
#include <set>
#include <algorithm>

#include "Connection_parser.hpp"

#define INF std::numeric_limits<uint64_t>::max()

namespace travel{
  struct Node{
    uint64_t id;
    uint64_t cost;
    uint64_t from_id;
    bool operator()(const Node& a, const Node& b) const{
      return a.cost < b.cost;
    }

    bool operator==(const Node& a) const{
      return id == a.id;
    }

    /*
    bool operator<(const Node& a) const{
      return this->cost < a.cost;
    }
    bool operator<=(const Node& a) const{
      return this->cost <= a.cost;
    }
    bool operator>(const Node& a) const{
      return this->cost > a.cost;
    }
    bool operator>=(const Node& a) const{
      return this->cost >= a.cost;
      }*/
  };

  class Dijkstra: public Connection_parser{
  public:
    Dijkstra(const std::string& _stations_filename, const std::string& _connections_filename):
      Connection_parser(_stations_filename, _connections_filename){
    }

    std::vector<std::pair<uint64_t,uint64_t>> compute_travel(uint64_t _start, uint64_t _end){
      std::vector<std::pair<uint64_t,uint64_t>> travel_stations;
      // std::vector<Node> graph_set;

      std::unordered_map<uint64_t,Node> graph_hashmap;
      std::unordered_map<uint64_t,Node> graph_hashmap_save;

      for(auto&& it: this->connections_hashmap){
        Node new_node;
        new_node.id = it.first;
        new_node.from_id = _start;
        if(it.first != _start){
          new_node.cost = INF;
        }else{
          new_node.cost = 0;
        }
        graph_hashmap.insert(std::pair<uint64_t,Node>(new_node.id, new_node));
      }

      while(!graph_hashmap.empty()){
        auto min_elem = min_element(graph_hashmap.begin(), graph_hashmap.end(),
                                    [](const std::pair<uint64_t,Node>& a,const std::pair<uint64_t,Node>& b){
                                      return a.second.cost < b.second.cost;
                                    });

        auto connected_stations = this->connections_hashmap.find(min_elem->second.id);
        for(auto&& it: connected_stations->second){
          auto weight = ((INF - min_elem->second.cost > it.second) ? it.second+min_elem->second.cost : INF);
          auto node = graph_hashmap.find(it.first);
          if(node == graph_hashmap.end()){
            continue;
          }
          if(node->second.cost >= weight){
            node->second.cost = weight;
            node->second.id = node->second.id;
            node->second.from_id = min_elem->second.id;
          }
        }

        graph_hashmap_save.insert(std::pair<uint64_t,Node>(min_elem->second.id, min_elem->second));
        if(min_elem->second.id == _end){
          break;
        }

        graph_hashmap.erase(min_elem->second.id);
      }

      auto cursor = graph_hashmap_save.find(_end)->second;

      travel_stations.push_back(std::pair<uint64_t,uint64_t>(cursor.id,cursor.cost));
      while(cursor.id != _start){
        cursor = graph_hashmap_save.find(cursor.from_id)->second;
        travel_stations.push_back(std::pair<uint64_t,uint64_t>(cursor.id,cursor.cost));
      }
      std::reverse(travel_stations.begin(), travel_stations.end());
      return travel_stations;
    }

    std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end){
      auto travel_stations = this->compute_travel(_start, _end);

      this->display_travel(travel_stations);

      return travel_stations;
    }

  private:
    void display_travel(const std::vector<std::pair<uint64_t,uint64_t> >& _travel_stations){
      std::cout << "\nBest way from " << this->stations_hashmap.find(_travel_stations.front().first)->second.name
                << " to " << this->stations_hashmap.find(_travel_stations.back().first)->second.name << " is:\n";

      std::pair<uint64_t,uint64_t> previous_node;
      uint64_t last_cost = 0;
      bool first = true;

      for(auto it = _travel_stations.begin(); it != _travel_stations.end(); ++it){
        auto curStation = this->stations_hashmap.find(it->first)->second;
        if(first && std::next(it) != _travel_stations.end()){
          auto nextStation = this->stations_hashmap.find(std::next(it)->first)->second;
          if(curStation.line_id == nextStation.line_id){
            first = false;
            std::cout << "\t<Take line " << curStation.line_id << "> " << curStation.line_name << std::endl;
            std::cout << "\t\tFrom " << curStation.name;
            last_cost = it->second;
          }else{
            std::cout << "\tWalk to " << nextStation.name << ", line " << nextStation.line_id << " (" << std::next(it)->second << " secs)" << std::endl;
          }
        }else{
          auto prevStation = this->stations_hashmap.find(previous_node.first)->second;
          if(!first && prevStation.line_id != curStation.line_id){
            std::cout << " to " << prevStation.name << " (" << previous_node.second-last_cost << " secs)" << std::endl;
            last_cost = previous_node.second;
            std::cout << "\tWalk to " << curStation.name << ", line " << curStation.line_id << " (" << it->second << " secs)" << std::endl;
            last_cost = it->second;
            if(std::next(it) != _travel_stations.end()){
              auto nextStation = this->stations_hashmap.find(std::next(it)->first)->second;
              if(curStation.line_id == nextStation.line_id){
                std::cout << "\t<Take line " << curStation.line_id << "> "<< curStation.line_name << std::endl;
                std::cout << "\t\tFrom " << curStation.name;
              }else{
                std::cout << "\tWalk to " << curStation.name << ", line " << curStation.line_id << " (" << std::next(it)->second << " secs)" << std::endl;
              }
            }
          }else if(!first && std::next(it) == _travel_stations.end()){
            std::cout << " to " << curStation.name << " (" << it->second-last_cost << " secs)" << std::endl;
            last_cost = it->second;
          }
        }

        previous_node = *it;
      }
      std::cout << "After " << previous_node.second << " secs, you have reached your destination!" << std::endl;
    }
  };
}
