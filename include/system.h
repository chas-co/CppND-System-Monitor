#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes(); 
  float MemoryUtilization();         
  long UpTime();                     
  int TotalProcesses();              
  int RunningProcesses();            
  std::string Kernel();               
  std::string OperatingSystem();      
  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string kernel_;
  std::string operatingSystem_;
  float memoryUtilisation_;
  float uptime_;
  float totalProcesses_;
  float runningProcesses_;
  
};

#endif