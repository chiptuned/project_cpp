#pragma once

#include <iostream>

#include "Station_parser.hpp"

namespace travel{
  class Connection_parser: public travel::Station_parser{
  public:
    Connection_parser(const std::string& _stations_filename, const std::string& _connections_filename):
      Station_parser(_stations_filename){
      std::ifstream ifs(_connections_filename, std::ifstream::in);

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

          if(ls.size() != 3){
            std::stringstream ss;
            ss << "Connection_parser, field missmatch on line " << cpt << " " << ls.size() << " instead of 5";
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

    const std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> >& get_connections_hashmap() const{
      return this->connections_hashmap;
    }

    void display(){
      for(auto&& it: connections_hashmap){
        std::cout << stations_hashmap.find(it.first)->second << std::endl;
        for(auto&& it2: it.second){
          std::cout << "\t" << stations_hashmap.find(it2.first)->second << std::endl;
        }
      }
    }

  protected:
    std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> > connections_hashmap;
  };
}
