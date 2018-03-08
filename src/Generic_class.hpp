#pragma once

#include <list> // container list
#include <utility> // pair
#include <string> // string of char
#include <unordered_map> // hashmap

#include "tools.hpp"

#define INF std::numeric_limits<int>::max()

namespace travel{
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
    virtual void read_stations(const std::string& _filename, std::list<travel::Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap) = 0;
    virtual void read_stations(const char* _filename, std::list<travel::Station>* _stations, std::unordered_map<unsigned int, travel::Station*>* _stations_hashmap) = 0;
    virtual void read_connections(const std::string& _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap) = 0;
    virtual void read_connections(const char* _filename, const std::unordered_map<unsigned int, travel::Station*>& _stations_hashmap, std::list<travel::Connection>* _connections, std::unordered_map<unsigned int, travel::Connection*>* _connections_hashmap) = 0;

    /**********************************************
      Display
    */
    virtual void display_stations() = 0;
    virtual void display_connections() = 0;
    virtual void display_graph() = 0;
    virtual void display_travel() = 0;
    virtual void display_travel(unsigned int _end) = 0;

    // to evaluate students' objects
    virtual void display_travel(unsigned int _end, const std::unordered_map<unsigned int, travel::Node*>& _graph_hashmap) = 0;

    /**********************************************
      Compute algorithms
    */
    virtual void compute_travel() = 0;
    virtual void compute_travel(unsigned int _start) = 0;
    virtual void compute_and_display_travel() = 0;
    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end) = 0;

    // to evaluate students' objects
    virtual void compute_travel(std::list<travel::Connection>* _connections, std::list<travel::Node>* _graph, std::unordered_map<unsigned int, travel::Node*>* _graph_hashmap) = 0;
    virtual void compute_travel(unsigned int _start, std::list<travel::Connection>* _connections, std::list<travel::Node>* _graph, std::unordered_map<unsigned int, travel::Node*>* _graph_hashmap) = 0;
    virtual void compute_and_display_travel(std::list<travel::Connection>* _connections, std::list<travel::Node>* _graph, std::unordered_map<unsigned int, travel::Node*>* _graph_hashmap) = 0;
    virtual void compute_and_display_travel(unsigned int _start, unsigned int _end, std::list<travel::Connection>* _connections, std::list<travel::Node>* _graph, std::unordered_map<unsigned int, travel::Node*>* _graph_hashmap) = 0;

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
    std::list<travel::Station> stations; // list of the known stations
    std::unordered_map<unsigned int, travel::Station*> stations_hashmap; // hashmap able to search a station in the list
    std::list<travel::Connection> connections; // list of the connections of each station
    std::unordered_map<unsigned int, travel::Connection*> connections_hashmap;  // hashmap able to search a connection in the list
    std::list<travel::Node> graph; // a list of stations that save the weight to arrive here starting from a unique station
    std::unordered_map<unsigned int, travel::Node*> graph_hashmap; // hashmap able to search a station in the graph to build the best way from A to B
    std::pair<unsigned int,unsigned int> travel; // pair of a starting station id and a end station id
  };
};
