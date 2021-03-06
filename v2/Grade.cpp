#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <utility>

#include "Network.hpp"
#include "Grade.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  const std::string str1 = {
  #include "..\\sample_data\\s_hacked.csv"
  };
  const std::string str2 = {
  #include "..\\sample_data\\c_hacked.csv"
  };
  const std::string str3 = {
  #include "..\\sample_data\\stations_hacked.csv"
  };
  const std::string str4 = {
  #include "..\\sample_data\\connections_hacked.csv"
  };
#else
  const std::string str1 = {
  #include "../sample_data/s_hacked.csv"
  };
  const std::string str2 = {
  #include "../sample_data/c_hacked.csv"
  };
  const std::string str3 = {
  #include "../sample_data/stations_hacked.csv"
  };
  const std::string str4 = {
  #include "../sample_data/connections_hacked.csv"
  };
#endif

#define f1 "1s.csv"
#define f2 "1c.csv"
#define f3 "1stations.csv"
#define f4 "1connections.csv"

namespace travel{
  travel::Grade::Grade(bool _small){
    if(!_small){
      this->stations_filename=f1;
      this->connections_filename=f2;
      this->stations_literal=str1;
      this->connections_literal=str2;
      this->ids.push_back(1722);
      this->ids.push_back(2062);
      this->ids.push_back(2017);
    }else{
      this->stations_filename=f3;
      this->connections_filename=f4;
      this->stations_literal=str3;
      this->connections_literal=str4;
      this->ids.push_back(7);
      this->ids.push_back(1);
      this->ids.push_back(20);
    }
  }

  // FIXME(vincent) : Output le nombre de stations/connections lues
  // et correctes, afin de renseigner sur les possibles erreurs de
  // dépendances à la création des objets statiques dans Grade.hpp
  
  void travel::Grade::stations(const Generic_station_parser& _input){
    std::cout << "===========================> Grade 1 <===========================" << std::endl;
    travel::Network network(stations_filename, connections_filename, stations_literal, connections_literal);

    auto sta = _input.get_stations_hashmap();
    auto sta_ref = network.get_stations_hashmap();

    auto cpt_sta = 0;
    auto cpt_bad = 0;

    auto check_station = [&cpt_bad](const Station& _a, const Station& _b){
      if(_a.name.compare(_b.name) == 0){
        if(_a.line_id.compare(_b.line_id) == 0){
          if(_a.address.compare(_b.address) == 0){
            if(_a.line_name.compare(_b.line_name) == 0){
              return true;
            }
          }
        }
      }
      cpt_bad++;
      return false;
    };

    for(auto&& it: sta_ref){
      auto test = sta.find(it.first);
      if(test == sta.end()){
        cpt_sta++;
        cpt_bad++;
      }else{
        check_station(test->second, it.second);
      }
    }

    if(cpt_sta == 0 && cpt_bad == 0){
      std::cout << "Stations: seems ok" << std::endl;
    }else{
      std::stringstream ss;
      ss << "Stations failed: " << cpt_sta << "/" << sta_ref.size() << " unfound" << std::endl << "\t" << cpt_bad << " seem to be bad initialized" << std::flush;
      throw(ss.str());
    }
  }

  void travel::Grade::connections(const Generic_connection_parser& _input){
    this->stations(_input);
    std::cout << "===========================> Grade 2 <===========================" << std::endl;
    travel::Network network(stations_filename, connections_filename, stations_literal, connections_literal);

    auto co = _input.get_connections_hashmap();
    auto co_ref = network.get_connections_hashmap();

    auto cpt_sta = 0;
    auto cpt_co = 0;
    auto ref_co = 0;
    for(auto&& it: co_ref){
      ref_co+=it.second.size();
      auto test = co.find(it.first);
      if(test == co.end()){
        cpt_sta++;
        cpt_co+=it.second.size();
      }else{
        for(auto&& it2: it.second){
          auto test2 = test->second.find(it2.first);
          if(test2 == test->second.end()){
            cpt_co++;
          }
        }
      }
    }

    if(cpt_sta == 0 && cpt_co == 0){
      std::cout << "Connections: seems ok" << std::endl;
    }else{
      std::stringstream ss;
      ss << "Connections failed: " << cpt_sta << "/" << co_ref.size() << " stations unfound" << std::endl << "\t" << cpt_co << "/" << ref_co << " connections unfound" << std::flush;
      throw(ss.str());
    }
  }
  void travel::Grade::dijkstra(Generic_mapper& _input, bool _levenstein){
    this->connections(_input);
    if(!_levenstein){
      std::cout << "===========================> Grade 3 <===========================" << std::endl;
    }else{
      std::cout << "=========================> Grade 3 bis <=========================" << std::endl;
    }

    travel::Network network(stations_filename, connections_filename, stations_literal, connections_literal);

    std::cout << "First tests" << std::endl;

    auto cpt = 0;
    auto estimated_time = 0.0;
    for(auto&& it: this->ids){
      for(auto&& it2: this->ids){
        auto start_count = std::chrono::steady_clock::now();
        if(!_levenstein){
          auto ref = network.compute_travel(it,it2);
          auto vec = _input.compute_travel(it,it2);
          cpt = (this->areEqual(ref, vec) ? cpt : cpt+1);

          if(!this->areEqual(ref,vec)){
            std::cout << "*************************** Reference ***************************" << std::endl;
            network.compute_and_display_travel(it,it2);
            std::cout << "*************************** Your work ***************************" << std::endl;
            _input.compute_and_display_travel(it,it2);
          }
        }else{
          auto sta_ref = network.get_stations_hashmap();
          auto sta = network.get_stations_hashmap();

          auto ref = network.compute_travel(sta_ref.find(it)->second.name,sta_ref.find(it2)->second.name);
          auto vec = _input.compute_travel(sta.find(it)->second.name,sta.find(it2)->second.name);
          cpt = (this->areEqual(ref, vec) ? cpt : cpt+1);

          if(!this->areEqual(ref,vec)){
            std::cout << "*************************** Reference ***************************" << std::endl;
            network.compute_and_display_travel(sta_ref.find(it)->second.name,sta_ref.find(it2)->second.name);
            std::cout << "*************************** Your work ***************************" << std::endl;
            _input.compute_and_display_travel(sta.find(it)->second.name,sta.find(it2)->second.name);
          }
        }
        auto duration = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::steady_clock::now() - start_count);
        estimated_time += static_cast<double>(duration.count())/1e6;
      }
    }

