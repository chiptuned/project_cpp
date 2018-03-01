#pragma once

#include <random> // temporaire (random)
#include <string> // temporaire ("" conversion)
#include <functional> // temporaire (bind)
#include <chrono> // temporaire (compute execution time)

#include "Generic_class.hpp"
#include "Network.hpp" // temporaire (call something)

/******************************************************************************
   To evaluate overload operator<< of the 3 structs and the squeleton of network
 */
// void grade_1(const Generic_class* ptr);

/******************************************************************************
   To evaluate Stations and display Stations
 */
// void grade_2(const Generic_class* ptr);

/******************************************************************************
   To evaluate Connections and display Connections
 */
// void grade_3(const Generic_class* ptr);

/******************************************************************************
   To evaluate graph and display
 */
// void grade_4(const Generic_class* ptr);

/******************************************************************************
   To evaluate the full path with the election of the way
 */
void grade_max(travel::Generic_class* ptr){ // si on veut le rendre constant il faudra faire une constructeur de copie...
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1,10);

  auto dice = std::bind ( distribution, generator );
  ptr->display_stations();
  ptr->display_connections();

  int N = 1e6;
  auto start = std::chrono::steady_clock::now();
  for(int i = 0; i < N; i++){
    auto start = dice(), end = dice();
    ptr->compute_travel(start, end);
  }
  auto duration = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::steady_clock::now() - start);
  std::cout << "\n" << N/1000. << "K loops in " <<
    static_cast<double>(duration.count())/1e6 <<
    " sec\t(" << static_cast<double>(duration.count())/(static_cast<double>(1e6)*1000) <<
    " ms/loop)" << std::endl;

}
