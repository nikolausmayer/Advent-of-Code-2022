
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

  auto register_x  = 1;
  auto clock_cycle = 0;

  std::array<bool,240> CRT{false};

  auto tick = [&]() {
    if (std::abs(register_x - (clock_cycle % 40)) <= 1)
      CRT[clock_cycle] = true;
    ++clock_cycle;
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

  for (auto i = 0u; i < CRT.size(); ++i) {
    fmt::print("{}", CRT[i] ? "█" : " ");
    if ((i+1) % 40 == 0)
      fmt::print("\n");
  }

  return EXIT_SUCCESS;
}

