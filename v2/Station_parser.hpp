#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <fstream>
#include <cstring>

namespace travel{
  struct Station{
    std::string name;
    std::string line_id;
    std::string address;
    std::string line_name;
  };

  std::ostream& operator<<(std::ostream& _os, Station& _stop){
    _os << "Station: " << _stop.name << " (Line: " << _stop.line_id << ")";
    return _os;
  }

  class Station_parser{
  public:
    Station_parser(const std::string& _filename){
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

    const std::unordered_map<uint64_t, Station>& get_stations_hashmap() const{
      return this->stations_hashmap;
    }

  protected:
    std::unordered_map<uint64_t, Station> stations_hashmap;
  };
}
