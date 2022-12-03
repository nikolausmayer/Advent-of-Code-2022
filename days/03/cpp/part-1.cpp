
#include <cstdint>
#include <algorithm>
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

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;

    /// Sets of item types per compartment
    std::set<char> first_compartment, second_compartment;
    for (auto c: line.substr(0, line.size()/2))
      first_compartment.insert(c);
    for (auto c: line.substr(line.size()/2))
      second_compartment.insert(c);
    std::vector<char> isect;
    std::ranges::set_intersection(first_compartment, second_compartment,
                                  std::back_inserter(isect));

    /// Accumulate score
    score += ((isect[0] & (1 << 5)) == 0
              ? isect[0] - 'A' + 27  // uppercase
              : isect[0] - 'a' + 1);
  }

  fmt::print("Sum of priorities: {}\n", score);

  return EXIT_SUCCESS;
}

