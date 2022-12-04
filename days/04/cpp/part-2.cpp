
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <map>
#include <regex>
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

  const std::regex RE("(\\d+)-(\\d+),(\\d+)-(\\d+)");

  int result = 0;

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;

    std::smatch base_match{};
    std::regex_match(line, base_match, RE);
    const int start1 = std::stoi(base_match[1].str());
    const int end1   = std::stoi(base_match[2].str());
    const int start2 = std::stoi(base_match[3].str());
    const int end2   = std::stoi(base_match[4].str());

    result += not (end1 < start2 or end2 < start1);
  }

  fmt::print("Pairs with at least partial overlap: {}\n", result);

  return EXIT_SUCCESS;
}

