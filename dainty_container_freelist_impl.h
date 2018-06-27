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

#ifndef _DAINTY_CONTAINTER_FREELIST_IMPL_H_
#define _DAINTY_CONTAINTER_FREELIST_IMPL_H_

#include <memory>
#include "dainty_named.h"
#include "dainty_container_valuestore.h"

namespace dainty
{
namespace container
{
namespace freelist
{
///////////////////////////////////////////////////////////////////////////////

  using named::t_void;
  using named::t_bool;
  using named::t_n_;

  enum  t_id_tag_ {};
  using t_id_ = named::t_ix_;
  using t_id  = named::t_explicit<t_id_, t_id_tag_>;

  template<typename T>
  struct t_result {
    using t_id    = freelist::t_id;
    using p_value = T*;
    using r_value = T&;

    t_id    id;
    p_value ptr;

    inline t_result()                        : id(0),   ptr(nullptr) { }
    inline t_result(t_id_ _id, p_value _ptr) : id(_id), ptr(_ptr)    { }

    inline operator t_bool() const                      { return  ptr; }
    inline r_value operator*()                          { return *ptr; }
  };

  template<typename T>
  struct t_entry {
    using t_store = valuestore::t_valuestore<T>;
    t_store store_;
    t_n_    free_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_void (*CLEANUP)(T&)>
  class t_freelist_impl_ {
    enum { USED = 0xFFFFFFFF }; // XXX - not a good idea - change later
  public:
    using t_id     = freelist::t_id;
    using t_result = freelist::t_result<T>;
    using t_entry  = freelist::t_entry<T>;
    using p_entry  = t_entry*;
    using p_centry = const t_entry*;
    using r_entry  = t_entry&;
    using r_centry = const t_entry&;
    using t_value  = T;
    using p_value  = T*;
    using p_cvalue = const T*;
    using r_value  = T&;
    using r_cvalue = const T&;

    inline
    t_freelist_impl_(p_entry _entry, t_n_ max) : size_{0}, free_{0} {
      for (t_n_ i = 0; i < max; /**/) {
        r_entry entry = _entry[i++];
        entry.free_ = i;
      }
    }

    inline
    t_result insert(p_entry _entry, t_n_ max) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_, entry.store_.default_construct());
        free_ = entry.free_;
        entry.free_ = USED;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_result insert(p_entry _entry, t_n_ max, r_cvalue value) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_, entry.store_.copy_construct(value));
        free_ = entry.free_;
        entry.free_ = USED;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_result insert(p_entry _entry, t_n_ max, t_value&& value) {
      if (free_ < max) {
        r_entry entry = _entry[free_];
        t_result tmp(free_, entry.store_.move_construct(std::move(value)));
        free_ = entry.free_;
        entry.free_ = USED;
        ++size_;
        return tmp;
      }
      return {};
    }

    inline
    t_bool erase(p_entry _entry, t_n_ max, t_id_ id) {
      if (id < max) {
        r_entry entry = _entry[id];
        if (entry.free_ == USED) {
          CLEANUP(entry.store_.ref());
          entry.store_.destruct();
          entry.free_  = free_;
          free_        = id;
          --size_;
          return true;
        }
      }
      return false;
    }

    inline
    t_bool erase(p_entry entry, t_n_ max, p_cvalue p) {
      named::t_uint64 begin = (named::t_uint64)entry,
                      end   = begin + (sizeof(t_entry)*max),
                      pos   = (named::t_uint64)p;
      if (pos >= begin && pos < end && !(pos % alignof(t_value)))
        return erase((pos - begin)/sizeof(t_entry));
      return false;
    }

    inline
    t_void clear(p_entry _entry, t_n_ max) {
      for (t_n_ i = 0; i < max; /* none */ ) {
        r_entry entry = _entry[i++];
        if (entry.free_ == USED) {
          CLEANUP(entry.store_.ref());
          entry.store_.destruct();
        }
        entry.free_ = i;
      }
      size_ = 0;
      free_ = 0;
    }

    inline
    t_bool is_full(t_n_ max) const {
      return free_ == max;
    }

    inline
    t_bool is_empty() const {
      return size_ == 0;
    }

    inline
    t_n_ get_size() const {
      return size_;
    }

    inline
    p_value get(p_entry _entry, t_n_ max, t_id_ id) {
      if (id < max) {
        r_entry entry = _entry[id];
        if (entry.free_ == USED)
          return entry.store_.ptr();
      }
      return nullptr;
    }

    inline
    p_cvalue get(p_centry _entry, t_n_ max, t_id_ id) const {
      if (id < max) {
        r_centry entry = _entry[id];
        if (entry.free_ == USED)
          return entry.store_.cptr();
      }
      return nullptr;
    }

    template<typename F>
    inline
    t_void each(p_entry _entry, t_n_ max, F f) {
      for (t_id_ id = 0; id < max; ++id) {
        r_entry entry = _entry[id];
        if (entry.free_ == USED)
          f(t_id{id}, entry.store_.ref());
      }
    }

    template<typename F>
    inline
    t_void each(p_centry _entry, t_n_ max, F f) const {
      for (t_id_ id = 0; id < max; ++id) {
        r_centry entry = _entry[id];
        if (entry.free_ == USED)
          f(t_id{id}, entry.store_.cref());
      }
    }


  private:
    t_n_ size_;
    t_n_ free_;
  };

///////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
