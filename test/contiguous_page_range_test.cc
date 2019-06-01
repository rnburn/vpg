#include "vpg/detail/contiguous_page_range.h"

#include "3rd_party/catch2/catch.hpp"
using vpg::detail::contiguous_page_range;

TEST_CASE("contiguous_page_range manages an area of mmapped memory") {
  const auto page_size = contiguous_page_range::page_size();
  contiguous_page_range page_range{page_size*10};

  SECTION("we can move-construct contiguous_page_range") {
    contiguous_page_range page_range2{std::move(page_range)};
  }

  SECTION("we can move-assign contiguous_page_range") {
    contiguous_page_range page_range2{page_size};
    page_range2 = std::move(page_range);
  }

  SECTION("we can access memory in contiguous_page_range after it's been realized") {
    page_range.realize(page_range.data(), page_size);
    auto xptr = static_cast<int*>(page_range.data());
    *xptr = 3;
    REQUIRE(*xptr == 3);
  }

  SECTION("we can unrealize memory that's been realized") {
    page_range.realize(page_range.data(), page_size);
    page_range.unrealize(page_range.data(), page_size);
  }
}
