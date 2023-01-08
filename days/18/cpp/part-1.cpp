
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <bitset>
#include <compare>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <vector>
#include <fmt/core.h>


int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  std::set<std::array<int,3>> map;

  const std::regex RE("(\\d+),(\\d+),(\\d+)");
  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    std::smatch base_match{};
    std::regex_match(line, base_match, RE);
    const int x = std::stoi(base_match[1].str());
    const int y = std::stoi(base_match[2].str());
    const int z = std::stoi(base_match[3].str());
    //fmt::print("<{}|{}|{}>\n", x, y, z);
    map.insert({x,y,z});
  }

  auto free_surfaces = 0;
  for (const auto& cube: map) {
    for (auto dim: {0, 1, 2}) {
      for (auto d: {-1, 1}) {
        auto test = cube;
        test[dim] += d;
        if (not map.contains(test))
          ++free_surfaces;
      }
    }
  }
  fmt::print("Lava droplet surface area: {}\n", free_surfaces);

  return EXIT_SUCCESS;
}

