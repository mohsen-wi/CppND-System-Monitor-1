#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  //cpu_proc_utilization_=Process::CpuUtilization();
  Process(int pid_);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;
  void update();
  long ActiveJiffies();
  

  // TODO: Declare any necessary private members
 private:
 int pid_;
 std::string user_;
 std::string command_;
 float cpuUtilization_;
 std::string ram_;
 long int uptime_;
 long actjiff_;

};

#endif