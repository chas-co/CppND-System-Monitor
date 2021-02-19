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

// Return this process's ID
Process::Process(int pid ): pid_(pid)
{ 
    Process::CpuUtilization();
    Process::Command();
    Process::Ram();
    Process::User();
    Process::UpTime();
}

int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    float total_time = LinuxParser::ActiveJiffies(Pid()) / sysconf(_SC_CLK_TCK);
    float seconds = LinuxParser::UpTime() -( LinuxParser::UpTime()- LinuxParser::UpTime(Pid()));
    cpuUtilization_= total_time / seconds; 

    return cpuUtilization_; 
}

// Return the command that generated this process
string Process::Command() { command_ = LinuxParser::Command(Pid()); return command_; }

// Return this process's memory utilization
string Process::Ram() 
{ 
    long ram_kb = LinuxParser::Ram(Pid()); 
    try
    {
        // convert from kilobyte to mega byte
        long ram_mb = ram_kb / 1000; 
        ram_= to_string(ram_mb);
    }
    catch(const std::invalid_argument& arg)
    {
        ram_ = "0" ;
    }
    
    return ram_; 
}

// Return the user (name) that generated this process
string Process::User() { user_ = LinuxParser::User(Pid()); return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { upTime_ = LinuxParser::UpTime(Pid()); return upTime_; }

// Overload the "less than" comparison operator for Process objects

bool Process::operator < (Process const& a) const 
{
    return (a.cpuUtilization_ < cpuUtilization_) ;
}