    if(cpt != 0){
      std::stringstream ss;
      ss << "Dijkstra failed: " << cpt << "/" << this->ids.size()*this->ids.size() << " failed" << std::flush;
      throw(ss.str());
    }else{
      auto co_ref = network.get_connections_hashmap();
      estimated_time = 1.5*(co_ref.size()*co_ref.size())*estimated_time/(this->ids.size()*this->ids.size());
      std::cout << "Tests: seems ok.\nWould you try complete test (wait ~ " << this->convert_seconds(estimated_time).str() << ") ? (N/y): ";
      auto c = 'N';
      std::cin >> c;
      if(c == 'y'){
        auto start_count = std::chrono::steady_clock::now();
        auto count = 0;
        for(auto&& it: co_ref){

          for(auto&& it2: co_ref){
            count++;
            if(!_levenstein){
              auto ref = network.compute_travel(it.first,it2.first);
              auto vec = _input.compute_travel(it.first,it2.first);

              if(this->areEqual(ref, vec) == false){
                std::cout << "*************************** Reference ***************************" << std::endl;
                network.compute_and_display_travel(it.first,it2.first);
                std::cout << "*************************** Your work ***************************" << std::endl;
                _input.compute_and_display_travel(it.first,it2.first);

                std::stringstream ss;
                ss << "Test failed: from " << it.first << " to " << it2.first << " unsolved" << std::flush;
                throw(ss.str());
              }
            }else{
              auto sta_ref = network.get_stations_hashmap();
              auto sta = network.get_stations_hashmap();

              auto ref = network.compute_travel(sta_ref.find(it.first)->second.name,sta_ref.find(it2.first)->second.name);
              auto vec = _input.compute_travel(sta.find(it.first)->second.name,sta.find(it2.first)->second.name);

              if(this->areEqual(ref, vec) == false){
                std::cout << "*************************** Reference ***************************" << std::endl;
                network.compute_and_display_travel(sta_ref.find(it.first)->second.name,sta_ref.find(it2.first)->second.name);
                std::cout << "*************************** Your work ***************************" << std::endl;
                _input.compute_and_display_travel(sta.find(it.first)->second.name,sta.find(it2.first)->second.name);

                std::stringstream ss;
                ss << "Test failed: from " << it.first << " to " << it2.first << " unsolved" << std::flush;
                throw(ss.str());
              }
            }
          }
          auto duration = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::steady_clock::now() - start_count);
          std::cout << std::setw(3) << static_cast<uint64_t>(100.*count/(co_ref.size()*co_ref.size())) << "% " << this->convert_seconds(static_cast<double>(duration.count())/1e6).str() << " elapsed, wait " << this->convert_seconds((co_ref.size()*co_ref.size()-count)*(static_cast<double>(duration.count())/1e6)/count).str() << std::endl;
        }
        std::cout << "\nTests: seems ok" << std::endl;
      }
    }
  }

  bool travel::Grade::areEqual(const std::vector<std::pair<uint64_t,uint64_t> >& _ref, const std::vector<std::pair<uint64_t, uint64_t> >& _vec){
    if(_ref.size() != _vec.size()){
      return false;
    }else{
      for(auto it_ref = _ref.begin(), it_vec = _vec.begin(); it_ref != _ref.end(); ++it_ref, ++it_vec){
        if(*it_ref != *it_vec){
          return false;
        }
      }
    }
    return true;
  }

  template<typename T>
  std::stringstream travel::Grade::convert_seconds(T _secs){
    uint64_t hours = static_cast<uint64_t>(_secs/3600);
    uint64_t minutes = static_cast<uint64_t>((_secs - static_cast<double>(hours)*3600)/60);
    uint64_t seconds = static_cast<uint64_t>(_secs - hours*3600 - minutes*60);

    std::stringstream ss;
    if(hours != 0){
      ss << std::setw(2) << hours << " hours ";
    }
    if(minutes != 0){
      ss << std::setw(2) << minutes << " mins and ";
    }
    ss << std::setw(2) << seconds << " secs";
    return ss;
  }
}
