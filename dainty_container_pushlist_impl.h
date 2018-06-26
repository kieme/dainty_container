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

#ifndef _DAINTY_CONTAINER_PUSHLIST_H_
#define _DAINTY_CONTAINER_PUSHLIST_H_

#include "dainty_named.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace pushlist
{
  using named::t_bool;
  using named::t_void;
  using named::t_n_;
  using named::t_n;
  using named::t_ix_;

  template<typename T>
  class t_pushlist_impl_ {
  public:
    using t_store  = valuestore::t_valuestore<T>;
    using p_store  = t_store*;
    using p_cstore = const t_store*;
    using t_value  = T;
    using p_value  = T*;
    using p_cvalue = const T*;

    inline
    t_pushlist_impl_() : next_(0) {
    }

    inline
    p_value push_back(p_store store, t_n_ max) {
      if (next_ < max)
        return store[next_++].default_construct();
      return nullptr;
    }

    inline
    p_value push_back(p_store store, t_n_ max, const t_value& value) {
      if (next_ < max)
        return store[next_++].copy_construct(value);
      return nullptr;
    }

    inline
    p_value push_back(p_store store, t_n_ max, t_value&& value) {
      if (next_ < max)
        return store[next_++].move_construct(std::move(value));
      return nullptr;
    }

    inline
    t_bool pop_back(p_store store) {
      if (next_) {
        store[--next_].destruct();
        return true;
      }
      return false;
    }

    inline
    t_bool is_full(t_n_ max) const {
      return next_ == max;
    }

    inline
    t_bool is_empty() const {
      return next_ == 0;
    }

    inline
    t_n_ get_size() const {
      return next_;
    }

    inline
    t_value& get(p_store store, t_n_ max, t_ix_ ix) {
      if (ix < max)
        return store[ix].ref();
      return nullptr;
    }

    inline
    const t_value& get(p_cstore store, t_n_ max, t_ix_ ix) const {
      if (ix < max)
        return store[ix].const_ref();
      return nullptr;
    }

    inline
    t_void clear(p_store store) {
      if (next_) {
        //valuestore::destruct_(store, next_);
        destruct_(store, t_n{next_});
        next_ = 0;
      }
    }

  private:
    t_n_ next_;
  };
}
}
}

#endif
