#pragma once

#include <cstddef>

namespace vpg {
namespace detail {
class contiguous_page_range {
 public:
  explicit contiguous_page_range(size_t max_size);

  ~contiguous_page_range() noexcept;

  void realize(void* address, size_t length);

  void unrealize(void* address, size_t length);

  void* data() noexcept { return data_; }

  const void* data() const noexcept { return data_; }

  size_t max_size() const noexcept { return max_size_; }
 private:
  size_t max_size_;
  void* data_;
};
} // namespace detail
} // namespace vpg
