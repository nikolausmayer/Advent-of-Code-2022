
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
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
#include <fmt/ostream.h>


int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  typedef std::array<int,2> P;
  std::map<P,int> map;

  auto minx = 99999;
  auto maxx = 0;
  auto miny = 99999;
  auto maxy = 0;

  for (;;) {
    if (infile.peek() == std::ifstream::traits_type::eof())
      break;
    auto pa = -1;
    auto pb = -1;
    while (infile.peek() != '\n') {
      if (infile.peek() == ' ')
        infile.ignore(4);  /// " -> "
      int a, b;
      infile >> a;   /// (X),Y
      infile.get();  /// X(,)Y
      infile >> b;   /// X,(Y)
    
      minx = std::min(minx, a);
      maxx = std::max(maxx, a);
      miny = std::min(miny, b);
      maxy = std::max(maxy, b);

      map[{pa, pb}] = 1;
      if (pa != -1) {
        while (a != pa or b != pb) {
          pa += (a != pa) * (a > pa ? 1 : -1);
          pb += (b != pb) * (b > pb ? 1 : -1);
          map[{pa, pb}] = 1;
        }
      } else {
        pa = a;
        pb = b;
      }
      map[{a, b}] = 1;
    }
    infile.get();
  }

  auto print_map = [&]() {
    for (auto y = miny; y <= maxy; ++y) {
      for (auto x = minx; x <= maxx; ++x) {
        fmt::print("{}", ".#o"[map[{x,y}]]);
      }
      fmt::print("\n");
    }
    fmt::print("\n");
  };

  minx = std::min(minx-1, 499);
  maxx = std::max(maxx+1, 501);
  miny = std::min(miny-1, -1);
  maxy = std::max(maxy+1, 1);

  //print_map();

  for (auto i = 0;; ++i) {
    auto x = 500;
    auto y = 0;
    bool settled = false;
    for (; not settled;) {
      if      (map[{x  ,y+1}] == 0) ++y;
      else if (map[{x-1,y+1}] == 0) --x, ++y;
      else if (map[{x+1,y+1}] == 0) ++x, ++y;
      else                          settled = true;

      if (y >= maxy)
        break;
    }
    if (settled) {
      map[{x,y}] = 2;
      //print_map();
    } else {
      fmt::print("Stable after {} sand units\n", i);
      break;
    }
  }

  return EXIT_SUCCESS;
}

