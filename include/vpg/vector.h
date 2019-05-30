#pragma once

#include <limits>

#include "vpg/detail/page_mapped_value_range.h"

namespace vpg {
template <class T>
class vector {
 public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;

  vector(size_t size = 0, const T& value = T{},
         size_t max_size = std::numeric_limits<uint32_t>::max())
      : value_range_{max_size, 0} {
    value_range_.reserve_from_back(size);
    for (size_t i = 0; i < size; ++i) {
      value_range_.emplace_back(value);
    }
  }

  void push_back(const T& value) {
    value_range_.push_back(value);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    value_range_.emplace_back(std::forward<Args>(args)...);
  }

  void pop_back() noexcept {
    value_range_.pop_back();
  }

 private:
   detail::page_mapped_value_range<T> value_range_;
};
} // namespace vpg
