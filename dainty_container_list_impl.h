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

#ifndef _DAINTY_CONTAINER_LIST_IMPL_H_
#define _DAINTY_CONTAINER_LIST_IMPL_H_

#include "dainty_named.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace list
{
  using named::t_bool;
  using named::t_void;
  using named::t_n_;
  using named::t_ix_;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_list_impl_ {
  public:
    using p_store  = valuestore::t_valuestore<t_value>*;
    using p_cstore = const valuestore::t_valuestore<t_value>*;
    using t_value  = T;
    using p_value  = T*;
    using p_cvalue = const T*;
    using r_cvalue = const T&;

    inline
    t_list_impl_() : next_(0) {
    }

    inline
    p_value push_back(p_store store, t_n_ max) {
      if (next_ < max)
        return store[next_++].default_construct();
      return nullptr;
    }

    inline
    p_value push_back(p_store store, t_n_ max, r_cvalue value) {
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
    t_bool erase(p_store store, t_ix_ ix) {
      if (ix < next_) {
        while (1) {
          store[ix].destruct();
          t_ix_ nix = ix + 1;
          if (nix < next_)
            store[ix].copy_construct(store_[nix].ref());
          else
            break;
          ix = nix;
        }
        --next_;
        return true;
      }
      return false;
    }

    inline
    void clear(p_store store) {
      if (next_) {
        valuestore::destruct_(store, next_);
        next_ = 0;
      }
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
    p_value get(p_store store, t_n_ max, t_ix_ ix) {
      if (ix < max)
        return store[ix].ptr();
      return nullptr;
    }

    inline
    p_cvalue get(p_store store, t_n_ max, t_ix_ ix) const {
      if (ix < max)
        return store[ix].cptr();
      return nullptr;
    }

  private:
    t_n_ next_;
  };
}
}
}

#endif
