#pragma once

#include <vector>
#include <utility>

#include "Generic_connection_parser.hpp"

namespace travel{
  struct Node{
    uint64_t id;
    uint64_t cost;
    uint64_t from_id;
  };

  class Generic_mapper: public Generic_connection_parser{
  public:
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end)=0;
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string& s1, const std::string& s2)=0;
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end)=0;
    virtual std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string& s1, const std::string& s2)=0;

  private:
    virtual uint64_t levenshtein_distance(const std::string& s1, const std::string& s2) = 0;
    virtual void display_travel(const std::vector<std::pair<uint64_t,uint64_t> >& _travel_stations)=0;
  };
}
