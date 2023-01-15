
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
      if (line.substr(0,4) == "root") {
        m.compute = [=,&monkeys]() {
          if (monkeys[m1]() == monkeys[m2]()) {
            //fmt::print("match\n");
            return 0l;
          } else {
            //fmt::print("{} ! = {}\n", monkeys[m1](), monkeys[m2]());
            return monkeys[m1]();
          }
        };
      } else {
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
      }
    } else {
      const auto tmp = std::stoi(line.substr(6));
      m.compute = [=]() { return tmp; };
    }
    monkeys[line.substr(0,4)] = std::move(m);
  }

  const auto target = 49'624'267'175'787l;
  long lower = -1;
  long upper = 1;
  long i     = 1;
  for (;;) {
    for (auto& [_,m]: monkeys) 
      m.computed = false;
    monkeys["humn"].compute = [=]() { return i; };
    const auto res = monkeys["root"].compute();
    //fmt::print("{}|{}  {} -> {}\n", lower, upper, i, res);
    if (res == 0) {
      //fmt::print("Human yells: {}\n", i);
      break;
    }
    if (lower == -1) {
      if (res > target) {
        i *= 2;
      } else {
        lower = i/2;
        upper = i;
        i = (lower + upper)/2; 
      }
    } else {
      if (res > target) {
        lower = i;
        i = (lower + upper)/2;
      } else {
        upper = i-1;
        i = (lower + upper)/2;
      }
    }
  }

  return EXIT_SUCCESS;
}

