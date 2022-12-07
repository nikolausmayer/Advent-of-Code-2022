
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
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  /// The real input is just a single line, but this makes the tests easier
  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    for (std::size_t end = 14; end <= line.size(); ++end) {
      std::set<char> chars;
      for (auto i = end - 14; i < end; ++i) {
        chars.insert(line[i]);
      }
      if (chars.size() == 14) {
        fmt::print("Marker {} at {}\n", line.substr(end - 14, 14), end);
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}

