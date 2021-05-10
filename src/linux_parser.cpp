#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float Total;
  float Free;

  std::ifstream filename (kProcDirectory + kMeminfoFilename);
  if (filename.is_open()){
    while(std::getline (filename, line)){
      std::replace (line.begin(), line.end(), ':',' ');
      std::istringstream linestream(line);
      while (linestream>> key >> value){
        if(key == "MemTotal" ){
           Total = stof(value);
        }else if (key == "MemFree"){
           Free = stof(value);
        }
        
      }

    }
  }
  return (Total - Free)/Total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string systemUptime;
  string idleUptime;

  std::ifstream filepath (kProcDirectory + kUptimeFilename);
  if (filepath. is_open()){
    std::getline(filepath , line);
    std::istringstream linestream (line);
    linestream >> systemUptime >> idleUptime; 
   }
  return stoi(systemUptime) - stoi(idleUptime);
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
   long  uptime = LinuxParser::UpTime();
  double frequency = sysconf(_SC_CLK_TCK);

    return frequency * uptime;

 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
std::ifstream fpid_jif(kProcDirectory + to_string(pid) + kStatFilename);
  long value;

  string line, utime, stime, cutime, cstime;
  if (fpid_jif.is_open()) {
    getline(fpid_jif, line);
    std::istringstream jif_str(line);
    int count = 1;
    string temp;
    while (count <= 13) {
      jif_str >> temp;
      count++;
    }
    jif_str >> utime >> stime >> cutime >> cstime;
  }
  value = std::stol(utime) + std::stol(stime) + std::stol(cutime) +
          std::stol(cstime);
  return value;
 }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  std::ifstream my_file(kProcDirectory + kStatFilename);
  if (my_file) {
    string line;
    getline(my_file, line);
    std::istringstream ajif_string(line);
    string key;
    int temp;
    int kUser_, kNice_, kSystem_, kIRQ_, kSoftIRQ_, kSteal_;
    ajif_string >> key;
    if (key == "cpu") {
      ajif_string >> kUser_ >> kNice_ >> kSystem_;
      ajif_string >> temp >> temp;
      ajif_string >> kIRQ_ >> kSoftIRQ_ >> kSteal_;
    }
    return kUser_ + kNice_ + kSystem_ + kIRQ_ + kSoftIRQ_ + kSteal_;
  }
  return 0;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string key;
  string user, nice, system, idle, iowait, irq, softirq;
  string steal, guest, guest_nice;
  long idleJiffies;
  long idleConversion = 0;
  long iowaitConverion = 0;

  std::ifstream filename(kProcDirectory + kStatFilename);
  if (filename.is_open()){
    while(std::getline(filename, line)){
      std::istringstream linestream(line);
      while(linestream>>key>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice){
        if (key =="cpu"){
          if (iowait != ""){
           iowaitConverion = std::stol (iowait);}
          if (idle != ""){
           idleConversion = std::stol (idle);}
          
           idleJiffies = iowaitConverion + idleConversion;
          
        }
      }
 
    }


  }

  return idleJiffies;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {  
  string line;
  string key;
  string value;
  std::vector<string> aggjiffies;

  std::ifstream filename (kProcDirectory + kStatFilename);
  if (filename.is_open()){
    std::getline(filename, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      if (key != "cpu") {
        aggjiffies.push_back(key);
      }
    }
    
  }
return aggjiffies;

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;

  std::ifstream filename (kProcDirectory + kStatFilename);
  if (filename.is_open()){
    while (std::getline(filename, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes"){
          if (value != "")
          return stoi(value);
        }
      }
    }
  }

return 0;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;

  std::ifstream filename (kProcDirectory + kStatFilename);
  if (filename.is_open()){
    while (std::getline(filename, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running"){
          if (value != "")
          return stoi(value);
        }
      }
    }
  }

return 0;
 }
 

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filename(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filename.is_open()){
      std::getline(filename, line);
  }
return line;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid){ 
  string line;
  string key;
  string value;
  
  std::ifstream filename (kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filename.is_open()){
    while(std::getline(filename, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmSize "){
            return value;
        }

      }
    }
  }
  return value;

 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;

  std::ifstream filename (kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filename.is_open()){
    while(std::getline(filename, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "Uid "){
            return value;
        }

      }
    }
  }
  return value;

 }


// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string x, value;
  string userID = Uid(pid);
  std::ifstream filename(kPasswordPath);
  
  if (filename.is_open()){
      while(std::getline(filename,line)){
        std::replace (line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while(linestream >> value >> x >> key){
          if(key == userID){
            return value;
          }
        }
      }

  }

    return value;

 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  float starttime;
 
   std::ifstream filename (kProcDirectory +  std::to_string(pid) + kStatFilename);
  
  if (filename.is_open()){
    std::getline(filename,line);
    std::istringstream linestream(line);

    for (int i=0; i<=21;i++) {
      linestream>>value;
      if (i==21) {
        if (value != ""){
        starttime = stof(value) / sysconf(_SC_CLK_TCK);
        return starttime;
        }
      }
    }
  }
  return 0.0; 
 }
