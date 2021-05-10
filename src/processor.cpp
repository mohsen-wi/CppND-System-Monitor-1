#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

PrevIdle = LinuxParser::IdleJiffies();
PrevActive = LinuxParser::ActiveJiffies();

Idle = LinuxParser::IdleJiffies();
Active = LinuxParser::ActiveJiffies();

PrevTotal = PrevIdle + PrevActive;
Total = Idle + Active;

// differentiate

Totald = Total - PrevTotal;
Idled = Idle - PrevIdle;
utilization = (float)(Totald - Idled)/Totald;


return utilization;

}