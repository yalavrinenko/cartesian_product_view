#include <concepts>
#include <fmt/core.h>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>
#include "cartesian_product.hpp"

int main() {
  namespace vw = std::ranges::views;
  auto v1 = std::vector{1, 2, 3};
  auto v2 = std::vector{-1, -2};
  auto v3 = std::vector{10, 11, 12};
  //
  auto cw = vw::cartesian_product(v1, v2, v3);// | vw::filter([](auto &e) { auto [e1, e2] = e; return e1 == 2; });
  //auto cw = vw::cartesian_product(dcw, v3);
  //auto cw = vw::cartesian_product(vw::iota(0u, 10u) | vw::common, vw::iota(0u, 5u) | vw::common);
  for (auto e: cw) {
    //fmt::print("({}, {}, {})\n", std::get<0>(std::get<0>(e)), std::get<1>(std::get<0>(e)), std::get<1>(e));
    fmt::print("({}, {}, {})\n", std::get<0>(e), std::get<1>(e), std::get<2>(e));
  }
  return 0;
}
