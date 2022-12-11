
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

struct Monkey
{
  std::size_t id = 0;
  std::list<int> items{};
  std::function<int(int)> op{};
  std::function<bool(int)> test{};
  std::size_t test_succeeded_target = 0;
  std::size_t test_failed_target    = 0;
  std::size_t inspections_count     = 0;
};

int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  const std::regex RE_number("\\d+");

  std::vector<Monkey> monkeys;

  for (std::string line;;) {
    std::getline(infile, line);  // Monkey X:
    if (infile.eof())
      break;

    monkeys.push_back({monkeys.size()});
    auto& monkey = monkeys.back();

    std::getline(infile, line);  //   Starting items: X[, Y]
    for (auto match = std::sregex_iterator(line.begin(), line.end(), RE_number);
         match != std::sregex_iterator();
         ++match) {
      monkey.items.push_back(std::stoi((*match).str()));
    }

    std::getline(infile, line);  //   Operation: new = old X Y
    if (line[25] == 'o') {
      if (line[23] == '+')
        monkey.op = [](int v) { return v + v; };
      else
        monkey.op = [](int v) { return v * v; };
    } else {
      const auto opnum = std::stoi(line.substr(25));
      if (line[23] == '+')
        monkey.op = [opnum](int v) { return v + opnum; };
      else
        monkey.op = [opnum](int v) { return v * opnum; };
    }

    std::getline(infile, line);  //   Test: divisible by X
    const auto testnum = std::stoi(line.substr(21));
    monkey.test = [testnum](int v) { return (v % testnum) == 0; };

    std::getline(infile, line);  //     If true: throw to monkey X
    monkey.test_succeeded_target = std::stoi(line.substr(29));

    std::getline(infile, line);  //     If false: throw to monkey X
    monkey.test_failed_target = std::stoi(line.substr(30));

    std::getline(infile, line);  // (empty line)
  }

  for (auto round = 0; round < 20; ++round) {
    fmt::print("\nRound {}:\n", round);
    for (auto& monkey: monkeys) {
      fmt::print("  Monkey {}:\n", monkey.id);
      while (monkey.items.size()) {
        ++monkey.inspections_count;

        auto worry_level = monkey.items.front();
        monkey.items.pop_front();
        fmt::print("\t{}", worry_level);
        worry_level = monkey.op(worry_level);
        fmt::print("\t{}", worry_level);
        worry_level /= 3;
        fmt::print("\t{}", worry_level);
        if (monkey.test(worry_level)) {
          fmt::print("\t-> {}", monkey.test_succeeded_target);
          monkeys[monkey.test_succeeded_target].items.push_back(worry_level);
        } else {
          fmt::print("\t-> {}", monkey.test_failed_target);
          monkeys[monkey.test_failed_target].items.push_back(worry_level);
        }
        fmt::print("\n");
      }
    }

    for (auto& monkey: monkeys) {
      fmt::print("Monkey {}:", monkey.id);
      for (auto v: monkey.items)
        fmt::print(" {}", v);
      fmt::print("\n");
    }
  }

  fmt::print("\nInspections:\n");
  for (const auto& monkey: monkeys)
    fmt::print("  Monkey {}: {}\n", monkey.id, monkey.inspections_count);

  std::vector<int> inspections;
  for (const auto& monkey: monkeys)
    inspections.push_back(monkey.inspections_count);
  std::ranges::sort(inspections, std::greater<int>());

  fmt::print("Monkey business: {}\n", inspections[0] * inspections[1]);

  return EXIT_SUCCESS;
}

