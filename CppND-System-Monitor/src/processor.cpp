#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
  vector<string> cpu_util = LinuxParser::CpuUtilization();
  string cpu, cpu_pct; 
  if (cpu_util.size() > 0) {
    std::istringstream iss(cpu_util[0]);
    iss >> cpu >> cpu_pct; 
    
    return std::stof(cpu_pct); 
  }
  
  return 0.0; 
}