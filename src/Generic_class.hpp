#pragma once

#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <unordered_map>

#define INF std::numeric_limits<int>::max()

namespace travel{
  typedef struct __station{
    std::string name; // how is called the station
    unsigned int id; // identify the station with a unique number
    unsigned int line_id; // identify the line of the station
  }Station; // a station is a stop for the net represented by a "name", an "id" and a "line"

  typedef struct __connection{
    Station* stop; // reference station of the connection
    std::list<std::pair<Station*, int> > neighbors; // list of pair of station and static cost to join this station from the reference station
  }Connection; // a connection represent a "station" connected to a "list of other stations with a cost" to join it


  // finalement changer pour une struct ?
  typedef std::tuple<Station*, int, int> Node; // a node is a triplet formed by a "station" and the "dynamic cost" to join it from a "previous station"

  class Generic_class{

  public: // public methods
    // Constructor & destructor
    Generic_class(): stations(0), connections(0), graph(0), travel(std::pair<int,int>(-1,-1)){
    }

    Generic_class(int _start, int _end): stations(0), connections(0), graph(0), travel(std::pair<int,int>(_start, _end)){
    }

    virtual ~Generic_class(){
    }

    // Setters
    virtual void set_travel(int _start, int _end) = 0;
    virtual void set_start(int _start) = 0;
    virtual void set_end(int _end) = 0;
    virtual void read_stations(char* _filename) = 0;
    virtual void read_stations(std::string _filename) = 0;
    virtual void read_connections(char* _filename) = 0;
    virtual void read_connections(std::string _filename) = 0;

    // Display
    virtual void display_stations() = 0;
    virtual void display_connections() = 0;
    virtual void display_graph() = 0;
    virtual void display_travel() = 0;

    // Compute algorithms
    virtual void compute_travel() = 0;
    virtual void compute_travel(int _start, int _end) = 0;

  protected: // protected methods
    // compute the dijkstra algorithm
    virtual void dijkstra() = 0;

  protected: // protected variables
    std::list<Station> stations; // list of the known stations
    std::unordered_map<int, Station*> stations_hashmap; // hashmap able to search a station in the list
    std::list<Connection> connections; // list of the connections of each station
    std::unordered_map<int, Connection*> connections_hashmap;  // hashmap able to search a connection in the list
    std::list<Node> graph; // a list of stations that save the weight to arrive here starting from a unique station
    std::unordered_map<int, Node*> graph_hashmap; // hashmap able to search a station in the graph to build the best way from A to B
    std::pair<int,int> travel; // pair of a starting station id and a end station id
  };
};