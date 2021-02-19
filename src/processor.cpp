#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string.h>

using std::vector;
using std::string;

// Constructor Definition
Processor::Processor(): prev_total(0), prev_idle(0), curr_total(LinuxParser::Jiffies()), curr_idle(LinuxParser::IdleJiffies()){}

// Return the aggregate CPU utilization
double Processor::Utilization() 
{ 
    curr_total = LinuxParser:: Jiffies();
    curr_idle = LinuxParser::IdleJiffies();
    // compute the cpu utilisation 
    float total_diff = Processor::curr_total - Processor::prev_total;
    float idle_diff = Processor::curr_idle - Processor::prev_idle;
    float usage = (total_diff - idle_diff) / total_diff;

    // the current stats become the previous stats for the next check
    prev_total = curr_total;
    prev_idle = curr_idle;
   
    return usage;
}