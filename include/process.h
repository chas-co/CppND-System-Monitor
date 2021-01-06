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
// Process constructor with the processed id passed to it
  Process(int pid);

  // Member Functions
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  // Declare any necessary private members
 private:
 int pid_;
 string user_;
 string command_;
 float cpuUtilization_;
 string ram_;
 long int upTime_;

 enum ProcessCPUStates {
    kUtime_ = 0,
    kStime_,
    kCutime_,
    kCstime_,
    kStarttime_
  };
};

#endif