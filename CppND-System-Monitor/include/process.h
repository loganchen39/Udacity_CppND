#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string; 

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  int Pid() const;                         // TODO: See src/process.cpp
  string User();                           // TODO: See src/process.cpp
  string Command();                        // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  string Ram();                            // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  // bool operator<(Process& a) const;  // TODO: See src/process.cpp
  
  // Added by Ligang:
  Process(int pid, string user, string command) : pid_(pid), user_(user), command_(command) {}

// TODO: Declare any necessary private members
private:
  int pid_ = -1; 
  string user_ = ""; 
  string command_ = "";
};

#endif