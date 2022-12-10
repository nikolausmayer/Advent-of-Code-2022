
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

  std::set<std::array<int,2>> tail_visited{};
  std::array<std::array<int,2>,10> knot_xy{};

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    const auto dir = line[0];
    const auto steps = std::stoi(line.substr(2));
    //fmt::print("<{}:{}>\n",dir,steps);

    for (auto i = 0; i < steps; ++i) {
      knot_xy[0][0] += (dir == 'R' ?  1 :
                        dir == 'L' ? -1 : 0);
      knot_xy[0][1] += (dir == 'U' ?  1 :
                        dir == 'D' ? -1 : 0);
      for (auto j = 1; j < 10; ++j) {
        const auto hx = knot_xy[j-1][0];
        const auto hy = knot_xy[j-1][1];
        auto&      tx = knot_xy[j][0];
        auto&      ty = knot_xy[j][1];
        if (std::max(std::abs(hx - tx), std::abs(hy - ty)) == 2) {
          tx += (hx > tx ? 1 : hx < tx ? -1 : 0);
          ty += (hy > ty ? 1 : hy < ty ? -1 : 0);
        } else {
          break;
        }
      }
      tail_visited.insert(knot_xy[9]);
      //fmt::print("  <{}:{}  {}:{}>\n", knot_xy[0][0],knot_xy[0][1],knot_xy[9][0],knot_xy[9][1]);
    }
  }

  fmt::print("Positions visited by tail: {}\n", tail_visited.size());

  return EXIT_SUCCESS;
}

