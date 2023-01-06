
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

  std::map<std::string, int> flow_rates;
  std::map<std::string, std::vector<std::string>> tunnels;
  std::map<std::string, std::uint64_t> loc_id;

  for (std::string line;;) {
    if (infile.peek() == std::ifstream::traits_type::eof())
      break;
    infile.ignore(6);      /// "Valve "
    std::string valve_name;
    infile >> valve_name;  /// "AA"
    loc_id[valve_name] = (1ul << loc_id.size());
    infile.ignore(15);     /// " has flow rate="
    infile >> flow_rates[valve_name];
    infile.ignore(8);      /// "; tunnel"
    if (infile.peek() == 's') {
      infile.ignore(16);   /// "s lead to valves"
    } else {
      infile.ignore(15);   /// " leads to valve"
    }
    //fmt::print("{} {} ->", valve_name, flow_rates[valve_name]);
    for (;;) {
      std::string tunnel;
      infile >> tunnel;    //// " AA," -> "AA,"
      tunnels[valve_name].push_back(tunnel.substr(0,2));  /// "AA," -> "AA"
      //fmt::print(" {}", tunnel.substr(0,2));
      if (infile.peek() == '\n')
        break;
    }
    //fmt::print("\n");
    infile.get();
  }

  std::map<std::uint64_t, int> id_flow_rates;
  for (const auto& [k,v]: flow_rates)
    id_flow_rates[loc_id[k]] = v;
  std::map<std::uint64_t, std::vector<std::uint64_t>> id_tunnels;
  for (const auto& [k,v]: tunnels) {
    std::vector<std::uint64_t> idt;
    for (const auto& t: v)
      idt.push_back(loc_id[t]);
    id_tunnels[loc_id[k]] = idt;
  }

  /// Dynamic programming
  std::map<std::uint64_t, std::map<std::uint64_t, int>> state;
  state[loc_id["AA"]][0] = 0;
  for (auto time_left = 30; time_left > 0; --time_left) {
    decltype(state) newstate;
    for (const auto& [location, states_map]: state) {
      for (const auto& [opened_valves, score]: states_map) {
        if (id_flow_rates[location] != 0 and
            (opened_valves & location) == 0ul) {
          auto new_opened_valves = (opened_valves | location);
          auto new_score = score + id_flow_rates[location] * (time_left - 1);
          newstate[location][new_opened_valves] = 
              std::max(newstate[location][new_opened_valves],
                       new_score);
        }
        for (const auto& tunnel_to: id_tunnels[location]) {
          newstate[tunnel_to][opened_valves] = 
              std::max(newstate[tunnel_to][opened_valves],
                       score);
        }
      }
    }
    state = newstate;
  }

  auto max_score = 0;
  for (const auto& [location, states_map]: state)
    for (const auto& [opened_valves, score]: states_map)
      max_score = std::max(max_score, score);
  fmt::print("Maximum pressure relieve: {}\n", max_score);

  return EXIT_SUCCESS;
}

