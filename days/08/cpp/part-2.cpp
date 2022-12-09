
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <fstream>
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

  std::vector<std::vector<int>> map;

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    
    std::vector<int> row;
    for (auto c: line)
      row.push_back(c - '0');
    map.push_back(std::move(row));
  }

  const auto H = (int)map.size();
  const auto W = (int)map[0].size();

  std::vector<std::vector<int>> scores(H, std::vector<int>(W, 1));
  for (auto y = 1; y < H-1; ++y) {
    /// The monotonic stack stores the INDICES of trees. The heights are then
    /// just looked up when needed.
    std::stack<int> monotonic_stack;
    /// Look west
    monotonic_stack.push(0);
    for (auto x = 1; x < W-1; ++x) {
      const auto tree = map[y][x];
      while (monotonic_stack.size() and map[y][monotonic_stack.top()] < tree)
        monotonic_stack.pop();
      scores[y][x] *= x - (monotonic_stack.size() ? monotonic_stack.top() : 0);
      monotonic_stack.push(x);
    }
    while (monotonic_stack.size())
      monotonic_stack.pop();
    /// Look east
    monotonic_stack.push(W-1);
    for (auto x = W-2; x > 0; --x) {
      const auto tree = map[y][x];
      while (monotonic_stack.size() and map[y][monotonic_stack.top()] < tree)
        monotonic_stack.pop();
      scores[y][x] *= (monotonic_stack.size() ? monotonic_stack.top() : W-1) - x;
      monotonic_stack.push(x);
    }
  }
  for (auto x = 1; x < W-1; ++x) {
    /// Look south
    std::stack<int> monotonic_stack;
    monotonic_stack.push(0);
    for (auto y = 1; y < H-1; ++y) {
      const auto tree = map[y][x];
      while (monotonic_stack.size() and map[monotonic_stack.top()][x] < tree)
        monotonic_stack.pop();
      scores[y][x] *= y - (monotonic_stack.size() ? monotonic_stack.top() : 0);
      monotonic_stack.push(y);
    }
    while (monotonic_stack.size())
      monotonic_stack.pop();
    /// Look north
    monotonic_stack.push(H-1);
    for (auto y = H-2; y > 0; --y) {
      const auto tree = map[y][x];
      while (monotonic_stack.size() and map[monotonic_stack.top()][x] < tree)
        monotonic_stack.pop();
      scores[y][x] *= (monotonic_stack.size() ? monotonic_stack.top() : H-1) - y;
      monotonic_stack.push(y);
    }
  }

  int max_scenic_score = 0;
  for (const auto& row: scores)
    for (auto scenic_score: row)
      max_scenic_score = std::max(max_scenic_score, scenic_score);
  fmt::print("Highest scenic score: {}\n", max_scenic_score);

  return EXIT_SUCCESS;
}

