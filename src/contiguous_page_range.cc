#include "vpg/detail/contiguous_page_range.h"

namespace vpg {
namespace detail {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
contiguous_page_range:: contiguous_page_range(size_t max_size) 
  : max_size_{max_size}
{
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
contiguous_page_range::~contiguous_page_range() noexcept {
}

//--------------------------------------------------------------------------------------------------
// realize
//--------------------------------------------------------------------------------------------------
void contiguous_page_range::realize(void* address, size_t length) {
  (void)address;
  (void)length;
}

//--------------------------------------------------------------------------------------------------
// unrealize
//--------------------------------------------------------------------------------------------------
void contiguous_page_range::unrealize(void* address, size_t length) {
  (void)address;
  (void)length;
}
} // namespace detail
} // namespace vpg
