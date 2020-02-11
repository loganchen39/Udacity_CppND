#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl; 

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  
  for (unsigned int i = 0; i < pids.size(); i++) {
      // cout << "In LinuxParser::Pids(), returning pids: " << pids[i] << endl; 
  }
  
  
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  int memtotal, memfree;
  string line, dummy, var; 
  std::ifstream ifs(kProcDirectory + kMeminfoFilename);
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream linestream(line);
    linestream >> dummy >> var;
    memtotal = std::stoi(var);
    
    std::getline(ifs, line);
    linestream.str(line);
    linestream >> dummy >> var;
    memfree = std::stoi(var);
  }
  
  return (memtotal-memfree)*1.0 / memtotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line, var;
  long uptime {0};
  std::ifstream ifs(kProcDirectory + kUptimeFilename);
  
  // return 2000000; 
  
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream linestream(line);
    linestream >> var;
    // cout << "In Linux::Parser::UpTime(), var = " << var << endl; 
    uptime = std::stol(var);
    // cout << "In Linux::Parser::UpTime(), uptime = " << uptime << endl;
    // return uptime; 
  }
  
  return uptime; 
}


// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  vector<string> vec_str;
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream ifs(kProcDirectory + kStatFilename);
  
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    
    int i_idle    = std::stoi(idle) + std::stoi(iowait);
    int i_nonidle = std::stoi(user) + std::stoi(nice) + std::stoi(system) + std::stoi(irq) + std::stoi(softirq) + std::stoi(steal); 
    int i_total   = i_idle + i_nonidle; 
    float cpu_pct = 1.0*i_nonidle / i_total; 
    vec_str.push_back(cpu + " " + std::to_string(cpu_pct));
    
    return vec_str; 
  }
  
  return {}; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }



// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string line, key, var;
  int totalproc{0};
  std::ifstream ifs(kProcDirectory + kStatFilename);
  
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> var;
        totalproc = std::stoi(var);
        return totalproc;
      }
    }
  }
  
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  string line, key, var;
  int runningproc{0};
  std::ifstream ifs(kProcDirectory + kStatFilename);
  
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> var;
        runningproc = std::stoi(var);
        return runningproc;
      }
    }
  }
  
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Command(int pid[[maybe_unused]]) 
string LinuxParser::Command(int pid) 
{
  string line, str_pid;
  str_pid = std::to_string(pid);
  std::ifstream ifs(kProcDirectory + str_pid + "/cmdline");
  if (ifs.is_open()) {
    std::getline(ifs, line);
    return line;
  }
  
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  string line, str_pid, key, value;
  str_pid = std::to_string(pid);
  std::ifstream ifs(kProcDirectory + str_pid + "/status");
  
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key.substr(0, 6) == "VmSize") {
        linestream >> value;
        return std::to_string(std::stoi(value)/1000);
      }
    }
  }
  
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string line, str_pid, key, value;
  str_pid = std::to_string(pid);
  std::ifstream ifs(kProcDirectory + str_pid + "/status");
  
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key.substr(0, 3) == "Uid") {
        linestream >> value;
        return value;
      }
    }
  }  
  
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string uid = Uid(pid);
  
  string line, user, dummy, uid_tmp;
  std::ifstream ifs(kPasswordPath);
  
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dummy >> uid_tmp;
      if (uid_tmp == uid) {
        return user; 
      }
    }
  }
  
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line, str_pid, dummy;
  str_pid = std::to_string(pid);
  std::ifstream ifs(kProcDirectory + str_pid + "/stat");
  
  // return 1000000; 
  
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream linestream(line); 
    int i = 1;
    while (i <= 22) {
      linestream >> dummy; 
      i++;
    }
    return std::stol(dummy);
  }
  
  return 0; 
}

float LinuxParser::CpuUtilProc(int pid)
{
  string line, var;
  int uptime, utime, stime, cutime, cstime, starttime, hertz=100; 
  int totaltime, seconds; 
  float cpu_usage{0.0}; 
  
  uptime = int(LinuxParser::UpTime());
  std::ifstream ifs(kProcDirectory + std::to_string(pid) + "/stat");
  
  if (ifs.is_open()) {
    std::getline(ifs, line);
    std::istringstream linestream(line); 
    int i = 1;
    while (i <= 22) {
      linestream >> var; 
      switch(i) {
        case 14: 
          utime = std::stoi(var);
          ++i;
          break;
        case 15: 
          stime = std::stoi(var);
          ++i;
          break;
        case 16:
          cutime = std::stoi(var);
          ++i;
          break;
        case 17:
          cstime = std::stoi(var);
          ++i;
          break;
        case 22:
          starttime = std::stoi(var);
          ++i;
          break;
        default:
          ++i;
          break;
      }
    }
    
    totaltime = utime + stime + cutime + cstime;
    seconds = uptime - (starttime/hertz);  
    // cpu_usage = 100.0*((totaltime/hertz)/seconds);  // wrong, will be 0!
    cpu_usage = 100.0 * ((1.0*totaltime/hertz)/seconds);
    
    /**
    if (pid == 1) {
      cout << "In LinuxParser::CpuUtilProc(int pid), uptime = " << uptime << ", utime = " 
           << utime << ", stime = " << stime << ", cutime = " << cutime << ", cstime = " 
           << cstime << ", starttime = " << starttime << ", totaltime = " << totaltime 
           << ", seconds = " << seconds << ", cpu_usage = " << cpu_usage << endl; 
    }
    */
  }
  
  return cpu_usage; 
}