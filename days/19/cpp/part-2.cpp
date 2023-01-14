
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

  typedef std::array<int, 4> A4;
  struct Blueprint {
    int id;
    std::array<A4, 4> recipes;
  };

  std::vector<Blueprint> blueprints;

  for (;;) {
    if (infile.peek() == std::ifstream::traits_type::eof())
      break;

    //Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
    Blueprint next_blueprint{};
    infile.ignore(10);
    infile >> next_blueprint.id;
    infile.ignore(23);
    infile >> next_blueprint.recipes[0][0];
    infile.ignore(28);
    infile >> next_blueprint.recipes[1][0];
    infile.ignore(32);
    infile >> next_blueprint.recipes[2][0];
    infile.ignore(9);
    infile >> next_blueprint.recipes[2][1];
    infile.ignore(29);
    infile >> next_blueprint.recipes[3][0];
    infile.ignore(9);
    infile >> next_blueprint.recipes[3][2];
    infile.ignore(11);
    blueprints.push_back(next_blueprint);

    if (blueprints.size() == 3)
      break;
  }

  std::uint32_t result = 1;
  for (const auto& blueprint: blueprints) {
    struct State {
      std::uint8_t next_plan = 0;
      std::uint8_t time_left = 32;
      std::array<std::uint8_t,4> resources{0,0,0,0};
      std::array<std::uint8_t,4> robots{1,0,0,0};
    };
    std::uint8_t max_geodes = 0;
    std::stack<State> stack;
    stack.push(State{0});
    stack.push(State{1});
    while (stack.size()) {
      const auto current = stack.top();
      stack.pop();
      if (current.time_left == 0) {
        //if (current.resources[3] > max_geodes)
        //  fmt::print("{}\n", current.resources[3]);
        max_geodes = std::max(max_geodes, current.resources[3]);
        continue;
      }
      /// Meet the world's dumbest heuristic...
      if ((current.resources[3] +
           current.robots[3] * current.time_left +
           (current.time_left * current.time_left) / 2) < max_geodes) {
        continue;
      }
      bool can_build = true;
      for (auto resource = 0; resource < 4; ++resource) {
        can_build &= (current.resources[resource] >=
                      blueprint.recipes[current.next_plan][resource]);
      }
      if (can_build) {
        auto next = current;
        /// Consume
        for (auto resource = 0; resource < 4; ++resource) {
          next.resources[resource] -= blueprint.recipes[next.next_plan][resource];
        }
        /// Collect
        for (auto resource = 0; resource < 4; ++resource) {
          next.resources[resource] += next.robots[resource];
        }
        /// Construct
        ++next.robots[next.next_plan];
        --next.time_left;

        for (auto robot = 0; robot < 4; ++robot) {
          auto n2 = next;
          n2.next_plan = robot;
          stack.push(n2);
        }
      } else {
        auto next = current;
        for (auto resource = 0; resource < 4; ++resource) {
          next.resources[resource] += next.robots[resource];
        }
        --next.time_left;
        stack.push(next);
      }
    }
    fmt::print("{} -> {}\n", blueprint.id, max_geodes);
    result *= max_geodes;
  }
  fmt::print("Product of achievable geodes: {}\n", result);

  return EXIT_SUCCESS;
}

