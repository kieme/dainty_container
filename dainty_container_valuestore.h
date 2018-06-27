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

#ifndef _DAINTY_CONTAINER_VALUESTORE_H_
#define _DAINTY_CONTAINER_VALUESTORE_H_

#include <utility>
#include <memory>
#include "dainty_named.h"

namespace dainty
{
namespace container
{
namespace valuestore
{
  using named::t_void;
  using named::t_uchar;
  using named::t_n;

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  T* construct_(T* ptr) {
    return new (ptr) T;
  }

  template<typename T>
  inline
  T* construct_(T* ptr, const T& value) {
    return new (ptr) T(value);
  }

  template<typename T>
  inline
  T* construct_(T* ptr, T&& value) {
    return new (ptr) T(std::move(value));
  }

  template<typename T>
  inline
  void destruct_(T* ptr) {
    ptr->~T();
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_valuestore {
  public:
    using t_bytes  = t_uchar[sizeof(T)];
    using t_value  = T;
    using t_ref    = T&;
    using t_cref   = const T&;
    using p_value  = T*;
    using p_cvalue = const T*;

    constexpr static t_n get_size() {
      return sizeof(T);
    }

    p_value copy_construct(const T& value) {
      return construct_(ptr(), value);
    }

    p_value move_construct(T&& value) {
      return construct_(ptr(), std::move(value));
    }

    p_value default_construct() {
      return construct_(ptr());
    }

    t_void destruct() {
      destruct_(ptr());
    }

    p_value   ptr()             { return mk_ptr(); }
    p_cvalue  ptr() const       { return mk_ptr(); }
    p_cvalue cptr() const       { return mk_ptr(); }

    t_ref   ref()               { return *ptr(); }
    t_cref  ref() const         { return *ptr(); }
    t_cref cref() const         { return *cptr(); }

    t_bytes&       bytes()       { return store_; }
    const t_bytes& bytes() const { return store_; }

  private:
    p_value mk_ptr() {
      return reinterpret_cast<t_value*>(&store_[0]);
    }

    p_cvalue mk_ptr() const {
      return reinterpret_cast<const t_value*>(&store_[0]);
    }

    alignas(t_value) t_bytes store_;
  };

////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_void destruct_(t_valuestore<T>* store, t_n n) {
    t_n::t_value max = get(n);
    for (decltype(max) ix = 0; ix < max; ++ix)
      store[ix].destruct();
  }

////////////////////////////////////////////////////////////////////////////////

#define DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(type)  \
  inline                                                 \
  t_void destruct_(t_valuestore<type>*, t_n) {           \
  }

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_bool)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_char)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_short)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_int)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_long)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_llong)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_uchar)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_ushort)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_uint)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_ulong)

  DAINTY_CONTAINER_VALUESTORE_DESTRUCT_IMPL(named::t_ullong)

  // XXX - must add other buildtins as well

////////////////////////////////////////////////////////////////////////////////

#define DAINTY_CONTAINER_VALUESTORE_IMPL(type)                    \
  inline                                                          \
  type* construct_(type* ptr) {                                   \
    *ptr = type();                                                \
     return ptr;                                                  \
  }                                                               \
                                                                  \
  inline                                                          \
  type* construct_(type* ptr, const type& value) {                \
    *ptr = value;                                                 \
    return ptr;                                                   \
  }                                                               \
                                                                  \
  inline                                                          \
  type* construct_(type* ptr, type&& value) {                     \
    *ptr = value;                                                 \
    return ptr;                                                   \
  }                                                               \
                                                                  \
  inline                                                          \
  t_void destruct_(type*) {                                       \
  }

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_bool)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_char)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_short)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_int)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_long)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_llong)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_uchar)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_ushort)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_uint)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_ulong)

  DAINTY_CONTAINER_VALUESTORE_IMPL(named::t_ullong)

  // XXX - must add other buildtins as well

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif