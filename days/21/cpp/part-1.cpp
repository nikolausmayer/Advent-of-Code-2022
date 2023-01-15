
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

  // root: pppw + sjmn
  // dbpl: 5
  struct Monkey {
    long result{0};
    bool computed{false};
    std::function<long()> compute;
    long operator()() { 
      if (not computed) {
        result = compute();
        computed = true;
      }
      return result; 
    }
  };
  std::map<std::string, Monkey> monkeys;

  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;

    Monkey m{};
    if (line.size() == 17) {
      const auto m1 = line.substr( 6, 4);
      const auto m2 = line.substr(13, 4);
      switch (line[11]) {
        case '+': 
          m.compute = [=,&monkeys](){return monkeys[m1]() + monkeys[m2]();}; 
          break;
        case '-': 
          m.compute = [=,&monkeys](){return monkeys[m1]() - monkeys[m2]();}; 
          break;
        case '*': 
          m.compute = [=,&monkeys](){return monkeys[m1]() * monkeys[m2]();}; 
          break;
        case '/': 
          m.compute = [=,&monkeys](){return monkeys[m1]() / monkeys[m2]();}; 
          break;
      }
    } else {
      const auto tmp = std::stoi(line.substr(6));
      m.compute = [=]() { return tmp; };
    }
    monkeys[line.substr(0,4)] = std::move(m);
  }

  fmt::print("Monkey 'root' yells: {}\n", monkeys["root"].compute());

  return EXIT_SUCCESS;
}

