#pragma once

#include <cassert>
#include <iterator>

#include "vpg/detail/contiguous_page_range.h"
#include "vpg/detail/value_range.h"

namespace vpg {
namespace detail {
template <class T>
class page_mapped_value_range {
 public:
  page_mapped_value_range(size_t max_size, size_t start)
      : page_range_{max_size},
        first_{reinterpret_cast<T*>(static_cast<char*>(page_range_.data()) +
                                    start)},
        last_{first_},
        value_range_{first_} {
    assert(start <= max_size);
  }

  void reserve_from_front(size_t n) {
    auto space_available = std::distance(first_, value_range_.first());
    if (space_available >= n) {
      return;
    }
    auto aligned_first = contiguous_page_range::align_down_to_page_boundary(
        static_cast<void*>(first_));
    auto start_new = first_ - n;
    auto aligned_first_new = contiguous_page_range::align_down_to_page_boundary(
        static_cast<void*>(start_new));
    if (aligned_first_new < page_range_.data()) {
      throw std::bad_alloc{};
    }
    page_range_.realize(aligned_first_new,
                        std::distance(static_cast<char*>(aligned_first_new),
                                      static_cast<char*>(aligned_first)));
    auto adjustment = std::distance(static_cast<char*>(aligned_first_new),
                                    reinterpret_cast<char*>(first_)) %
                      sizeof(T);
    first_ = reinterpret_cast<T*>(static_cast<char*>(aligned_first_new) +
                                  adjustment);
  }

  void reserve_from_back(size_t n) {
    (void)n;
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    reserve_from_front(1);
    value_range_.emplace_front(std::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    reserve_from_back(1);
    value_range_.emplace_back(std::forward<Args>(args)...);
  }

 private:
  contiguous_page_range page_range_;
  T* first_;
  T* last_;
  value_range<T> value_range_;
};
}  // namespace detail
}  // namespace vpg
