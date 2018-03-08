#include "tools.hpp"

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
    _os << "The station \"" << *(_connection.stop) << "\" is connected to:\n";
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
}
