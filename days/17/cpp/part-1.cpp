
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

  std::string jet_pattern;
  std::getline(infile, jet_pattern);
  std::size_t next_jet = 0;

  std::vector<std::vector<std::vector<bool>>> rock_shapes = {
    {{1,1,1,1}},        /// -
    {{0,1,0},           /// +
     {1,1,1},
     {0,1,0}},
    {{0,0,1},           /// L
     {0,0,1},
     {1,1,1}},
    {{1},{1},{1},{1}},  /// |
    {{1,1},             /// o
     {1,1}}
  };
  std::size_t next_rock = 0;

  constexpr auto W = 7;

  std::vector<std::array<bool,W>> map;
  for (;;) {
    //{
    //  fmt::print("\nBefore rock {}:\n|.......|\n", next_rock);
    //  for (auto y = (int)map.size()-1; y >= 0; --y) {
    //    fmt::print("|");
    //    for (auto x = 0; x < 7; ++x) {
    //      fmt::print("{}", (map[y][x] ? "#" : "."));
    //    }
    //    fmt::print("|\n");
    //  }
    //  fmt::print("+-------+\n");
    //}

    int rock_x = 2; 
    int rock_y = map.size() + rock_shapes[next_rock % rock_shapes.size()].size() + 2;
    const auto& falling_rock = rock_shapes[next_rock % rock_shapes.size()];
    ++next_rock;
    if (next_rock > 2022) {
      break;
    }

    for (;;) {
      //{
      //  fmt::print("{}\n", jet_pattern);
      //  auto c = 0u;
      //  for (; c < (next_jet % jet_pattern.size()); ++c)
      //    fmt::print(" ");
      //  fmt::print("{}", jet_pattern[next_jet % jet_pattern.size()]);
      //  for (; c < jet_pattern.size(); ++c)
      //    fmt::print(" ");
      //  fmt::print("\n");
      //}
      const auto jet = jet_pattern[next_jet % jet_pattern.size()];
      ++next_jet;

      /// Sideways movement
      if (not ((rock_x == 0 and jet == '<') or
               (rock_x == 7 - (int)falling_rock[0].size() and jet == '>'))) {
        bool shift_ok = true;
        for (auto ry = 0; ry < (int)falling_rock.size(); ++ry) {
          for (auto rx = 0; rx < (int)falling_rock[0].size(); ++rx) {
            const auto ty = rock_y - ((int)falling_rock.size() - ry - 1);
            const auto tx = rock_x + rx;
            if (ty < (int)map.size() and
                falling_rock[(int)falling_rock.size() - ry - 1][rx] and
                map[ty][tx + (jet == '<' ? -1 : 1)]) {
              shift_ok = false;
            }
          }
        }
        if (shift_ok) {
          rock_x += (jet == '<' ? -1 : 1);
        }
      }

      //{
      //  for (auto y = rock_y; y >= std::max(0, rock_y - 7); --y) {
      //    fmt::print("|");
      //    for (auto x = 0; x < 7; ++x) {
      //      auto c = ".";
      //      if (y < map.size() and map[y][x])
      //        c = "#";
      //      const auto ry = rock_y - y;
      //      const auto rx = x - rock_x;
      //      if (ry >= 0 and ry < falling_rock.size() and
      //          rx >= 0 and rx < falling_rock[0].size() and
      //          falling_rock[ry][rx])
      //        c = "@";
      //      fmt::print("{}", c);
      //    }
      //    fmt::print("| {}\n", y);
      //  }
      //  fmt::print("|vvvvvvv|\n");
      //}

      /// Downward movement
      {
        bool fall_ok = true;
        for (auto ry = 0; ry < (int)falling_rock.size(); ++ry) {
          for (auto rx = 0; rx < (int)falling_rock[0].size(); ++rx) {
            const auto ty = rock_y - ((int)falling_rock.size() - ry - 1) - 1;
            const auto tx = rock_x + rx;
            if (ty < 0 or
                (ty < (int)map.size() and 
                 falling_rock[(int)falling_rock.size() - ry - 1][rx] and
                 map[ty][tx])) {
              fall_ok = false;
            }
          }
        }
        if (fall_ok) {
          --rock_y;
        } else {
          //{
          //  for (auto y = rock_y + 4; y >= std::max(0, rock_y - 7); --y) {
          //    fmt::print("|");
          //    for (auto x = 0; x < 7; ++x) {
          //      auto c = ".";
          //      if (y < map.size() and map[y][x])
          //        c = "#";
          //      const auto ry = rock_y - y;
          //      const auto rx = x - rock_x;
          //      if (ry >= 0 and ry < falling_rock.size() and
          //          rx >= 0 and rx < falling_rock[0].size() and
          //          falling_rock[ry][rx])
          //        c = "@";
          //      fmt::print("{}", c);
          //    }
          //    fmt::print("| {}\n", y);
          //  }
          //  fmt::print("|vvvvvvv|\n");
          //}

          /// Register rock into map (and extend map height)
          for (auto ry = 0; ry < (int)falling_rock.size(); ++ry) {
            const auto ty = rock_y - ((int)falling_rock.size() - ry - 1);
            if (ty == (int)map.size()) {
              map.push_back({false});
            }
            for (auto rx = 0; rx < (int)falling_rock[0].size(); ++rx) {
              const auto tx = rock_x + rx;
              map[ty][tx] |= falling_rock[(int)falling_rock.size() - ry - 1][rx];
            }
          }
          /// Next rock
          break;
        }
      }
    }
  }

  fmt::print("Rock tower height: {}\n", map.size());

  return EXIT_SUCCESS;
}

