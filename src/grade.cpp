#include <random> // (random)
#include <string> // ("" conversion)
#include <functional> // (bind)
#include <chrono> // (compute execution time)
#include <cstdlib>

#include "grade.hpp"
#include "Network_sol.hpp" // temporaire (call something)

/******************************************************************************
   To evaluate overload operator<< of the 3 structs and the squeleton of network
*/
void grade_1(){
  {
    travel::Station s1 = {"addr", "A", 1, "lname", "2"};
    travel::Station s2 = {"addr", "B", 3, "lname", "4"};
    travel::Station s3 = {"addr", "C", 5, "lname", "6"};
    travel::Connection c1, c2, c3;
    travel::Node n1 = {&c1, 1, 1};
    travel::Node n2 = {&c2, 10, 3};
    travel::Node n3 = {&c3, 100, 5};
    c1.stop = &s1;
    c1.neighbors.push_back(std::pair<travel::Station*, int>(&s2,1));
    c1.neighbors.push_back(std::pair<travel::Station*, int>(&s3,2));

    c2.stop = &s2;
    c2.neighbors.push_back(std::pair<travel::Station*, int>(&s1,3));
    c2.neighbors.push_back(std::pair<travel::Station*, int>(&s3,4));

    c3.stop = &s3;
    c3.neighbors.push_back(std::pair<travel::Station*, int>(&s1,5));
    c3.neighbors.push_back(std::pair<travel::Station*, int>(&s2,6));

    std::cout << "************" << " YOUR RESULTS " << "************" << std::endl;
    std::cout << s1 << std::endl << s2 << std::endl << s3 << std::endl;
    std::cout << c1 << std::endl << c2 << std::endl << c3 << std::endl;
    std::cout << n1 << std::endl << n2 << std::endl << n3 << std::endl;
  }
  {
    sol::Station s1 = {"addr", "A", 1, "lname", "2"};
    sol::Station s2 = {"addr", "B", 3, "lname", "4"};
    sol::Station s3 = {"addr", "C", 5, "lname", "6"};
    sol::Connection c1, c2, c3;
    sol::Node n1 = {&c1, 1, 1};
    sol::Node n2 = {&c2, 10, 3};
    sol::Node n3 = {&c3, 100, 5};
    c1.stop = &s1;
    c1.neighbors.push_back(std::pair<sol::Station*, int>(&s2,1));
    c1.neighbors.push_back(std::pair<sol::Station*, int>(&s3,2));

    c2.stop = &s2;
    c2.neighbors.push_back(std::pair<sol::Station*, int>(&s1,3));
    c2.neighbors.push_back(std::pair<sol::Station*, int>(&s3,4));

    c3.stop = &s3;
    c3.neighbors.push_back(std::pair<sol::Station*, int>(&s1,5));
    c3.neighbors.push_back(std::pair<sol::Station*, int>(&s2,6));

    std::cout << "\n************" << " EXPECTED RESULTS " << "************" << std::endl;
    std::cout << s1 << std::endl << s2 << std::endl << s3 << std::endl;
    std::cout << c1 << std::endl << c2 << std::endl << c3 << std::endl;
    std::cout << n1 << std::endl << n2 << std::endl << n3 << std::endl;
  }
}

/******************************************************************************
   To evaluate Stations and display Stations
*/
void grade_2(travel::Generic_class* ptr){
  sol::Network<travel::Station,travel::Connection,travel::Node> network;

  std::cout << "************" << " YOUR RESULTS " << "************" << std::endl;
  std::cout << "Step 1: Stations" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());

  std::cout << "Step 2: Connections" << std::endl;
  network.read_connections(f2);
  network.set_random_generator();

  std::cout << "Step 3: Graphe" << std::endl;
  auto start = network.get_random_station(), end = network.get_random_station();
  std::cout << start << " " << end << std::endl;
  network.compute_travel(start);

  std::cout << "Step 4: Travel" << std::endl;
  network.display_travel(end);

  std::cout << "\n************" << " EXPECTED RESULTS " << "************" << std::endl;
  sol::Network<sol::Station,sol::Connection,sol::Node> network2;
  network2.read_stations(f1);
  network2.read_connections(f2);
  network2.compute_travel(start);
  network2.display_travel(end);
}

