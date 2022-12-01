
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
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

  fmt::print("{}\n", *std::ranges::max_element(calories_per_elf));

  return EXIT_SUCCESS;
}

