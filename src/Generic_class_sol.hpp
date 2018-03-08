#pragma once

#include <list> // container list
#include <utility> // pair
#include <string> // string of char
#include <unordered_map> // hashmap

#include "tools.hpp"

#define INF std::numeric_limits<int>::max()

namespace sol{
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
    sol::Station* stop; // reference station of the connection
    std::list<std::pair<sol::Station*, int> > neighbors; // list of pair of station and static cost to join this station from the reference station
  }Connection; // a connection represent a "station" connected to a "list of other stations with a cost" to join it

  typedef struct __node{
    sol::Connection* connection; // the connection of the node
    unsigned long cost; // the cumulative cost of this node
    unsigned int from_id; // id of the station that generated this cost
  }Node; // a node represent a connection with a cost to join it from a dedicated station

  /**********************************************
     Generic_class class
  */
  template<typename TS, /* Station Type  */
           typename TC, /* Connection Type  */
           typename TN  /* Node Type  */
           >
  class Generic_class{

    /**********************************************
      Public methods
    */
  public:
    /**********************************************
      Constructor & destructor
    */
    Generic_class(): stations(0), connections(0), graph(0), travel(std::pair<int,int>(-1,-1)){
    }

    Generic_class(int _start, int _end): stations(0), connections(0), graph(0), travel(std::pair<int,int>(_start, _end)){
    }

    virtual ~Generic_class(){
    }

    /**********************************************
      Setters
    */
    virtual void set_travel(unsigned int _start, unsigned int _end) = 0;
    virtual void set_travel(std::pair<unsigned int, unsigned int> _travel) = 0;
    virtual void set_start(unsigned int _start) = 0;
    virtual void set_end(unsigned int _end) = 0;
    virtual void read_stations(const char* _filename) = 0;
    virtual void read_stations(const std::string& _filename) = 0;
    virtual void read_connections(const char* _filename) = 0;
    virtual void read_connections(const std::string& _filename) = 0;

    // to evaluate students' objects
    virtual void read_stations(const std::string& _filename, std::list<TS>* _stations, std::unordered_map<unsigned int, TS*>* _stations_hashmap) = 0;
    virtual void read_stations(const char* _filename, std::list<TS>* _stations, std::unordered_map<unsigned int, TS*>* _stations_hashmap) = 0;
    virtual void read_connections(const std::string& _filename, const std::unordered_map<unsigned int, TS*>& _stations_hashmap, std::list<TC>* _connections, std::unordered_map<unsigned int, TC*>* _connections_hashmap) = 0;
    virtual void read_connections(const char* _filename, const std::unordered_map<unsigned int, TS*>& _stations_hashmap, std::list<TC>* _connections, std::unordered_map<unsigned int, TC*>* _connections_hashmap) = 0;

    /**********************************************
      Display
    */
    virtual void display_stations() = 0;
    virtual void display_connections() = 0;
    virtual void display_graph() = 0;
    virtual void display_travel() = 0;
    virtual void display_travel(unsigned int _end) = 0;

    // to evaluate students' objects
    virtual void display_travel(unsigned int _end, const std::unordered_map<unsigned int, TN*>& _graph_hashmap) = 0;

    /**********************************************
      Compute algorithms
    */
    virtual void compute_travel() = 0;
    virtual void compute_travel(unsigned int _start) = 0;
    virtual void compute_and_display_travel() = 0;
    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end) = 0;

    // to evaluate students' objects
    virtual void compute_travel(std::list<TC>* _connections, std::list<TN>* _graph, std::unordered_map<unsigned int, TN*>* _graph_hashmap) = 0;
    virtual void compute_travel(unsigned int _start, std::list<TC>* _connections, std::list<TN>* _graph, std::unordered_map<unsigned int, TN*>* _graph_hashmap) = 0;
    virtual void compute_and_display_travel(std::list<TC>* _connections, std::list<TN>* _graph, std::unordered_map<unsigned int, TN*>* _graph_hashmap) = 0;
    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end, std::list<TC>* _connections, std::list<TN>* _graph, std::unordered_map<unsigned int, TN*>* _graph_hashmap) = 0;

    /**********************************************
      Protected methods
    */
  protected:

    /**********************************************
      Public variables
    */
  public:

    /**********************************************
      Protected variables
    */
  protected:
    std::list<TS> stations; // list of the known stations
    std::unordered_map<unsigned int, TS*> stations_hashmap; // hashmap able to search a station in the list
    std::list<TC> connections; // list of the connections of each station
    std::unordered_map<unsigned int, TC*> connections_hashmap;  // hashmap able to search a connection in the list
    std::list<TN> graph; // a list of stations that save the weight to arrive here starting from a unique station
    std::unordered_map<unsigned int, TN*> graph_hashmap; // hashmap able to search a station in the graph to build the best way from A to B
    std::pair<unsigned int,unsigned int> travel; // pair of a starting station id and a end station id
  };
};
