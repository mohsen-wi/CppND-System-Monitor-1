#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:

unsigned long long int PrevIdle, PrevActive, Idle, Active, PrevTotal, Total, Totald, Idled;
float utilization;
};

#endif