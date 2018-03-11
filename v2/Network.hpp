#pragma once

#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <map>

#include "Generic_mapper.hpp"

#define INF std::numeric_limits<uint64_t>::max()

namespace travel{
  class Network: public Generic_mapper{
  public:
    Network(const std::string& _stations_filename, const std::string& _connections_filename){
      this->read_stations(_stations_filename);
      this->read_connections(_connections_filename);
    }

    virtual std::vector<std::pair<uint64_t,uint64_t>> compute_travel(uint64_t _start, uint64_t _end) override{
      if(this->connections_hashmap.find(_start) == this->connections_hashmap.end()){
        throw("Starting id unknown");
      }else if(this->connections_hashmap.find(_end) == this->connections_hashmap.end()){
          throw("Ending id unknown");
      }

      std::vector<std::pair<uint64_t,uint64_t>> travel_stations;
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

    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string& s1, const std::string& s2) override{
      std::map<std::pair<uint64_t,uint64_t>, uint64_t> start, end;
      for(auto&& it: this->connections_hashmap){
        start.insert(std::pair<std::pair<uint64_t,uint64_t>, uint64_t>(std::pair<uint64_t, uint64_t>(levenshtein_distance(s1,this->stations_hashmap.find(it.first)->second.name),it.first),it.first));
        end.insert(std::pair<std::pair<uint64_t,uint64_t>, uint64_t>(std::pair<uint64_t, uint64_t>(levenshtein_distance(s2,this->stations_hashmap.find(it.first)->second.name),it.first),it.first));
      }
      return this->compute_travel(start.begin()->second, end.begin()->second);
    }

    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string& s1, const std::string& s2) override{
      auto travel_stations = this->compute_travel(s1,s2);
      this->display_travel(travel_stations);
      return travel_stations;
    }

    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override{
      auto travel_stations = this->compute_travel(_start, _end);
      this->display_travel(travel_stations);
      return travel_stations;
    }

  protected:
    //pompé pimpé: https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
    uint64_t levenshtein_distance(const std::string& s1, const std::string& s2) override{
      std::string s12(s1), s22(s2);

      auto up_case = [](char c){
        return ((c >= 'a' && c <= 'z') ? c-'a'+'A' : c);
      };
      std::for_each(s12.begin(), s12.end(), up_case);
      std::for_each(s22.begin(), s22.end(), up_case);

      const std::size_t len1 = s12.size(), len2 = s22.size();
      std::vector<uint64_t> col(len2+1), prev_col(len2+1);

      for (uint64_t i = 0; i < prev_col.size(); i++){
        prev_col[i] = i;
      }
      for (uint64_t i = 0; i < len1; i++) {
        col[0] = i+1;
        for (uint64_t j = 0; j < len2; j++){
          // note that std::min({arg1, arg2, arg3}) works only in C++11,
          // for C++98 use std::min(std::min(arg1, arg2), arg3)
          col[j+1] = std::min({ prev_col[1 + j] + 1, col[j] + 1, prev_col[j] + (s12[i]==s22[j] ? 0 : 1) });
        }
        col.swap(prev_col);
      }
      return prev_col[len2];
    }

    virtual void display_travel(const std::vector<std::pair<uint64_t,uint64_t> >& _travel_stations) override{
      {
        auto start = this->stations_hashmap.find(_travel_stations.front().first);
        auto end = this->stations_hashmap.find(_travel_stations.back().first);
        if(start == this->stations_hashmap.end() || end == this->stations_hashmap.end()){
          throw("failed in the travel building");
        }
        std::cout << "\nBest way from " << start->second.name << " (line " << start->second.line_id << ") to " << end->second.name << " (line " << end->second.line_id << ") is:\n";
      }

      std::pair<uint64_t,uint64_t> previous_node = *(_travel_stations.begin());
      uint64_t last_cost = 0;
      bool first = true;

      if(_travel_stations.size() > 1){
        for(auto it = _travel_stations.begin(); it != _travel_stations.end(); ++it){
          auto curStation = this->stations_hashmap.find(it->first)->second;
          if(first && std::next(it) != _travel_stations.end()){
            auto nextStation = this->stations_hashmap.find(std::next(it)->first)->second;
            if(curStation.line_id == nextStation.line_id && curStation.line_name.compare(curStation.line_name.size()-1-10, 10, nextStation.line_name.substr(nextStation.line_name.size()-1-10, 10)) == 0){
              first = false;
              std::cout << "\t<Take line " << curStation.line_id << "> " << curStation.line_name << std::endl;
              std::cout << "\t\tFrom " << curStation.name;
              last_cost = it->second;
            }else if(curStation.line_id != nextStation.line_id){
              std::cout << "\tWalk to " << nextStation.name << ", line " << nextStation.line_id << " (" << std::next(it)->second-last_cost << " secs)" << std::endl;
              last_cost = std::next(it)->second;
            }
          }else{
            auto prevStation = this->stations_hashmap.find(previous_node.first)->second;
            if(!first && prevStation.line_id != curStation.line_id){
              std::cout << " to " << prevStation.name << " (" << previous_node.second-last_cost << " secs)" << std::endl;
              last_cost = previous_node.second;
              std::cout << "\tWalk to " << curStation.name << ", line " << curStation.line_id << " (" << it->second-last_cost << " secs)" << std::endl;
              last_cost = it->second;
              if(std::next(it) != _travel_stations.end()){
                auto nextStation = this->stations_hashmap.find(std::next(it)->first)->second;
                if(curStation.line_id == nextStation.line_id){
                  std::cout << "\t<Take line " << curStation.line_id << "> "<< curStation.line_name << std::endl;
                  std::cout << "\t\tFrom " << curStation.name;
                }else{
                  std::cout << "\tWalk to " << curStation.name << ", line " << curStation.line_id << " (" << std::next(it)->second-last_cost << " secs)" << std::endl;
                }
              }
            }else if(!first && std::next(it) == _travel_stations.end()){
              std::cout << " to " << curStation.name << " (" << it->second-last_cost << " secs)" << std::endl;
              last_cost = it->second;
            }
          }

          previous_node = *it;
        }
      }
      std::cout << "After " << previous_node.second << " secs, you have reached your destination!" << std::endl;
    }

