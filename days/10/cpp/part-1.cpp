
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <vector>
#include <fmt/core.h>


int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  auto register_x              = 1;
  auto clock_cycle             = 0;
  auto sum_of_signal_strengths = 0;

  auto tick = [&]() {
    ++clock_cycle;
    if (clock_cycle ==  20 or clock_cycle ==  60 or
        clock_cycle == 100 or clock_cycle == 140 or
        clock_cycle == 180 or clock_cycle == 220) {
      sum_of_signal_strengths += clock_cycle * register_x;
    }
  };

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;

    if (line[0] == 'a') {
      tick();
      tick();
      register_x += std::stoi(line.substr(4));
    } else {
      tick();
    }
  }

  fmt::print("Sum of signal strengths: {}\n", sum_of_signal_strengths);

  return EXIT_SUCCESS;
}

