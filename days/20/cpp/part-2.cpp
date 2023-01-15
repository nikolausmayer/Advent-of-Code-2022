
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

  /// Build chain
  struct Node {
    long val;
    Node* prev{nullptr};
    Node* next{nullptr};
  };
  std::vector<std::shared_ptr<Node>> nodes;
  for (long x; not infile.eof() and infile >> x;) {
    nodes.push_back(std::make_shared<Node>(x * 811589153l));
    if (nodes.size() > 1) {
      nodes[nodes.size()-2]->next = nodes.back().get();
      nodes.back()->prev = nodes[nodes.size()-2].get();
    }
  }
  /// Make chain circular
  nodes.front()->prev = nodes.back().get();
  nodes.back()->next  = nodes.front().get();

  //for (const auto& n: nodes)
  //  fmt::print("{} <- ({}) -> {}\n", n->prev->val, n->val, n->next->val);
  //fmt::print("\n");

  /// Move elements around
  for (auto round = 0; round < 10; ++round) {
    for (auto& n: nodes) {
      //fmt::print("(({}))\n", n->val);
      if (n->val % ((long)nodes.size() - 1) == 0)
        continue;
      auto iter = n->val % ((long)nodes.size() - 1l);
      auto* old_prev = n->prev;
      auto* old_next = n->next;
      auto* new_prev = n->prev;
      auto* new_next = n->next;
      if (n->val < 0) {
        for (auto i = iter; i < 0; ++i)
          new_prev = new_prev->prev;
        new_next = new_prev->next;
      } else {
        for (auto i = iter; i > 0; --i)
          new_next = new_next->next;
        new_prev = new_next->prev;
      }
      old_prev->next = old_next;
      old_next->prev = old_prev;
      n->prev = new_prev;
      n->next = new_next;
      new_prev->next = n.get();
      new_next->prev = n.get();
    }
  }


  auto* probe = nodes[0].get();
  while (probe->val != 0)
    probe = probe->next;
  auto result = 0l;
  for (auto i = 0; i < 3; ++i) {
    for (auto j = 0; j < 1000; ++j)
      probe = probe->next;
    result += probe->val;
    //fmt::print("{}\n", probe->val);
  }
  fmt::print("Sum of grove coordinate numbers: {}\n", result);

  return EXIT_SUCCESS;
}