    void read_stations(const std::string& _filename) override{
      std::ifstream ifs(_filename, std::ifstream::in);

      if(!ifs.good()){
        throw("Unvalid stations' file");
      }else{
        char header_line[1024];
        ifs.getline(header_line,1024,'\n');
      }

      uint64_t cpt = 0;
      while(ifs.good() && !ifs.eof()){
        char line[1024];
        ifs.getline(line, 1024, '\n');
        std::string str(line);
        if(str.compare("") != 0){
          std::vector<std::string> ls;
          std::string::size_type cur1 = 0;
          auto cur2 = str.find(",",cur1);
          while(cur2!=std::string::npos){
            ls.push_back(str.substr(cur1, cur2-cur1));
            cur1 = cur2+1;
            cur2 = str.find(",", cur1);
          }
          ls.push_back(str.substr(cur1, str.size()-cur1));

          if(ls.size() != 5){
            std::stringstream ss;
            ss << "Station_parser, field missmatch on line " << cpt << " " << ls.size() << " instead of 5";
            throw(ss.str());
          }

          Station new_station;
          new_station.name = ls.at(0);
          auto id = static_cast<uint64_t>(std::stoll(ls.at(1)));
          new_station.line_id = ls.at(2);
          new_station.address = ls.at(3);
          new_station.line_name = ls.at(4);

          this->stations_hashmap.insert(std::pair<uint64_t, Station>(id, new_station));
          cpt++;
        }
      }
      ifs.close();
    }

    void read_connections(const std::string& _filename) override{
      std::ifstream ifs(_filename, std::ifstream::in);

      if(!ifs.good()){
        throw("Unvalid stations' file");
      }else{
        char header_line[1024];
        ifs.getline(header_line,1024,'\n');
      }

      uint64_t cpt = 0;
      while(ifs.good() && !ifs.eof()){
        char line[1024];
        ifs.getline(line, 1024, '\n');
        std::string str(line);
        if(str.compare("") != 0){
          std::vector<std::string> ls;
          std::string::size_type cur1 = 0;
          auto cur2 = str.find(",",cur1);
          while(cur2!=std::string::npos){
            ls.push_back(str.substr(cur1, cur2-cur1));
            cur1 = cur2+1;
            cur2 = str.find(",", cur1);
          }
          ls.push_back(str.substr(cur1, str.size()-cur1));

          if(ls.size() < 3){
            std::stringstream ss;
            ss << "Connection_parser, field missmatch on line " << cpt << " " << ls.size() << " instead of 3";
            throw(ss.str());
          }

          auto first = this->stations_hashmap.find(static_cast<uint64_t>(std::stoll(ls.at(0))));
          auto second = this->stations_hashmap.find(static_cast<uint64_t>(std::stoll(ls.at(1))));

          if(!(first == this->stations_hashmap.end() || second == this->stations_hashmap.end())){
            auto known_connection = this->connections_hashmap.find(first->first);

            if(known_connection == this->connections_hashmap.end()){
              this->connections_hashmap.insert(std::pair<uint64_t, std::unordered_map<uint64_t,uint64_t> >(first->first, std::unordered_map<uint64_t,uint64_t>({{second->first, static_cast<uint64_t>(stoll(ls.at(2)))}})));
            }else{
              auto check_connection = known_connection->second.find(second->first);
              if(check_connection == known_connection->second.end()){
                known_connection->second.insert(std::pair<uint64_t,uint64_t>(second->first, static_cast<uint64_t>(stoll(ls.at(2)))));
              }
            }
          }
        }
      }

      ifs.close();
    }
  };
}
