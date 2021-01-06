#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//  Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
    // Create a vector to store the system process ids from the file system
   vector<int> procs_id = LinuxParser::Pids();
    vector<Process> sys_processes;
    processes_.clear();
    // Loop through the processes ids, create Process objects and store them in the system process container
    for (int p : procs_id)
    {
        Process proc(p);
        sys_processes.emplace_back(proc);
    }
    sort( sys_processes.begin(),  sys_processes.end());
    processes_ =  sys_processes;

    return processes_; 

}

//Return the system's kernel identifier (string)
std::string System::Kernel() { kernel_ = LinuxParser::Kernel(); return kernel_; }

//Return the system's memory utilization
float System::MemoryUtilization() { memoryUtilisation_ = LinuxParser::MemoryUtilization(); return memoryUtilisation_; }

//  Return the operating system name
std::string System::OperatingSystem() { operatingSystem_ = LinuxParser:: OperatingSystem(); return operatingSystem_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { runningProcesses_ = LinuxParser::RunningProcesses(); return runningProcesses_; }

// Return the total number of processes on the system
int System::TotalProcesses() { totalProcesses_ = LinuxParser::TotalProcesses(); return totalProcesses_; }

//  Return the number of seconds since the system started running
long int System::UpTime() { uptime_ = LinuxParser::UpTime(); return uptime_; }
