/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

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
   using r_value = typename named::t_prefix<T>::r_;
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
     if (valid_ == VALID)
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

   r_maybe operator=(R_maybe maybe) {
     if (valid_)
       store_.destruct();
     if (maybe == VALID)
       store_.copy_construct(maybe.store_.cref());
     valid_ = maybe.valid_;
     return *this;
   }

   r_maybe operator=(x_maybe maybe) {
     if (valid_)
       store_.destruct();
     if (maybe == VALID)
       store_.move_construct(std::move(maybe.store_.ref()));
     valid_ = maybe.valid_;
     return *this;
   }

   operator t_validity() const { return valid_; }

  private:
    template<typename T1> friend       T1& set(t_maybe<T1>&);
    template<typename T1> friend const T1& get(const t_maybe<T1>&);

    valuestore::t_valuestore<t_value> store_;
    t_validity                        valid_;
  };

  template<typename T>
  inline T& set(t_maybe<T>& maybe) {
    return maybe.store_.ref();
  }

  template<typename T>
  inline const T& get(const t_maybe<T>& maybe) {
    return maybe.store_.cref();
  }
}
}
}

#endif
