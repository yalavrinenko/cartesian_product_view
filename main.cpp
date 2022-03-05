#include <concepts>
#include <fmt/core.h>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>
#include <algorithm>
#include "cartesian_product.hpp"

int main() {
  namespace vw = std::ranges::views;
  auto v1 = std::vector{1, 2, 3};
  auto v2 = std::vector{-1, -2};
  auto v3 = std::vector{10, 11, 12};
  //
//  auto cw = vw::cartesian_product(v1 | std::views::take(2), v2, v3 | vw::take(1));
  auto cw = vw::cartesian_product(v1, v2, v3);// | vw::filter([](auto &e) { auto [e1, e2] = e; return e1 == 2; });
//  auto cw = vw::cartesian_product(vw::iota(0, 5), vw::iota(1, 10));
  for (auto e: cw) {
    //fmt::print("({}, {}, {})\n", std::get<0>(std::get<0>(e)), std::get<1>(std::get<0>(e)), std::get<1>(e));
    fmt::print("({}, {}, {})\n", std::get<0>(e), std::get<1>(e), std::get<0>(e));
    std::get<0>(e) += 1;
    std::get<1>(e) -= 1;
    std::get<2>(e) += 10;
  }

  auto dump = [](auto const& e) {
    rng::for_each(e, [](auto const& v) { fmt::print("{} ", v); });
    fmt::print("\n");
  };

  dump(v1);
  dump(v2);
  dump(v3);
  return 0;
}
