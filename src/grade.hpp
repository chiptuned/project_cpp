#pragma once

#include <random> // temporaire (random)
#include <string> // temporaire ("" conversion)
#include <functional> // temporaire (bind)
#include <chrono> // temporaire (compute execution time)

#include "Generic_class.hpp"
#include "Network.hpp" // temporaire (call something)

#define f1 "../sample_data/mar2_15h58_stations.csv"
#define f2 "../sample_data/mar2_15h58_connections.csv"

/******************************************************************************
   To evaluate overload operator<< of the 3 structs and the squeleton of network
*/
void grade_1(){
  travel::Station s1 = {"A", 1, "2"};
  travel::Station s2 = {"B", 3, "4"};
  travel::Station s3 = {"C", 5, "6"};
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

  std::cout << s1 << std::endl << s2 << std::endl << s3 << std::endl;
  std::cout << c1 << std::endl << c2 << std::endl << c3 << std::endl;
  std::cout << n1 << std::endl << n2 << std::endl << n3 << std::endl;
}

/******************************************************************************
   To evaluate Stations and display Stations
*/
void grade_2(travel::Generic_class* ptr){
  travel::Network network;

  std::cout << "Step 1" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());


  std::cout << "Step 2" << std::endl;
  network.read_connections(f2);

  std::cout << "Step 3" << std::endl;
  network.compute_travel(1722);

  std::cout << "Step 4" << std::endl;
  network.display_travel(2062);
}

/******************************************************************************
   To evaluate Connections and display Connections
*/
void grade_3(travel::Generic_class* ptr){
  travel::Network network;

  std::cout << "Step 1" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());

  network.read_connections(f2);
  std::cout << "Step 2" << std::endl;
  ptr->read_connections(f2, network.get_stations_hashmap(), &(network.get_connections()), &(network.get_connections_hashmap()));
  ptr->read_connections(std::string(f2), network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());

  std::cout << "Step 3" << std::endl;
  network.compute_travel(1722);

  std::cout << "Step 4" << std::endl;
  network.display_travel(2062);
}

/******************************************************************************
   To evaluate graph and display
*/
void grade_4(travel::Generic_class* ptr){
  travel::Network network;

  std::cout << "Step 1" << std::endl;
  ptr->read_stations(f1, &network.get_stations(), &network.get_stations_hashmap());
  ptr->read_stations(std::string(f1), &network.get_stations(), &network.get_stations_hashmap());

  std::cout << "Step 2" << std::endl;
  ptr->read_connections(f2, network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());
  ptr->read_connections(std::string(f2), network.get_stations_hashmap(), &network.get_connections(), &network.get_connections_hashmap());

  std::cout << "Step 3" << std::endl;
  ptr->compute_travel(1722, &network.get_connections(), &network.get_graph(), &network.get_graph_hashmap());

  std::cout << "Step 4" << std::endl;
  network.display_travel(2062);
}

/******************************************************************************
   To evaluate the full path with the election of the way
*/
void grade_max(travel::Generic_class* ptr){ // si on veut le rendre constant il faudra faire une constructeur de copie...
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1,10);

  // auto dice = std::bind ( distribution, generator );
  travel::Network network;
  network.read_stations(f1);
  network.read_connections(f2);
  ptr->read_stations(f1);
  ptr->read_connections(f2);

  int N = 2;//1e4;
  auto start = std::chrono::steady_clock::now();
  for(int i = 0; i < N; i++){
    auto start = 1722, end = 2062; //dice(), end = dice();
    ptr->compute_travel(start);
    ptr->display_travel(end);

    network.compute_travel(start);
    network.display_travel(end);
  }
  auto duration = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::steady_clock::now() - start);
  std::cout << "\n" << N/1000. << "K loops in " <<
    static_cast<double>(duration.count())/1e6 <<
    " sec\t(" << static_cast<double>(duration.count())/(static_cast<double>(N)*1000) <<
    " ms/loop)" << std::endl;
}
