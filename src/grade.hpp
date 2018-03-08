#pragma once

#include "Generic_class.hpp"

#define f1 "../sample_data/stations2.csv"
#define f2 "../sample_data/connections2.csv"

/******************************************************************************
   To evaluate overload operator<< of the 3 structs and the squeleton of network
*/
void grade_1();

/******************************************************************************
   To evaluate Stations and display Stations
*/
void grade_2(travel::Generic_class* ptr);

/******************************************************************************
   To evaluate Connections and display Connections
*/
void grade_3(travel::Generic_class* ptr);

/******************************************************************************
   To evaluate graph and display
*/
void grade_4(travel::Generic_class* ptr);

/******************************************************************************
   To evaluate the full path with the election of the way
*/
void grade_max(travel::Generic_class* ptr);
