#pragma once

#include <initializer_list>
#include <utility>

namespace utl {

template <typename BeginIt, typename EndIt>
struct range {
  using begin_t = BeginIt;
  using end_t = EndIt;

  range(BeginIt&& begin, EndIt&& end)
      : begin_(std::forward<BeginIt>(begin)), end_(std::forward<EndIt>(end)) {}

  BeginIt begin() { return begin_; }
  EndIt end() { return end_; }

  template <typename It>
  auto&& read(It& it) {
    return *it;
  }

  template <typename It>
  void next(It& it) {
    ++it;
  }

  template <typename It>
  bool valid(It& it) {
    return it != end_;
  }

  BeginIt begin_;
  EndIt end_;
};

template <typename Container>
auto all(Container& c) {
  using std::begin;
  using std::end;
  return range<decltype(begin(c)), decltype(end(c))>{begin(c), end(c)};
}

template <typename T>
auto all(std::initializer_list<T>&& c) {
  return range<decltype(begin(c)), decltype(end(c))>{begin(c), end(c)};
}

}  // namespace utl