/******************************************************************************
   To evaluate Connections and display Connections
*/
void grade_3(travel::Generic_class* ptr){
  sol::Network<travel::Station,travel::Connection,travel::Node> network;

  std::cout << "************" << " YOUR RESULTS " << "************" << std::endl;
  std::cout << "Step 1: Stations" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());

  std::cout << "Step 2: Connections" << std::endl;
  ptr->read_connections(f2, network.get_stations_hashmap(), &(network.get_connections()), &(network.get_connections_hashmap()));
  ptr->read_connections(std::string(f2), network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());
  network.set_random_generator();

  std::cout << "Step 3: Graphe" << std::endl;
  auto start = network.get_random_station(), end = network.get_random_station();
  std::cout << start << " " << end << std::endl;
  network.compute_travel(start);

  std::cout << "Step 4: Travel" << std::endl;
  network.display_travel(end);

  std::cout << "\n************" << " EXPECTED RESULTS " << "************" << std::endl;
  sol::Network<sol::Station,sol::Connection,sol::Node> network2;
  network2.read_stations(f1);
  network2.read_connections(f2);
  network2.compute_travel(start);
  network2.display_travel(end);
}

/******************************************************************************
   To evaluate graph and display
*/
void grade_4(travel::Generic_class* ptr){
  sol::Network<travel::Station,travel::Connection,travel::Node> network;

  std::cout << "************" << " YOUR RESULTS " << "************" << std::endl;
  std::cout << "Step 1: Stations" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());

  std::cout << "Step 2: Connections" << std::endl;
  ptr->read_connections(f2, network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());
  ptr->read_connections(std::string(f2), network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());
  network.set_random_generator();

  std::cout << "Step 3: Graphe" << std::endl;
  auto start = network.get_random_station(), end = network.get_random_station();
  std::cout << start << " " << end << std::endl;
  ptr->compute_travel(start, &network.get_connections(), &network.get_graph(), &network.get_graph_hashmap());

  std::cout << "Step 4: Travel" << std::endl;
  network.display_travel(end);

  std::cout << "\n************" << " EXPECTED RESULTS " << "************" << std::endl;
  sol::Network<sol::Station,sol::Connection,sol::Node> network2;
  network2.read_stations(f1);
  network2.read_connections(f2);
  network2.compute_travel(start);
  network2.display_travel(end);
}

/******************************************************************************
   To evaluate the full path with the election of the way
*/
void grade_max(travel::Generic_class* ptr){
  sol::Network<sol::Station,sol::Connection,sol::Node> network;

  network.read_stations(f1);
  network.read_connections(f2);
  network.set_random_generator();
  ptr->read_stations(f1);
  ptr->read_connections(f2);

  int N = 2;//1e4;
  auto start = std::chrono::steady_clock::now();
  for(int i = 0; i < N; i++){
    auto start = network.get_random_station(), end = network.get_random_station(); //dice(), end = dice();
    std::cout << start << " " << end << std::endl;
    std::cout << std::endl << "************" << " YOUR RESULTS " << "************" << std::endl;
    ptr->compute_travel(start);
    ptr->display_travel(end);

    std::cout << "\n************" << " EXPECTED RESULTS " << "************" << std::endl << std::endl;
    network.compute_travel(start);
    network.display_travel(end);
  }
  auto duration = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::steady_clock::now() - start);
  std::cout << "\n" << N/1000. << "K loops in " <<
    static_cast<double>(duration.count())/1e6 <<
    " sec\t(" << static_cast<double>(duration.count())/(static_cast<double>(N)*1000) <<
    " ms/loop)" << std::endl;
}