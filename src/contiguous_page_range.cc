#include "vpg/detail/contiguous_page_range.h"

#include <cassert>
#include <iterator>
#include <memory>

#include <sys/mman.h>
#include <unistd.h>

namespace vpg {
namespace detail {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
contiguous_page_range::contiguous_page_range(size_t max_size)
    : max_size_{max_size} {
  if (max_size_ == 0) {
    data_ = nullptr;
    return;
  }
  auto result =
      ::mmap(nullptr, max_size_, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (result == MAP_FAILED) {
    throw std::bad_alloc{};
  }
  data_ = result;
}

contiguous_page_range::contiguous_page_range(contiguous_page_range&& other) noexcept 
  : max_size_{other.max_size_},
    data_{other.data_}
{
  other.max_size_ = 0;
  other.data_ = nullptr;
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
contiguous_page_range::~contiguous_page_range() noexcept {
  unreserve();
}

//--------------------------------------------------------------------------------------------------
// operator=
//--------------------------------------------------------------------------------------------------
contiguous_page_range& contiguous_page_range::operator=(
    contiguous_page_range&& other) noexcept {
  unreserve();
  max_size_ = other.max_size_;
  data_ = other.data_;
  other.max_size_ = 0;
  other.data_ = nullptr;
  return *this;
}

//--------------------------------------------------------------------------------------------------
// realize
//--------------------------------------------------------------------------------------------------
void contiguous_page_range::realize(void* address, size_t length) {
  assert(length % page_size() == 0);
  assert(data_ <= address);
  assert(static_cast<size_t>(
             std::distance(static_cast<char*>(data_),
                           static_cast<char*>(address) + length)) <= max_size_);
  auto rcode = ::mmap(address, length, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
  if (rcode == MAP_FAILED) {
    throw std::bad_alloc{};
  }
}

//--------------------------------------------------------------------------------------------------
// unrealize
//--------------------------------------------------------------------------------------------------
void contiguous_page_range::unrealize(void* address, size_t length) {
  assert(length % page_size() == 0);
  assert(data_ <= address);
  assert(static_cast<size_t>(
             std::distance(static_cast<char*>(data_),
                           static_cast<char*>(address) + length)) <= max_size_);
  auto rcode =
      ::mmap(address, length, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (rcode == MAP_FAILED) {
    throw std::bad_alloc{};
  }
}

//--------------------------------------------------------------------------------------------------
// page_size
//--------------------------------------------------------------------------------------------------
size_t contiguous_page_range::page_size() noexcept {
  static size_t result = ::getpagesize();
  return result;
}

//--------------------------------------------------------------------------------------------------
// unreserve
//--------------------------------------------------------------------------------------------------
void contiguous_page_range::unreserve() noexcept {
  if (max_size_ == 0) {
    return;
  }
  auto rcode = ::munmap(data_, max_size_);
  (void)rcode;
  assert(rcode != -1);
  max_size_ = 0;
  data_ = nullptr;
}
} // namespace detail
} // namespace vpg
