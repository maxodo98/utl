#pragma once

#include <utility>

#include "utl/clear_t.h"
#include "utl/pipes/make_range.h"

namespace utl {

template <typename Range, typename TakeWhile>
struct take_while_range : public clear_t<Range> {
  using parent_t = clear_t<Range>;

  template <typename R>
  take_while_range(R&& r, TakeWhile&& take_while)
      : parent_t(std::forward<parent_t>(r)),
        take_while_(std::forward<TakeWhile>(take_while)) {}

  template <typename It>
  bool valid(It& it) {
    return take_while_.fn_(this->read(it));
  }

  TakeWhile take_while_;
};

template <typename Fn>
struct take_while_t {
  take_while_t(Fn&& f) : fn_(std::forward<Fn>(f)) {}

  template <typename T>
  friend auto operator|(T&& r, take_while_t&& f) {
    return take_while_range<decltype(make_range(r)), take_while_t>(
        make_range(std::forward<T>(r)), std::move(f));
  }

  Fn fn_;
};

template <typename TakeWhile>
take_while_t<TakeWhile> take_while(TakeWhile&& f) {
  return take_while_t<TakeWhile>(std::forward<TakeWhile>(f));
}

template <typename Range, typename TakeWhile>
struct is_range<take_while_range<Range, TakeWhile>> : std::true_type {};

}  // namespace utl