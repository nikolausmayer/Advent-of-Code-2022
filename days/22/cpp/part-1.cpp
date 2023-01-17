
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

  std::map<std::array<int,2>,int> map;
  std::string instructions;
  int W = 0, H = 0;
  for (;; ++H) {
    std::string line;
    std::getline(infile, line);
    W = std::max(W, (int)line.size());
    for (auto x = 0; x < (int)line.size(); ++x)
      if (line[x] != ' ')
        map[{x,H}] = (line[x] == '.' ? 1 : 2);
    if (line == "") {
      /// Read including trailing newline; this makes the loop below simpler
      std::getline(infile, instructions, 'x');
      break;
    }
  }

  /// Find starting position
  int x = 0;
  int y = 0;
  for (; map.count({x,y}) == 0 or map[{x,y}] == 2; ++x) {}
  //fmt::print("start at {},{}\n", x, y);

  int steps = 0;
  int dir   = 0;  /// 0E, 1S, 2W, 3N
  for (auto c: instructions) {
    if (c >= '0' and c <= '9') {
      steps = 10 * steps + (c - '0');
    } else {
      const auto dx = (dir == 2 ? -1 :
                       dir == 0 ?  1 : 0);
      const auto dy = (dir == 3 ? -1 :
                       dir == 1 ?  1 : 0);
      //fmt::print("walk {} steps\n", steps);
      for (; steps > 0; --steps) {
        auto nx = x + dx;
        auto ny = y + dy;
        //fmt::print("  tap {},{}\n", nx, ny);
        /// Wrap around
        if (map.count({nx,ny}) == 0) {
          nx -= dx;
          ny -= dy;
          while (map.count({nx,ny})) {
            nx -= dx;
            ny -= dy;
          }
          nx += dx;
          ny += dy;
          //fmt::print("  wraparound to {},{}\n", nx, ny);
        }
        /// Do not move into obstacle
        if (map[{nx,ny}] == 2) {
          //fmt::print("  cannot move to {},{}\n", nx, ny, x, y);
          break;
        }
        x = nx;
        y = ny;
      }
      //fmt::print("  now at {},{}\n", x, y);
      steps = 0;
      if (c == 'R') {
        dir = (dir + 1) % 4;
        //fmt::print("turn right to {}\n", dir);
      } else if (c == 'L') {
        dir = (dir + 4 - 1) % 4;
        //fmt::print("turn left to {}\n", dir);
      }
    }
  }

  fmt::print("Stopped at row {}, column {}, dir {}; final password is {}\n",
             y+1, x+1, dir, 1000 * (y+1) + 4 * (x+1) + dir);

  return EXIT_SUCCESS;
}

