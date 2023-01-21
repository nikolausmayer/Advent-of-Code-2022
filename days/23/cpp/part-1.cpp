
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

  std::set<std::array<int,2>> elves;

  for (int y = 0;; ++y) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    for (auto x = 0; x < (int)line.size(); ++x)
      if (line[x] == '#')
        elves.insert({x,y});
  }

  //auto print_map = [&]() {
  //  int minx = 9999, maxx = -9999,
  //      miny = 9999, maxy = -9999;
  //  for (const auto& [x,y]: elves) {
  //    minx = std::min(minx, x);
  //    miny = std::min(miny, y);
  //    maxx = std::max(maxx, x);
  //    maxy = std::max(maxy, y);
  //  }
  //  for (auto y = miny-1; y <= maxy+1; ++y) {
  //    for (auto x = minx-1; x <= maxx+1; ++x) {
  //      fmt::print("{}", (elves.contains({x,y}) ? "#" : "."));
  //    }
  //    fmt::print("\n");
  //  }
  //  fmt::print("\n");
  //};

  constexpr std::array<std::array<int,2>,12> checks{{
    { 0,-1}, /*N*/ { 1,-1}, /*NE*/ {-1,-1}, /*NW*/
    { 0, 1}, /*S*/ { 1, 1}, /*SE*/ {-1, 1}, /*SW*/
    {-1, 0}, /*W*/ {-1,-1}, /*NW*/ {-1, 1}, /*SW*/
    { 1, 0}, /*E*/ { 1,-1}, /*NE*/ { 1, 1}, /*SE*/
  }};
  std::size_t check_start = 0;

  for (auto round = 0; round < 10; ++round) {
    //fmt::print("Round {}\n", round+1);
    //print_map();
    std::map<std::array<int,2>,std::array<int,2>> elf_proposals{};
    std::map<std::array<int,2>,int> proposal_counts{};
    for (const auto& elf: elves) {
      auto neighbors = false;
      for (auto ci = 0u; ci < checks.size(); ++ci) {
        const auto [dx,dy] = checks[ci];
        neighbors |= elves.contains({elf[0]+dx,elf[1]+dy});
      }
      if (neighbors) {
        bool found = false;
        for (auto c = 0u; c < checks.size() and not found; c += 3) {
          auto occupied = false;
          for (auto ci = 0u; ci < 3u; ++ci) {
            const auto [dx,dy] = checks[(check_start + ci + c) % checks.size()];
            occupied |= elves.contains({elf[0]+dx,elf[1]+dy});
          }
          if (not occupied) {
            const std::array<int,2> proposal{
              elf[0]+checks[(check_start + c) % checks.size()][0],
              elf[1]+checks[(check_start + c) % checks.size()][1]
            };
            elf_proposals[elf] = proposal;
            ++proposal_counts[proposal];

            found = true;
          }
        }
        if (not found) {
          elf_proposals[elf] = elf;
        }
      } else {
        elf_proposals[elf] = elf;
        ++proposal_counts[elf];
      }
    }
    decltype(elves) new_elves{};
    for (const auto& elf: elves) {
      if (proposal_counts[elf_proposals[elf]] == 1) {
        new_elves.insert(elf_proposals[elf]);
      } else {
        new_elves.insert(elf);
      }
    }
    check_start = (check_start + 3) % checks.size();
    elves = new_elves;
  }
  //print_map();

  {
    int minx = 9999, maxx = -9999,
        miny = 9999, maxy = -9999;
    for (const auto& [x,y]: elves) {
      minx = std::min(minx, x);
      miny = std::min(miny, y);
      maxx = std::max(maxx, x);
      maxy = std::max(maxy, y);
    }
    auto free_ground_tiles = 0;
    for (auto y = miny; y <= maxy; ++y)
      for (auto x = minx; x <= maxx; ++x)
        free_ground_tiles += (not elves.contains({x,y}));
    fmt::print("Empty ground tiles: {}\n", free_ground_tiles);
  }

  return EXIT_SUCCESS;
}

