#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;
using std::ostringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) 
{ 
  long hh = long(seconds/3600), mm = (seconds - hh*3600)/60, ss = seconds - hh*3600 - mm*60; 
  string str_res;
  ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hh << ":" << std::setw(2) << std::setfill('0') << mm << ":" << std::setw(2) << std::setfill('0') << ss;
  
  return oss.str();
}