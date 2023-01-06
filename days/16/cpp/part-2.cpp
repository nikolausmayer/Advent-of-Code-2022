
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
  for (auto time_left = 26; time_left > 0; --time_left) {
    auto newstate = state;
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

  struct BTrie {
    int val;
    std::uint64_t mask;
    BTrie *yes, *no;
  };
  BTrie root{0, 0, nullptr, nullptr};
  const auto maxn = loc_id.size();
  for (const auto& [location, states_map]: state) {
    for (const auto& [opened_valves, score]: states_map) {
      auto* probe = &root;
      probe->val = std::max(probe->val, score);
      for (auto shift = 0ul; shift < maxn; ++shift) {
        if ((opened_valves & (1ul << shift)) != 0) {
          if (probe->yes == nullptr)
            probe->yes = new BTrie{score, opened_valves, nullptr, nullptr};
          probe = probe->yes;
        } else {
          if (probe->no == nullptr)
            probe->no = new BTrie{score, opened_valves, nullptr, nullptr};
          probe = probe->no;
        }
        probe->val = std::max(probe->val, score);
      }
    }
  }
  auto max_score = 0;
  std::function<void(BTrie const*, std::uint64_t, std::size_t, int)> recurse;
  recurse = [&](BTrie const* probe, std::uint64_t v, std::size_t i, int score) {
    if (probe == nullptr or score + probe->val <= max_score) {
      return;
    }
    if (i == maxn) {
      max_score = std::max(max_score, score + probe->val);
      //std::cout << std::bitset<64>(probe->mask) << " (" << probe->val << ") +\n"
      //          << std::bitset<64>(v)           << " (" << score << ") -> "
      //          << score + probe->val << "\n\n";
      return;
    }
    if ((v & (1ul << i)) == 0) {
      recurse(probe->yes, v, i+1, score);
    }
    recurse(probe->no, v, i+1, score);
  };
  for (const auto& [location, states_map]: state) {
    for (const auto& [opened_valves, score]: states_map) {
      recurse(&root, opened_valves, 0, score);
    }
  }
  fmt::print("Maximum pressure relieve: {}\n", max_score);

  /// Cleanup
  std::function<void(BTrie*)> del;
  del = [&](BTrie* n) {
    if (n == nullptr)
      return;
    del(n->yes);
    del(n->no);
    delete n;
  };
  del(root.yes);  
  del(root.no);  

  return EXIT_SUCCESS;
}

