#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "Generic_mapper.hpp"

namespace travel{
  class Grade{
  public:
    Grade(bool _small); // true: small database, false: ratp database
    void stations(const Generic_station_parser& _input);
    void connections(const Generic_connection_parser& _input);
    void dijkstra(Generic_mapper& _input);
    // void levenstein(const Generic_mapper&);
  private:
    template<typename T>
    std::stringstream convert_seconds(T _secs);
    bool areEqual(const std::vector<std::pair<uint64_t,uint64_t> >& _ref, const std::vector<std::pair<uint64_t, uint64_t> >& _vec);
    std::string stations_filename;
    std::string connections_filename;
    std::vector<uint64_t> ids;
  };

  static travel::Grade evaluate_small(true);
  static travel::Grade evaluate(false);
}
