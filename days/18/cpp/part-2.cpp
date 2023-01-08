
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

  int minx = std::numeric_limits<int>::max(), maxx = 0,
      miny = std::numeric_limits<int>::max(), maxy = 0,
      minz = std::numeric_limits<int>::max(), maxz = 0;

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
    map.insert({x,y,z});
    minx = std::min(minx, x); maxx = std::max(maxx, x);
    miny = std::min(miny, y); maxy = std::max(maxy, y);
    minz = std::min(minz, z); maxz = std::max(maxz, z);
  }

  --minx; ++maxx;
  --miny; ++maxy;
  --minz; ++maxz;

  auto outer_surfaces = 0;
  std::queue<std::array<int,3>> queue;
  std::set<std::array<int,3>> visited;
  queue.push({minx, miny, minz});
  while (queue.size()) {
    const auto cube = queue.front();
    queue.pop();
    if (visited.contains(cube) or map.contains(cube))
      continue;
    visited.insert(cube);
    for (auto dim: {0, 1, 2}) {
      for (auto d: {-1, 1}) {
        auto test = cube;
        test[dim] += d;
        if (map.contains(test))
          ++outer_surfaces;
        else if (test[0] >= minx and test[0] <= maxx and
                 test[1] >= miny and test[1] <= maxy and
                 test[2] >= minz and test[2] <= maxz)
          queue.push(test);
      }
    }
  }
  fmt::print("Lava droplet outer surface area: {}\n", outer_surfaces);

  return EXIT_SUCCESS;
}

