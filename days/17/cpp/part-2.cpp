
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

  std::map<std::array<int,7>, std::vector<std::array<std::size_t, 2>>> signatures;
  std::map<std::size_t, std::size_t> tower_heights;
  bool hyperjumped = false;
  std::size_t jump_skipped = 0;

  const auto target = 1'000'000'000'000ul;

  std::vector<std::array<bool,W>> map;
  for (;;) {
    if ((not hyperjumped) and
        next_rock != 0 and
        next_rock % rock_shapes.size() == 0) {
      std::array<int,7> signature;
      for (auto x = 0; x < 7; ++x) {
        for (auto y = (int)map.size() - 1; y >= 0; --y) {
          if (map[y][x]) {
            signature[x] = map.size() - y + 1;
            break;
          }
          if (y == 0) {
            signature[x] = -1;
          }
        }
      }
      if (signatures.count(signature)) {
        for (const auto& m: signatures[signature]) {
          if (m[1] == (next_jet % jet_pattern.size())) {
            fmt::print("Signature of rock #{} [ ", next_rock);
            for (auto i = 0u; i < signature.size(); ++i) {
              fmt::print("{} ", signature[i]);
            }
            fmt::print("] re-identified from rock #{}", m[0]);
            fmt::print(" with a tower height difference of {} - {} = {}\n",
                       map.size(),
                       tower_heights[m[0]],
                       map.size() - tower_heights[m[0]]);
            const auto delta_t = next_rock - m[0];
            const auto delta_h = map.size() - tower_heights[m[0]];
            const auto skips = (target - next_rock) / delta_t;
            next_rock   += skips * delta_t;
            jump_skipped = skips * delta_h;
            fmt::print("...((()))...\n");
            fmt::print("hyperjumped to rock #{} with tower height {}\n",
                       next_rock, map.size() + jump_skipped);
            hyperjumped = true;
          }
        }
      } 
      if (not hyperjumped) {
        signatures[signature].push_back({next_rock, 
                                         next_jet % jet_pattern.size()});;
        tower_heights[next_rock] = map.size();
      }
    }

    int rock_x = 2; 
    int rock_y = map.size() + rock_shapes[next_rock % rock_shapes.size()].size() + 2;
    const auto& falling_rock = rock_shapes[next_rock % rock_shapes.size()];
    ++next_rock;
    if (next_rock > target) {
      break;
    }

    for (;;) {
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

  fmt::print("Rock tower height: {}\n", map.size() + jump_skipped);

  return EXIT_SUCCESS;
}

