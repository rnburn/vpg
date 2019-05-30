#pragma once

#include <utility>

namespace vpg {
namespace detail {
template <class T>
class value_range {
 public:
   explicit value_range(T* first) 
     : first_{first}, last_{first}
   {}

   ~value_range() {
     for (auto iter=first_; iter!=last_; ++iter) {
       iter->~T();
     }
   }

   template <class... Args>
   void emplace_back(Args&&... args) {
     ::new (last_) T(std::forward<Args>(args)...);
     ++last_;
   }

   template <class... Args>
   void emplace_front(Args&&... args) {
     auto first_new = first_ - 1;
     ::new (first_new) T(std::forward<Args>(args)...);
     first_ = first_new;
   }

   void pop_front() noexcept {
     first_->~T();
     ++first_;
   }

   void pop_back() noexcept {
     --last_;
     last_->~T();
   }

   const T* first() const { return first_; }
   const T* last() const { return last_; }
 private:
   T* first_;
   T* last_;
};
} // namespace detail
} // namespace vpg
