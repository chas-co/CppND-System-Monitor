#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
 // Constructor
  Processor();

// Memeber Function
  double Utilization(); 

  // Declare any necessary private members
 private:
 // the previous total and idle cpu usage time  
  float prev_total;
  float prev_idle;
  
 // the Current total and idle time since boot
  float curr_total;
  float curr_idle;

};

#endif