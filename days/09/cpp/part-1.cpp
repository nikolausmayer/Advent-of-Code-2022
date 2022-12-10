
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

  std::set<std::array<int,2>> tail_visited;
  auto hx = 0;
  auto hy = 0;
  auto tx = 0;
  auto ty = 0;

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    const auto dir = line[0];
    const auto steps = std::stoi(line.substr(2));
    //fmt::print("<{}:{}>\n",dir,steps);

    for (auto i = 0; i < steps; ++i) {
      hx += (dir == 'R' ?  1 :
             dir == 'L' ? -1 : 0);
      hy += (dir == 'U' ?  1 :
             dir == 'D' ? -1 : 0);

      if (std::max(std::abs(hx - tx), std::abs(hy - ty)) == 2) {
        tx += (hx > tx ? 1 : hx < tx ? -1 : 0);
        ty += (hy > ty ? 1 : hy < ty ? -1 : 0);
      }
      tail_visited.insert({tx,ty});

      //fmt::print("  <{}:{}  {}:{}>\n", hx, hy, tx, ty);
    }
  }

  fmt::print("Positions visited by tail: {}\n", tail_visited.size());

  return EXIT_SUCCESS;
}

