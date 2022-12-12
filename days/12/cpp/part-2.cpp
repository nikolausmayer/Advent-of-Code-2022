
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <vector>
#include <fmt/core.h>
#include <fmt/ostream.h>

struct Node {
  std::array<int,2> pos;
  int cost, heuristic;

  Node(std::array<int,2> pos, int cost, std::array<int,2> target)
  : pos{pos},
    cost{cost}
  {
    heuristic = std::abs(pos[0] - target[0]) +
                std::abs(pos[1] - target[1]);
  }

  friend std::ostream& operator<<(std::ostream& os, const Node& a) {
    return os << "((" << a.pos[0] << "," << a.pos[1] << "), " << a.cost << ", " << a.heuristic << ")";
  }
};
template<> struct fmt::formatter<Node> : ostream_formatter {};

int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  std::map<std::array<int,2>,int> map;
  auto W = 0;
  auto H = 0;
  std::array<int,2> pos, target;

  std::list<std::array<int,2>> a_list;

  for (auto y = 0;; ++y) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;
    W = line.size();

    for (auto x = 0; x < (int)line.size(); ++x) {
      const auto c = line[x];
      switch (c) {
        case 'S': { pos    = {x, y}; map[{x,y}] = 0;  break; }
        case 'E': { target = {x, y}; map[{x,y}] = 26; break; }
        default : { map[{x,y}] = (c-'a'); }
      }
      if (c == 'a' or c == 'S') {
        a_list.push_back({x,y});
      }
    }
    H = std::max(H, y+1);
  }

  int shortest_shortest_path = 412;
  for (const auto& start: a_list) {
    std::set<std::array<int,2>> visited;

    auto cmp = [](const auto& a, const auto& b) {
      return a.cost + a.heuristic > b.cost + b.heuristic;
    };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> queue(cmp);
    queue.push({start, 0, target});
    while (queue.size()) {
      const auto next = queue.top();
      queue.pop();
      if (next.pos == target) {
        //if (next.cost < shortest_shortest_path) {
        //  fmt::print("New minimum: {}\n", next.cost);
        //}
        shortest_shortest_path = std::min(shortest_shortest_path, next.cost);
        break;
      }
      if (visited.contains(next.pos)) {
        continue;
      }
      visited.insert(next.pos);

      const auto x = next.pos[0];
      const auto y = next.pos[1];
      for (auto& [dx,dy]: std::vector<std::array<int,2>>{{-1,0},{1,0},{0,-1},{0,1}}) {
        if (x+dx < 0 or x+dx == W or y+dy < 0 or y+dy == H or
            visited.contains({x+dx, y+dy}) or
            map[{x+dx,y+dy}] > map[{x,y}] + 1) {
          continue;
        }
        queue.push({{x+dx, y+dy}, next.cost+1, target});
      }
    }
  }

  fmt::print("Fewest steps to goal: {}\n", shortest_shortest_path);

  return EXIT_SUCCESS;
}

