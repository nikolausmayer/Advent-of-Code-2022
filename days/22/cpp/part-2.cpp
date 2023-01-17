
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
      //fmt::print("walk {} steps\n", steps);
      for (; steps > 0; --steps) {
        const auto dx = (dir == 2 ? -1 :
                         dir == 0 ?  1 : 0);
        const auto dy = (dir == 3 ? -1 :
                         dir == 1 ?  1 : 0);
        auto nx = x + dx;
        auto ny = y + dy;
        //auto from_face = -1;
        //auto next_face = -1;
        //fmt::print("  tap {},{}  ({} {}  {})\n", nx, ny, dx, dy, dir);
        /// Wrap around
        int next_dir = dir;
        if (map.count({nx,ny}) == 0) {
          /// Change cube face
          ///      +------+      +------+
          ///     /  1x  /|     /|......|
          ///    /  y   / |    /.|...6y.|
          ///   +------+ 2|   +.y|...x..|
          ///   |      |yx+   |4.+------+
          ///   |  3x  | /    |x/...y../ 
          ///   |  y   |/     |/...5x./  
          ///   +------+      +------+   
          ///  12    1^6>, 1<4>, 2^6^, 2>5<, 2v3<
          ///  3     3<4v, 3>2^
          /// 45     4^3>, 4<1>, 5>2<, 5v6<
          /// 6      6<1v, 6>5^, 6v2v

          const auto tx = nx;
          const auto ty = ny;
          if        (tx <  100 and ty ==  -1) { /// 1^6>
            assert(dir == 3); 
            nx = 0; 
            ny = (tx - 50) + 150; 
            next_dir = 0; //from_face = 1; next_face = 6;
          } else if (tx ==  49 and ty <   50) { /// 1<4> (flip y)
            assert(dir == 2); 
            nx = 0; 
            ny = (49 - ty) + 100; 
            next_dir = 0; //from_face = 1; next_face = 4;
          } else if (tx >= 100 and ty ==  -1) { /// 2^6^
            assert(dir == 3);
            nx = tx - 100; 
            ny = ty + 200; 
            next_dir = 3; //from_face = 2; next_face = 6;
          } else if (tx == 150 and ty <   50) { /// 2>5< (flip y)
            assert(dir == 0); 
            nx = 99; 
            ny = (49 - ty) + 100; 
            next_dir = 2; //from_face = 2; next_face = 5;
          } else if (tx >= 100 and ty ==  50) { /// 2v3<
            assert(dir == 1); 
            nx = 99; 
            ny = tx - 100 + 50; 
            next_dir = 2; //from_face = 2; next_face = 3;
          } else if (tx ==  49 and ty <  100) { /// 3<4v
            assert(dir == 2); 
            nx = ty - 50; 
            ny = 100; 
            next_dir = 1; //from_face = 3; next_face = 4;
          } else if (tx == 100 and ty >=  50 and ty < 100) { /// 3>2^
            assert(dir == 0); 
            nx = ty - 50 + 100; 
            ny = 49; 
            next_dir = 3; //from_face = 3; next_face = 3;
          } else if (tx <   50 and ty ==  99) { /// 4^3>
            assert(dir == 3); 
            nx = 50; 
            ny = tx + 50; 
            next_dir = 0; //from_face = 4; next_face = 1;
          } else if (tx ==  -1 and ty <  150) { /// 4<1> (flip y)
            assert(dir == 2); 
            nx = 50; 
            ny = 49 - (ty - 100);
            next_dir = 0; //from_face = 4; next_face = 2;
          } else if (tx == 100 and ty >= 100) { /// 5>2< (flip y)
            assert(dir == 0); 
            nx = 149; 
            ny = 49 - (ty - 100);
            next_dir = 2; //from_face = 5; next_face = 2;
          } else if (tx >=  50 and ty == 150) { /// 5v6<
            assert(dir == 1); 
            nx = 49; 
            ny = tx - 50 + 150; 
            next_dir = 2; //from_face = 5; next_face = 6;
          } else if (tx ==  -1 and ty >= 150) { /// 6<1v
            assert(dir == 2); 
            nx = ty - 150 + 50; 
            ny = 0; 
            next_dir = 1; //from_face = 6; next_face = 1;
          } else if (tx ==  50 and ty >= 150) { /// 6>5^
            assert(dir == 0); 
            nx = ty - 150 + 50; 
            ny = 149; 
            next_dir = 3; //from_face = 6; next_face = 5;
          } else if (tx <   50 and ty == 200) { /// 6v2v
            assert(dir == 1); 
            nx = tx + 100; 
            ny = 0; 
            next_dir = 1; //from_face = 6; next_face = 2;
          } else {
            assert(false);
          }
          //fmt::print("  flip to {},{}, face {}->{}, dir {}->{}\n", nx, ny, from_face, next_face, dir, next_dir);
        }
        /// Do not move into obstacle
        if (map[{nx,ny}] == 2) {
          //fmt::print("  cannot move to {},{}\n", nx, ny, x, y);
          break;
        }
        x = nx;
        y = ny;
        dir = next_dir;
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

