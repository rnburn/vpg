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

  SECTION("round_to_page_boundary rounds a number up to a multiple of the page size") {
    for (size_t n : {1ul, 2ul, contiguous_page_range::page_size(),
                     contiguous_page_range::page_size() * 2 + 1,
                     contiguous_page_range::page_size() * 2 - 1}) {
      auto n_prime = contiguous_page_range::round_to_page_boundary(n);
      REQUIRE(n <= n_prime);
      REQUIRE(n_prime - n < contiguous_page_range::page_size());
      REQUIRE(n_prime % contiguous_page_range::page_size() == 0);
    }
  }

  std::vector<char> data(contiguous_page_range::page_size()*2);

  SECTION("We can round a pointer down to a page boundary") {
    for (auto& c : data) {
      auto ptr = static_cast<void*>(&c);
      auto ptr_prime = contiguous_page_range::align_down_to_page_boundary(ptr);
      REQUIRE(ptr_prime <= ptr);
      REQUIRE(contiguous_page_range::align_down_to_page_boundary(ptr_prime) == ptr_prime);
      REQUIRE(std::distance(static_cast<char*>(ptr_prime),
                            static_cast<char*>(ptr)) <
              contiguous_page_range::page_size());
    }
  }

  SECTION("We can round up to a page boundary") {
    for (auto& c : data) {
      auto ptr = static_cast<void*>(&c);
      auto ptr_prime = contiguous_page_range::align_up_to_page_boundary(ptr);
      REQUIRE(ptr <= ptr_prime);
      REQUIRE(contiguous_page_range::align_up_to_page_boundary(ptr_prime) == ptr_prime);
      REQUIRE(std::distance(static_cast<char*>(ptr),
                            static_cast<char*>(ptr_prime)) <
              contiguous_page_range::page_size());
    }
  }
}
