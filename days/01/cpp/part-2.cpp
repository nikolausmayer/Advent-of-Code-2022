
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
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

  std::vector<int> calories_per_elf{0};
  for (std::string calories;;) {
    std::getline(infile, calories);
    if (infile.eof()) {
      break;
    }
    if (calories == "") {
      calories_per_elf.push_back(0);
      continue;
    }
    calories_per_elf.back() += std::atoi(calories.c_str());
  }

  std::ranges::partial_sort(calories_per_elf, 
                            calories_per_elf.begin() + 3, 
                            std::greater<>());
  fmt::print("{}\n", std::accumulate(calories_per_elf.begin(),
                                     calories_per_elf.begin() + 3,
                                     0));

  return EXIT_SUCCESS;
}

