
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


int main(int argc, char* argv[]) 
{
  if (argc < 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  typedef std::array<int,4> R;

  /// base vectors: x'=(+x,-y), y'=(+x,+y)
  std::vector<R> possible_location_rectangles;
  auto pos_dist_to_rect = [](int x, int y, int dist) {
    return std::array<int,4>{(x - dist) - y,
                             (x - dist) + y,
                             (x + dist) - y,
                             (x + dist) + y};
  };
  /// Over-estimate
  possible_location_rectangles.push_back(pos_dist_to_rect(2'000'000,
                                                          2'000'000,
                                                          4'000'000));

  auto sub_b_from_a = [](const R& rold, const R& rnew) {
    decltype(possible_location_rectangles) result;
    if (rold[2] < rnew[0] or
        rold[0] > rnew[2] or
        rold[3] < rnew[1] or
        rold[1] > rnew[3]) {
      result.push_back(rold);
    } else {
      const auto xl = (rold[0] < rnew[0]);
      const auto yl = (rold[1] < rnew[1]);
      const auto xu = (rold[2] > rnew[2]);
      const auto yu = (rold[3] > rnew[3]);

      const auto a = std::min(rold[0], rnew[0]);
      const auto b = std::max(rold[0], rnew[0]);
      const auto c = std::min(rold[2], rnew[2]);
      const auto d = std::max(rold[2], rnew[2]);
      const auto e = std::min(rold[1], rnew[1]);
      const auto f = std::max(rold[1], rnew[1]);
      const auto g = std::min(rold[3], rnew[3]);
      const auto h = std::max(rold[3], rnew[3]);

      ///  a   b     c   d
      /// e+---+-----+---+
      ///  | A |  B  | C |
      /// f+---+-----+---+
      ///  |   |     |   |
      ///  | D |     | E |
      ///  |   |     |   |
      /// g+---+-----+---+
      ///  | F |  G  | H |
      /// h+---+-----+---+

      if (xl and yl)  /// A
        result.push_back({a, e, b-1, f-1});
      if (       yl)  /// B
        result.push_back({b, e, c, f-1});
      if (xu and yl)  /// C
        result.push_back({c+1, e, d, f-1});
      if (xl       )  /// D
        result.push_back({a, f, b-1, g});
      if (xu       )  /// E
        result.push_back({c+1, f, d, g});
      if (xl and yu)  /// F
        result.push_back({a, g+1, b-1, h});
      if (       yu)  /// G
        result.push_back({b, g+1, c, h});
      if (xu and yu)  /// H
        result.push_back({c+1, g+1, d, h});
    }
    return result;
  };

  //auto printmap = [&]() {
  //  std::map<std::array<int,2>,char> poss;
  //  for (auto ri = 0u; ri < possible_location_rectangles.size(); ++ri) {
  //    const auto& r = possible_location_rectangles[ri];
  //    for (auto ry = r[1]; ry <= r[3]; ++ry) {
  //      for (auto rx = r[0]; rx <= r[2]; ++rx) {
  //        const auto x = ( rx + ry) / 2;
  //        const auto y = (-rx + ry) / 2;
  //        poss[{x,y}] = 'a'+ri;
  //      }
  //    }
  //  }
  //  poss[{0,0}] = '0';
  //  for (auto y = -12; y <= 22; ++y) {
  //    for (auto x = -12; x <= 22; ++x) {
  //      if (poss.count({x,y})) {
  //        fmt::print("{}", std::string{poss[{x,y}]});
  //      } else {
  //        fmt::print(".");
  //      }
  //    }
  //    fmt::print("\n");
  //  }
  //};

  for (;;) {
    if (infile.peek() == std::ifstream::traits_type::eof())
      break;
    int sx, sy, bx, by;
    infile.ignore(12);
    infile >> sx;
    infile.ignore(4);
    infile >> sy;
    infile.ignore(25);
    infile >> bx;
    infile.ignore(4);
    infile >> by;
    infile.get();

    const auto manhattan = std::abs(sx - bx) + std::abs(sy - by);

    decltype(possible_location_rectangles) new_rects;
    const auto nrect = pos_dist_to_rect(sx, sy, manhattan);
    for (const auto& rect: possible_location_rectangles) {
      for (const auto fragment: sub_b_from_a(rect, nrect)) {
        new_rects.push_back(fragment);
      }
    }
    possible_location_rectangles = new_rects;
    //printmap();
  }


  for (auto ri = 0u; ri < possible_location_rectangles.size(); ++ri) {
    const auto& r = possible_location_rectangles[ri];
    if (r[0] == r[2] and r[1] == r[3]) {
      const auto x = ( r[0] + r[1]) / 2;
      const auto y = (-r[0] + r[1]) / 2;
      fmt::print("Solution tuning frequency: {}\n", x * 4'000'000l + y);
      break;
    }
  }

  return EXIT_SUCCESS;
}

