#pragma once

#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <unordered_map>

#define INF std::numeric_limits<int>::max()

namespace travel{
  /**********************************************
     UTILS
  */
  typedef struct __station{
    std::string name; // how is called the station
    unsigned int id; // identify the station with a unique number
    unsigned int line_id; // identify the line of the station
  }Station; // a station is a stop for the net represented by a "name", an "id" and a "line"

  typedef struct __connection{
    std::string name; // name of the connection according to the line
    Station* stop; // reference station of the connection
    std::list<std::pair<Station*, int> > neighbors; // list of pair of station and static cost to join this station from the reference station
  }Connection; // a connection represent a "station" connected to a "list of other stations with a cost" to join it


  // finalement changer pour une struct ?
  // typedef std::tuple<Station*, int, unsigned int> Node; // a node is a triplet formed by a "station" and the "dynamic cost" to join it from a "previous station"
  typedef struct __node{
    Connection* connection;
    int cost;
    unsigned int from_id;
  }Node;

  /**********************************************
     Generic_class class
  */
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
    virtual void read_stations(const std::string& _filename, std::list<Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap) = 0;
    virtual void read_stations(const char* _filename, std::list<Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap) = 0;
    virtual void read_connections(const std::string& _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap) = 0;
    virtual void read_connections(const char* _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap) = 0;

    /**********************************************
      Display
    */
    virtual void display_stations() = 0;
    virtual void display_connections() = 0;
    virtual void display_graph() = 0;
    virtual void display_travel() = 0;

    /**********************************************
      Compute algorithms
    */
    virtual void compute_travel() = 0;
    virtual void compute_travel(unsigned int _start, unsigned int _end) = 0;
    virtual void compute_and_display_travel() = 0;
    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end) = 0;

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
    std::list<Station> stations; // list of the known stations
    std::unordered_map<unsigned int, Station*> stations_hashmap; // hashmap able to search a station in the list
    std::list<Connection> connections; // list of the connections of each station
    std::unordered_map<unsigned int, Connection*> connections_hashmap;  // hashmap able to search a connection in the list
    std::list<Node> graph; // a list of stations that save the weight to arrive here starting from a unique station
    std::unordered_map<unsigned int, Node*> graph_hashmap; // hashmap able to search a station in the graph to build the best way from A to B
    std::pair<unsigned int,unsigned int> travel; // pair of a starting station id and a end station id
  };
};
