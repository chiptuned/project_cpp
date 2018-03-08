#pragma once

#include <iostream>
#include <list>
#include <utility>

namespace travel{
  /**********************************************
     UTILS
  */
  typedef struct __station{
    std::string address; // to locate the station
    std::string name; // how is called the station
    unsigned int id; // identify the station with a unique number
    std::string line_name; // verbose name of the line with its direction
    std::string line_id; // identify the line of the station
  }Station; // a station is a stop for the net represented by a "name", an "id" and a "line"

  typedef struct __connection{
    std::string name; // name of the connection according to the line
    travel::Station* stop; // reference station of the connection
    std::list<std::pair<travel::Station*, int> > neighbors; // list of pair of station and static cost to join this station from the reference station
  }Connection; // a connection represent a "station" connected to a "list of other stations with a cost" to join it

  typedef struct __node{
    travel::Connection* connection; // the connection of the node
    unsigned long cost; // the cumulative cost of this node
    unsigned int from_id; // id of the station that generated this cost
  }Node; // a node represent a connection with a cost to join it from a dedicated station

  std::ostream& operator<<(std::ostream& _os, travel::Station& _stop);
  std::ostream& operator<<(std::ostream& _os, travel::Connection& _connection);
  std::ostream& operator<<(std::ostream& _os, const travel::Node& _node);
}
