
#include <cassert>
#include <cstdint>
#include <algorithm>
#include <array>
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
#include <fmt/ostream.h>

//struct Node;
//template<> struct fmt::formatter<Node> : ostream_formatter {};
struct Node 
{
  bool is_value = false;
  int value = 0;
  std::vector<std::unique_ptr<Node>> children;

  Node()
  { }

  Node(int value)
  : is_value{true},
    value{value}
  { }

  Node(std::istream& is)
  {
    std::stack<Node*> tree;
    tree.push(this);
    while (is.peek() != '\n') {
      const char c = is.peek();
      switch (c) {
        case '[': { 
          is.get(); 
          tree.top()->children.push_back(std::make_unique<Node>()); 
          tree.push(tree.top()->children.back().get());
          break; 
        }
        case ']': {
          is.get();
          tree.pop();
          break;
        }
        case ',': {
          is.get();
          break;
        }
        default: {
          int n;
          is >> n;
          tree.top()->children.push_back(std::make_unique<Node>(n)); 
        }
      }
    }
  }

  bool operator==(const Node& other) const
  {
    return (*this <=> other) == std::strong_ordering::equal;
  }

  std::strong_ordering operator<=>(const Node& other) const
  {
    //fmt::print("{} cmp {}\n", *this, other);
    if (is_value and other.is_value) {
      /// Both numbers
      //fmt::print("val {} <-> {}\n", value, other.value);
      return value <=> other.value;
    } else if ((not is_value) and (not other.is_value)) {
      /// Both lists
      for (auto i = 0u; i < std::max(children.size(), other.children.size()); ++i) {
        if (i >= children.size()) {
          //fmt::print("{} less children than {}\n", *this, other);
          return std::strong_ordering::less;
        }
        if (i >= other.children.size()) {
          //fmt::print("{} more children than {}\n", *this, other);
          return std::strong_ordering::greater;
        }
        if (*children[i] != *other.children[i]) {
          //fmt::print("unequal children {} {}\n", *children[i], *other.children[i]);
          return *children[i] <=> *other.children[i];
        }
      }
      return std::strong_ordering::equal;
    } else if (is_value) {
      //fmt::print("convert this {} to list\n", value);
      Node tmp{};
      tmp.children.push_back(std::make_unique<Node>(value));
      return tmp <=> other;
    } else {
      //fmt::print("convert other {} to list\n", other.value);
      Node tmp{};
      tmp.children.push_back(std::make_unique<Node>(other.value));
      return *this <=> tmp;
    }
  }

  //friend std::ostream& operator<<(std::ostream& os, const Node& a)
  //{
  //  if (a.is_value) {
  //    os << a.value;
  //  } else {
  //    os << "[ ";
  //    for (const auto& c: a.children) {
  //      os << *c << " ";
  //    }
  //    os << "]";
  //  }
  //  return os;
  //}
};

int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  auto first_divider  = 0;
  auto second_divider = 0;
  std::vector<std::unique_ptr<Node>> all_packets{};
  std::vector<std::unique_ptr<Node>> dividers{};
  for (const auto divider: {"[[2]]\n", "[[6]]\n"}) {
    std::istringstream iss{divider};
    all_packets.push_back(std::make_unique<Node>(iss));
  }
  for (const auto divider: {"[[2]]\n", "[[6]]\n"}) {
    std::istringstream iss{divider};
    dividers.push_back(std::make_unique<Node>(iss));
  }

  for (;;) {
    while (infile.peek() == '\n')
      infile.get();
    if (infile.eof())
      break;
    all_packets.push_back(std::make_unique<Node>(infile));
  }
  std::ranges::sort(all_packets, 
                    [](const auto& a, const auto& b) {
                      return *a < *b;
                    });
  for (auto i = 0u; i < all_packets.size(); ++i) {
    if (*all_packets[i] == *dividers[0])
      first_divider = i + 1;
    if (*all_packets[i] == *dividers[1])
      second_divider = i + 1;
  }

  fmt::print("Decoder key: {} * {} = {}\n", first_divider, second_divider, first_divider * second_divider);

  return EXIT_SUCCESS;
}

