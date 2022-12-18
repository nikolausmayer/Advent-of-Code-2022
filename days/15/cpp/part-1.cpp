
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

  const auto exclusion_row = 2'000'000;
  std::set<int> row_exclusions;
  std::set<int> beacons_in_row;

  for (;;) {
    if (infile.peek() == std::ifstream::traits_type::eof())
      break;
    int sx, sy, bx, by;
    infile.ignore(12);
    infile >> sx;
    infile.ignore(4);
    infile >> sy;
    infile.ignore(25);
    infile >> bx;
    infile.ignore(4);
    infile >> by;
    infile.get();

    if (by == exclusion_row)
      beacons_in_row.insert(bx);

    const auto manhattan = std::abs(sx - bx) + std::abs(sy - by);
    const auto target_row_range = manhattan - std::abs(sy - exclusion_row);
    for (auto i = -target_row_range; i <= target_row_range; ++i)
      row_exclusions.insert(sx + i);
  }
  for (auto b: beacons_in_row)
    row_exclusions.erase(b);
  fmt::print("Positions in row #{} where no beacon can be: {}\n", 
             exclusion_row, row_exclusions.size());

  return EXIT_SUCCESS;
}

