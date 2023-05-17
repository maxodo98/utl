#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "fmt/ostream.h"

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(unlikely)
#define UTL_UNLIKELY [[unlikely]]
#else
#define UTL_UNLIKELY
#endif
#else
#define UTL_UNLIKELY
#endif

#define utl_verify(condition, fmt_str, ...)                 \
  [&]() {                                                   \
    if (!(condition)) {                                     \
      UTL_UNLIKELY throw ::utl::fail(fmt_str, __VA_ARGS__); \
    }                                                       \
  }()

namespace utl {

template <typename Msg, typename... FmtArgs>
std::runtime_error fail(Msg&& msg, FmtArgs... args) {
  using clock = std::chrono::system_clock;

  auto const now = clock::to_time_t(clock::now());
  struct tm tmp;
#if _MSC_VER >= 1400
  gmtime_s(&tmp, &now);
#else
  gmtime_r(&now, &tmp);
#endif

  fmt::print(std::clog, "{} [VERIFY FAIL] ", std::put_time(&tmp, "%FT%TZ"));
  fmt::print(std::clog, std::forward<Msg>(msg), std::forward<FmtArgs>(args)...);
  fmt::print(std::clog, "\n");

  return std::runtime_error{
      fmt::format(std::forward<Msg>(msg), std::forward<FmtArgs>(args)...)};
}

template <typename Msg, typename... FmtArgs>
void verify(bool condition, Msg&& msg, FmtArgs... args) {
  if (!condition) {
    UTL_UNLIKELY throw fail(std::forward<Msg>(msg),
                            std::forward<FmtArgs>(args)...);
  }
}

template <typename Msg, typename... FmtArgs>
void verify_silent(bool condition, Msg&& msg, FmtArgs... args) {
  if (!condition) {
    UTL_UNLIKELY throw std::runtime_error{
        fmt::format(std::forward<Msg>(msg), std::forward<FmtArgs>(args)...)};
  }
}

template <typename Exception>
void verify_ex(bool condition, Exception&& ex) {
  if (!condition) {
    UTL_UNLIKELY throw ex;
  }
}

}  // namespace utl
