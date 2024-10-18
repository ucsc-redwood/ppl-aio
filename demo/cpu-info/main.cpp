#include <fstream>
#include <iostream>

void print_cpu_info() {
  std::ifstream cpuinfo("/proc/cpuinfo");
  if (!cpuinfo.is_open()) {
    std::cerr << "Failed to open /proc/cpuinfo" << std::endl;
    return;
  }

  std::string line;
  while (std::getline(cpuinfo, line)) {
    // Output each line from /proc/cpuinfo
    std::cout << line << std::endl;
  }

  cpuinfo.close();
}

int main() {
  print_cpu_info();
  return 0;
}