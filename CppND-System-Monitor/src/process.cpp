#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>


#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector; 
// using LinuxParser; 

// TODO: Return this process's ID
int Process::Pid() const
{ 
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  return LinuxParser::CpuUtilProc(pid_); 
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
  return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
  return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
  return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
  return LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a[[maybe_unused]]) const 
// bool Process::operator<(Process const& a) const 
bool Process::operator<(Process const& a) const
{
  int apid = a.Pid();  //The Pid() function has to be const, or compile error for this function!
  return pid_ < apid ? true : false; 
  
  // string thisuser = User(), auser = a.User();
  // return thisuser < auser ? true : false;
  
  // return CpuUtilization() < a.CpuUtilization() ? true : false;
  // return this->User() < a.User() ? true : false; 
  // return true; 
}