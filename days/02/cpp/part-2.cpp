
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fmt/core.h>

int main(int argc, char* argv[]) 
{
  if (argc < 2) {
    return EXIT_FAILURE;
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    return EXIT_FAILURE;
  }

  int score = 0;
  for (char a, b;;) {
    infile >> a >> b;
    if (infile.eof()) {
      break;
    }
    const int p1 = a - 'A';
    const int p2 = (b == 'X' ? "ZXY"[p1] :       // need to lose (A->Z etc)
                    b == 'Y' ? "XYZ"[p1] :       // need to draw (A->X etc)
                               "YZX"[p1]) - 'X'; // need to win  (A->Y etc)
    score +=   (p1 == p2) * 3                  // draw
             + (((p2 - p1 + 3) % 3) == 1) * 6  // win ("difference is 1")
             + p2 + 1;                         // points for chosen symbol
  }

  fmt::print("{}\n", score);

  return EXIT_SUCCESS;
}

