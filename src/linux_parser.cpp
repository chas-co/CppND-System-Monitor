#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() 
{
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
string LinuxParser::Kernel()
 {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() 
{
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
  return pids;
}

// Reads and returns the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string line;
  string key;
  float value;
  float Memfree;
  float MemTotal;
  float MemUtilised;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // Search for memtotal and its value
        if (key == "MemTotal") {
          MemTotal= value;
        }
        // search for memfree and its value
        else if(key=="MemFree"){
          Memfree=value;
          break;
        }
      }
    }
  }
  MemUtilised= (MemTotal-Memfree) / MemTotal;
  return MemUtilised;
  }

// Reads and returns the system uptime
long LinuxParser::UpTime() 
{ 
  string SysUpTime, IdleUpTime;
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> SysUpTime >> IdleUpTime;
    uptime=std::stol(SysUpTime);
  }
  return uptime;
}
//Reads and returns CPU utilization
vector<long> LinuxParser::CpuUtilization() 
{ 
  string line;
  string key;
  long user, nice, system, idle,iowait,irq,softirq, steal, guest, guest_nice;
  vector<long> CpuStats{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest>> guest_nice;
          CpuStats.push_back(user); 
          CpuStats.push_back(nice);
          CpuStats.push_back(system);
          CpuStats.push_back(idle);
          CpuStats.push_back(iowait);
          CpuStats.push_back(irq);
          CpuStats.push_back(softirq);
          CpuStats.push_back(steal);
          CpuStats.push_back(guest);
          CpuStats.push_back(guest_nice);
          return CpuStats;  
    }
  }
return CpuStats;
}

// Reads and returns the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  long jiffies = ActiveJiffies() + IdleJiffies();
  return jiffies; 
}

// Reads and returns the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) 
{ 
  string pidstr = to_string(pid);
  string value; 
  vector<string> jiffies{};
  long active_jiffies;
  string line, key;
  std::ifstream filstream(kProcDirectory + pidstr + kStatFilename);
  if (filstream.is_open())
  {
    while (getline(filstream,line))
    {
      std::istringstream linestream(line);
      for (int i = 1; i <= kStarttime; i++) 
      {
        linestream >> value;
        if (i == kUtime || i == kStime || i == kCutime || i == kCstime ) 
        {
          // read in clock ticks and convert to seconds by dividing by clock ticks per second
          active_jiffies += std::stof(value); 
        }
      }
    }
  
  }
  return active_jiffies; 
}

//Reads and returns the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  vector <long> cpustats = CpuUtilization();
  long active_jiffies = 
      cpustats[kUser_] + cpustats[kNice_]+ cpustats[kSystem_] + 
      cpustats[kIRQ_] + cpustats[kSoftIRQ_] + cpustats[kSteal_];
  return active_jiffies; 

}

// Reads and returns the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  vector<long> cpustats = CpuUtilization();
  long idle_jiffies = cpustats[kIdle_] + cpustats[kIOwait_];
  return idle_jiffies;
}

//Reads and returns the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line;
  string key;
  string value;
  int TotalProcs;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          TotalProcs= std::stoi(value);
          return TotalProcs;
        }
      }
    }
  } 
  return TotalProcs;
  }

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses()
 { 
  string line;
  string key;
  string value;
  int RunningProcs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          RunningProcs= std::stoi(value);
          return RunningProcs;
        }
      }
    }
  } 
  return RunningProcs;
  }

// Reads and returns the command associated with a process

string LinuxParser::Command(int pid) 
{ 
  string pidstr= to_string(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + pidstr + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line; 
    }
   return line; 
}

//Reads and returns the memory used by a process
long LinuxParser::Ram(int pid) 
{
  string pidstr= to_string(pid);
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + pidstr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  return value; 
}
// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string pidstr= to_string(pid);
  string line,key, value;
  std::ifstream filestream(kProcDirectory + pidstr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value;  
}

//Reads and returns the user associated with a process
string LinuxParser::User(int pid){
  string uid =LinuxParser::Uid(pid) ;
  string line,key,value, letter;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), ',', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> letter >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value;
}



// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  string pidstr= to_string(pid);
  string line;
  string value;
  long clockticks;
  std::ifstream filestream(kProcDirectory + pidstr + kStatFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line) ;
    std::istringstream linestream(line);
    for (int i=1; i<kStarttime+1; i++ )
    {
      linestream >> value;
    }
   
    clockticks= std::stoi(value);
  }
    
  long uptime= clockticks / sysconf(_SC_CLK_TCK);
  return uptime; 
}