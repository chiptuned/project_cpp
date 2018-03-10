#pragma once

#include <string>
#include <unordered_map>

namespace travel{
  struct Station{
    std::string name;
    std::string line_id;
    std::string address;
    std::string line_name;
    bool operator==(const Station& _in) const{
      if(this->name.compare(_in.name) == 0){
        if(this->line_id.compare(_in.line_id) == 0){
          if(this->address.compare(_in.address) == 0){
            if(this->line_name.compare(_in.line_name) == 0){
              return true;
            }
          }
        }
      }
      return false;
    }
    bool operator!=(const Station& _in) const{
      return !(*this == _in);
    }
    friend std::ostream& operator<<(std::ostream& _os, const Station& _stop){
      _os << "Station: " << _stop.name << " (line " << _stop.line_id;
      return _os;
    }
  };

  class Generic_station_parser{
  public:

    const std::unordered_map<uint64_t, Station>& get_stations_hashmap() const{
      return this->stations_hashmap;
    }

  protected:
    virtual void read_stations(const std::string& _filename)=0;

  protected:
    std::unordered_map<uint64_t, Station> stations_hashmap;
  };
}
