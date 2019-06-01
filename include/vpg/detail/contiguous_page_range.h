#pragma once

#include <cstddef>

namespace vpg {
namespace detail {
class contiguous_page_range {
 public:
  contiguous_page_range() noexcept : contiguous_page_range{0} {}

  explicit contiguous_page_range(size_t max_size);

  contiguous_page_range(const contiguous_page_range&) = delete;

  contiguous_page_range(contiguous_page_range&& other) noexcept;

  ~contiguous_page_range() noexcept;

  contiguous_page_range& operator=(const contiguous_page_range&) = delete;

  contiguous_page_range& operator=(contiguous_page_range&& other) noexcept;

  void realize(void* address, size_t length);

  void unrealize(void* address, size_t length);

  void* data() noexcept { return data_; }

  const void* data() const noexcept { return data_; }

  size_t max_size() const noexcept { return max_size_; }

  static size_t page_size() noexcept;

 private:
  size_t max_size_;
  void* data_;

  void unreserve() noexcept;
};
} // namespace detail
} // namespace vpg
