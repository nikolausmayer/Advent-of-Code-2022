
#include <cassert>
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
  std::vector<std::vector<bool>> visibility(H, std::vector<bool>(W, false));
  /// Horizontal sweeps
  for (auto y = 1; y < H-1; ++y) {
    auto max_height = map[y][0];
    for (auto x = 1; x < W-1; ++x) {
      const auto tree = map[y][x];
      visibility[y][x] = visibility[y][x] | (tree > max_height);
      max_height = std::max(max_height, tree);
    }
    max_height = map[y][W-1];
    for (auto x = W-2; x > 0; --x) {
      const auto tree = map[y][x];
      visibility[y][x] = visibility[y][x] | (tree > max_height);
      max_height = std::max(max_height, tree);
    }
  }
  /// Vertical sweeps
  for (auto x = 1; x < W-1; ++x) {
    auto max_height = map[0][x];
    for (auto y = 1; y < H-1; ++y) {
      const auto tree = map[y][x];
      visibility[y][x] = visibility[y][x] | (tree > max_height);
      max_height = std::max(max_height, tree);
    }
    max_height = map[H-1][x];
    for (auto y = H-2; y > 0; --y) {
      const auto tree = map[y][x];
      visibility[y][x] = visibility[y][x] | (tree > max_height);
      max_height = std::max(max_height, tree);
    }
  }

  /// Edge trees
  auto result = 2*W + 2*H - 4;
  /// Interior trees
  for (auto y = 1; y < H-1; ++y) {
    for (auto x = 1; x < W-1; ++x) {
      result += visibility[y][x];
    }
  }
  fmt::print("Visible trees: {}\n", result);

  return EXIT_SUCCESS;
}

