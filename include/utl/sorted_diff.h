#pragma once

#include <string_view>
#include <utility>

namespace utl {

enum class op : std::uint8_t { kAdd, kDel };

constexpr std::string_view to_str(op const o) {
  switch (o) {
    case op::kAdd: return "add";
    case op::kDel: return "del";
  }
  std::unreachable();
}

template <typename It, typename Lt, typename Eq, typename Fn>
void sorted_diff(It a, It const a_end, It b, It const b_end, Lt&& cmp,
                 Eq&& deep_eq, Fn&& fn) {
  while (a != a_end || b != b_end) {
    if (a == a_end) {
      fn(op::kAdd, *b);
      ++b;
    } else if (b == b_end) {
      fn(op::kDel, *a);
      ++a;
    } else if (cmp(*a, *b)) {
      fn(op::kDel, *a);
      ++a;
    } else if (cmp(*b, *a)) {
      fn(op::kAdd, *b);
      ++b;
    } else {
      if (!deep_eq(*a, *b)) {
        fn(*a, *b);
      }
      ++a;
      ++b;
    }
  }
}

template <typename Collection, typename Lt, typename Eq, typename Fn>
void sorted_diff(Collection const& a, Collection const& b, Lt&& cmp,
                 Eq&& deep_eq, Fn&& fn) {
  sorted_diff(begin(a), end(a), begin(b), end(b), std::forward<Lt>(cmp),
              std::forward<Eq>(deep_eq), std::forward<Fn>(fn));
}

}  // namespace utl