
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

  const std::map<char, int> snafu_to_decimal{
    {'=', -2},
    {'-', -1},
    {'0',  0},
    {'1',  1},
    {'2',  2}
  };

  auto sum = 0l;

  std::vector<std::string> map;
  for (std::string line;;) {
    std::getline(infile, line);
    if (infile.eof())
      break;
    auto number = 0l;
    for (auto c: line)
      number = number * 5 + snafu_to_decimal.at(c);
    sum += number;
    //fmt::print("{}\t{}\n", number, sum);
  }

  const std::map<int, char> decimal_to_snafu{
    {-2, '='},
    {-1, '-'},
    { 0, '0'},
    { 1, '1'},
    { 2, '2'}
  };

  std::ostringstream snafu;
  auto scale = 1l;
  while (std::abs(sum) > 2*scale)
    scale *= 5;
  while (scale > 0) {
    //fmt::print("{} -> ", sum);
    int digit = 0;
    while (sum > (scale-1)/2) {
      ++digit;
      sum -= scale;
      //fmt::print(" (-{}) ", scale);
    }
    while (sum < -(scale-1)/2) {
      --digit;
      sum += scale;
      //fmt::print(" (+{}) ", scale);
    }
    snafu << decimal_to_snafu.at(digit);
    //fmt::print(" -> {} -> \"{}\"\n", sum, snafu.str());
    scale /= 5;
  }

  fmt::print("SNAFU number is {}\n", snafu.str());


  return EXIT_SUCCESS;
}

