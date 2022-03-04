//
// Created by yalavrinenko on 04.03.2022.
//

#ifndef CARTESIANPRODUCTS_CARTESIAN_PRODUCT_HPP
#define CARTESIANPRODUCTS_CARTESIAN_PRODUCT_HPP

#include <ranges>
#include <tuple>
#include <concepts>

namespace rng = std::ranges;

template<rng::view V1, rng::view V2>
requires rng::forward_range<V1> && rng::forward_range<V2>
class cartesian_product_view : public rng::view_interface<cartesian_product_view<V1, V2>> {
  struct iterator;
  struct sentinel;

public:
  cartesian_product_view() = default;

  cartesian_product_view(V1 master, V2 slave) : v1_{std::move(master)}, v2_{std::move(slave)} {}

  iterator begin() { return iterator{*this}; }
  sentinel end() { return sentinel{rng::end(v1_)}; }

private:
  V1 v1_;
  V2 v2_;
};

template<rng::view V1, rng::view V2>
requires rng::forward_range<V1> && rng::forward_range<V2>
struct cartesian_product_view<V1, V2>::sentinel {
  rng::sentinel_t<V1> sv1_;
};

template<class T>
concept tuple_type = requires(T v) {
  typename std::tuple_size<std::decay_t<T>>::type;
  {std::tuple_size_v<std::decay_t<T>>};
  {std::get<0>(v)};
};

namespace {
  template<typename T1, typename T2>
  requires(tuple_type<T1> && !tuple_type<T2>) static decltype(auto) join_values(T1 &&a, T2 &&b) {
    return std::tuple_cat(std::forward<T1>(a), std::tie(std::forward<T2>(b)));
  }

  template<typename T1, typename T2>
  requires(!tuple_type<T1> && tuple_type<T2>) static decltype(auto) join_values(T1 &&a, T2 &&b) {
    return std::tuple_cat(std::tie(std::forward<T1>(a)), std::forward<T2>(b));
  }

  template<typename T1, typename T2>
  requires(tuple_type<T1> &&tuple_type<T2>) static decltype(auto) join_values(T1 &&a, T2 &&b) {
    return std::tuple_cat(std::forward<T1>(a), std::forward<T2>(b));
  }

  template<typename T1, typename T2>
  requires(!tuple_type<T1> && !tuple_type<T2>) static decltype(auto) join_values(T1 &&a, T2 &&b) {
    return std::tuple_cat(std::tie(a), std::tie(b));
  }
}

template<rng::view V1, rng::view V2>
requires rng::forward_range<V1> && rng::forward_range<V2>
struct cartesian_product_view<V1, V2>::iterator {
private:
  cartesian_product_view<V1, V2> *base_ = nullptr;
  rng::iterator_t<V1> iter_master_;
  rng::iterator_t<V2> iter_slave_;

public:
  using value_type = decltype(::join_values(*iter_master_, *iter_slave_));
  using difference_type = std::ptrdiff_t;
  using reference = decltype(::join_values(std::declval<rng::range_reference_t<V1>>(), std::declval<rng::range_reference_t<V2>>()));

  iterator() = default;
  explicit iterator(cartesian_product_view &cprod) : base_{&cprod}, iter_master_{rng::begin(base_->v1_)}, iter_slave_{rng::begin(base_->v2_)} {}

  reference operator*() const {
    //return unwrap_tuple(std::make_tuple(*iter_master_, *iter_slave_));
    return ::join_values(*iter_master_, *iter_slave_);
  }

  iterator &operator++() {
    ++iter_slave_;
    if (iter_slave_ == rng::end(base_->v2_)) {
      iter_slave_ = rng::begin(base_->v2_);
      ++iter_master_;
    }
    return *this;
  }

  iterator operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  bool operator==(iterator const &lhs) const = default;
  bool operator==(sentinel const &lhs) const { return (iter_master_ == lhs.sv1_); }
};

namespace std::ranges::views {

  struct cartesian_product_fn {
    template<rng::viewable_range R1, rng::viewable_range R2>
    auto operator() (R1 &&master, R2 &&slave) const{
      return cartesian_product_view<rng::views::all_t<R1>, rng::views::all_t<R2>>(rng::views::all(std::forward<R1>(master)),
                                                                                  rng::views::all(std::forward<R2>(slave)));
    }

    template<rng::viewable_range R1, rng::viewable_range ... Rs>
    auto operator() (R1 &&master, Rs&& ... slaves) const {
      auto slave = this->template operator()(slaves...);
      return cartesian_product_view<rng::views::all_t<R1>, rng::views::all_t<decltype(slave)>>(rng::views::all(std::forward<R1>(master)),
                                                                                               rng::views::all(std::move(slave)));
    }
  };

  inline constexpr auto cartesian_product = cartesian_product_fn{};
}



#endif//CARTESIANPRODUCTS_CARTESIAN_PRODUCT_HPP
