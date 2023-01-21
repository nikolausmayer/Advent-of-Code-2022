
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
#include <bitset>
#include <compare>
#include <fstream>
#include <list>
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

  std::vector<std::string> map;
  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    map.push_back(std::move(line));
  }

  const auto W = (int)map[0].size() - 2;
  const auto H = (int)map.size() - 2;

  std::vector<std::vector<bool>> rows_l, rows_r, cols_u, cols_d;
  for (auto y = 0; y < H; ++y) {
    std::vector<bool> row_l, row_r;
    for (auto x = 0; x < W; ++x) {
      row_l.push_back(map[y+1][x+1] == '<');
      row_r.push_back(map[y+1][x+1] == '>');
    }
    rows_l.push_back(std::move(row_l));
    rows_r.push_back(std::move(row_r));
  }
  for (auto x = 0; x < W; ++x) {
    std::vector<bool> col_u, col_d;
    for (auto y = 0; y < H; ++y) {
      col_u.push_back(map[y+1][x+1] == '^');
      col_d.push_back(map[y+1][x+1] == 'v');
    }
    cols_u.push_back(std::move(col_u));
    cols_d.push_back(std::move(col_d));
  }

  auto is_u = [&](int x, int y, int t) { return cols_u[x][(y+t)      %H]; };
  auto is_d = [&](int x, int y, int t) { return cols_d[x][(y+H-(t%H))%H]; };
  auto is_l = [&](int x, int y, int t) { return rows_l[y][(x+t)      %W]; };
  auto is_r = [&](int x, int y, int t) { return rows_r[y][(x+W-(t%W))%W]; };
  auto is_any = [&](int x, int y, int t) { 
    return is_u(x,y,t) or is_d(x,y,t) or is_l(x,y,t) or is_r(x,y,t);
  };

  //auto print_map = [&](int t) {
  //  std::ostringstream oss;
  //  for (auto x = 0; x < W; ++x)
  //    oss << '#';
  //  fmt::print("#.{}\n", oss.str());
  //  for (auto y = 0; y < H; ++y) {
  //    oss.str(""); oss << '#';
  //    for (auto x = 0; x < W; ++x)
  //      oss << (is_any(x,y,t) ? 'x' : '.');
  //    fmt::print("{}#\n", oss.str());
  //  }
  //  oss.str("");
  //  for (auto x = 0; x < W; ++x)
  //    oss << '#';
  //  fmt::print("{}.#\n\n", oss.str());
  //};

  struct State {
    int x, y, time, heuristic;
  };
  auto cmp = [&](const auto& a, const auto& b) {
    return (a.time + a.heuristic) > (b.time + b.heuristic);
  };
  std::set<std::array<int,3>> visited;
  std::priority_queue<State, std::vector<State>, decltype(cmp)> queue(cmp);
  int total_time = 0;

  /// First trip to goal

  queue.push({0,-1,0,1});
  while (queue.size()) {
    const auto next = queue.top();
    queue.pop();
    if (next.heuristic == 0) {
      fmt::print("Reached goal after {} minutes?\n", next.time);
      total_time = next.time;
      break;
    }
    if (visited.contains({next.x, next.y, next.time}))
      continue;
    visited.insert({next.x, next.y, next.time});

    for (auto [dx,dy]: std::vector<std::array<int,2>>{{0,0},
                                                      {0,-1},{0,1},
                                                      {-1,0},{1,0}}) {
      const auto nx = next.x + dx;
      const auto ny = next.y + dy;
      if ((nx >= 0 and nx < W and
           ny >= 0 and ny < H and
           not is_any(nx, ny, next.time + 1)) or
          (nx == 0 and ny == -1) or
          (nx == W - 1 and ny == H))
        queue.push({nx, ny, next.time + 1, (W - 1 - nx) + (H - ny)});
    }
  }

  /// Oh no, forgot the snacks! Go back to the start...

  while (queue.size()) queue.pop();
  visited.clear();
  queue.push({W-1,H,total_time,1});
  while (queue.size()) {
    const auto next = queue.top();
    queue.pop();
    if (next.heuristic == 0) {
      fmt::print("Reached snacks after {} minutes\n", next.time);
      total_time = next.time;
      break;
    }
    if (visited.contains({next.x, next.y, next.time}))
      continue;
    visited.insert({next.x, next.y, next.time});

    for (auto [dx,dy]: std::vector<std::array<int,2>>{{0,0},
                                                      {0,-1},{0,1},
                                                      {-1,0},{1,0}}) {
      const auto nx = next.x + dx;
      const auto ny = next.y + dy;
      if ((nx >= 0 and nx < W and
           ny >= 0 and ny < H and
           not is_any(nx, ny, next.time + 1)) or
          (nx == W - 1 and ny == H) or
          (nx == 0 and ny == -1))
        queue.push({nx, ny, next.time + 1, nx + (ny + 1)});
    }
  }

  /// Ok, got the snacks. Now back to the goal!

  while (queue.size()) queue.pop();
  visited.clear();
  queue.push({0,-1,total_time,1});
  while (queue.size()) {
    const auto next = queue.top();
    queue.pop();
    if (next.heuristic == 0) {
      fmt::print("Reached goal after {} minutes!\n", next.time);
      break;
    }
    if (visited.contains({next.x, next.y, next.time}))
      continue;
    visited.insert({next.x, next.y, next.time});

    for (auto [dx,dy]: std::vector<std::array<int,2>>{{0,0},
                                                      {0,-1},{0,1},
                                                      {-1,0},{1,0}}) {
      const auto nx = next.x + dx;
      const auto ny = next.y + dy;
      if ((nx >= 0 and nx < W and
           ny >= 0 and ny < H and
           not is_any(nx, ny, next.time + 1)) or
          (nx == 0 and ny == -1) or
          (nx == W - 1 and ny == H))
        queue.push({nx, ny, next.time + 1, (W - 1 - nx) + (H - ny)});
    }
  }

  return EXIT_SUCCESS;
}

