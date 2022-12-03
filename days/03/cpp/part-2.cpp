
#include <cstdint>
#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <fmt/core.h>

int main(int argc, char* argv[]) 
{
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    return EXIT_FAILURE;
  }

  long score = 0;

  std::array<std::set<char>,3> group_rucksacks;
  std::string line;

  for (std::size_t idx = 0;; idx = (idx + 1) % 3) {
    std::getline(infile, line);
    if (infile.eof())
      break;

    group_rucksacks[idx].clear();
    for (auto c: line)
      group_rucksacks[idx].insert(c);

    if (idx == 2) {
      // Common item types in two rucksacks
      std::vector<char> isect_first_two;
      std::ranges::set_intersection(group_rucksacks[0], group_rucksacks[1],
                                    std::back_inserter(isect_first_two));
      // Common item type in all three rucksacks
      std::vector<char> isect;
      std::ranges::set_intersection(group_rucksacks[2], isect_first_two,
                                    std::back_inserter(isect));

      /// Accumulate score
      score += ((isect[0] & (1 << 5)) == 0
                ? isect[0] - 'A' + 27  // uppercase
                : isect[0] - 'a' + 1);
    }
  }

  fmt::print("Sum of priorities: {}\n", score);

  return EXIT_SUCCESS;
}

