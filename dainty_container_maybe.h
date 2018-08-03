#include <utility>

#ifndef _DAINTY_CONTAINER_MAYBE_H_
#define _DAINTY_CONTAINER_MAYBE_H_

#include "dainty_named.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace maybe
{
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

  template<typename T>
  class t_maybe {
  public:
   using t_value = typename named::t_prefix<T>::t_;
   using R_value = typename named::t_prefix<T>::R_;
   using x_value = typename named::t_prefix<T>::x_;

   using r_maybe = typename named::t_prefix<t_maybe>::r_;
   using R_maybe = typename named::t_prefix<t_maybe>::R_;
   using x_maybe = typename named::t_prefix<t_maybe>::x_;

   inline
   t_maybe() : valid_{INVALID} {
   }

   inline
   t_maybe(R_value value) : valid_{VALID} {
     store_.copy_construct(value);
   }

   inline
   t_maybe(x_value value) : valid_{VALID} {
     store_.move_construct(std::move(value));
   }

   inline
   t_maybe(R_maybe maybe) : valid_{maybe.valid_} {
     if (valid_ == VALID)
       store_.copy_construct(maybe.store_.cref());
   }

   inline
   t_maybe(x_maybe maybe) : valid_{maybe.valid_} {
     if (valid_ == VALID)
       store_.move_construct(std::move(maybe.store_.ref()));
   }

   inline
   ~t_maybe() {
     if (valid_)
       store_.destruct();
   }

   inline
   r_maybe operator=(R_value value) {
     if (valid_)
       store_.destruct();
     store_.copy_construct(value);
     valid_ = VALID;
     return *this;
   }

   inline
   r_maybe operator=(x_value value) {
     if (valid_)
       store_.destruct();
     store_.move_construct(std::move(value));
     valid_ = VALID;
     return *this;
   }

   inline
   r_maybe operator=(R_maybe maybe) {
     if (valid_)
       store_.destruct();
     store_.copy_construct(t.store_.cref());
     return *this;
   }

   inline
   r_maybe operator=(x_maybe maybe) {
     if (valid_)
       store_.destruct();
     store_.move_construct(std::move(t.store_.ref()));
     return *this;
   }

   operator t_validity() const { return valid_; }

  private:
    valuestore::t_valuestore<t_value> store_;
    t_validity                        valid_;
  };

  // get
  // set
}
}
}

#endif

int main() {
  return 0;
}
