#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{
    const int secs_per_min = 60; 
    const int secs_per_hr = 3600;

    long hrs = seconds / secs_per_hr;
    int mins = (seconds % secs_per_hr) / secs_per_min;
    int secs = seconds % secs_per_min;

    string time =  TimeStringFormat(hrs) + ":" +  TimeStringFormat(mins) + ":" + TimeStringFormat(secs);

  return time;
}

// This function helps to format the time units 
string Format:: TimeStringFormat(const long time_value)
{
    // if the time is a single digit zero is appended to the from of the time otherwise the time value is retained
  if (time_value < 10)
    return "0" + std::to_string(time_value);
  else
    return std::to_string(time_value);
}