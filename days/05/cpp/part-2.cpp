
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

  /// Parse starting stacks of crates
  std::vector<std::vector<char>> stacks;
  for (std::string line;;) {
    std::getline(infile, line);
    if (line == "")
      break;
    for (auto i = 0u, stack = 0u; i < line.size(); i += 4, ++stack) {
      if (line[i] == '[') {
        while (stacks.size() <= stack)
          stacks.push_back({});
        stacks[stack].push_back(line[i+1]);
      }
    }
  }
  for (auto& s: stacks)
    std::ranges::reverse(s);

  /// Offloading onto a buffer and then onto target stack keeps order
  std::vector<char> buffer_stack;

  /// Parse rearrangement procedure
  const std::regex RE("move (\\d+) from (\\d+) to (\\d+)");
  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    std::smatch match{};
    std::regex_match(line, match, RE);
    const int count = std::stoi(match[1].str());
    const int from  = std::stoi(match[2].str());
    const int to    = std::stoi(match[3].str());
    for (auto i = 0; i < count; ++i) {
      buffer_stack.push_back(stacks[from - 1].back());
      stacks[from - 1].pop_back();
    }
    for (auto i = 0; i < count; ++i) {
      stacks[to - 1].push_back(buffer_stack.back());
      buffer_stack.pop_back();
    }

    //for (auto s: stacks) {
    //  for (auto v: s) fmt::print("{} ", v);
    //  fmt::print("\n");
    //}
    //fmt::print("\n");
  }

  fmt::print("Answer: ");
  for (auto s: stacks)
    fmt::print("{}", s.back());
  fmt::print("\n");

  return EXIT_SUCCESS;
}

