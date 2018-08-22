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

#ifndef _DAINTY_CONTAINER_BYTEBUF_H_
#define _DAINTY_CONTAINER_BYTEBUF_H_

#include "dainty_container_bytebuf_impl.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
//////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N = 0>
  class t_bytebuf {
  public:
    using t_tag   = TAG;

    constexpr static t_bool on_heap = false;

    t_bytebuf() = default;
    t_bytebuf(const t_bytebuf&) = default;
    template<t_n_ N1>
    t_bytebuf(const t_bytebuf<TAG, N1>&);

    t_bytebuf& operator=(const t_bytebuf&) = default;
    template<t_n_ N1>
    t_bytebuf& operator=(const t_bytebuf<TAG, N1>&);

    t_n    get_capacity() const;
    r_byte operator[](t_ix);
    R_byte operator[](t_ix) const;

    p_bytes  ptr();
    P_bytes  ptr()  const;
    P_bytes cptr() const;

  private:
    template<typename, t_n_> friend class t_bytebuf;
    t_byte store_[N];
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  class t_bytebuf<TAG, 0> {
  public:
    using t_tag   = TAG;

    constexpr static t_bool on_heap = true;

     t_bytebuf() = default;
     t_bytebuf(t_n max);
     t_bytebuf(t_bytebuf&&);
     t_bytebuf(const t_bytebuf&);
     template<t_n_ N1>
     t_bytebuf(const t_bytebuf<TAG, N1>&);
    ~t_bytebuf();

    t_bytebuf& operator=(t_bytebuf&&);
    t_bytebuf& operator=(const t_bytebuf&);
    template<t_n_ N1>
    t_bytebuf& operator=(const t_bytebuf<TAG, N1>&);

    operator t_validity() const;

    t_n    get_capacity() const;
    r_byte operator[](t_ix);
    R_byte operator[](t_ix) const;

    p_bytes  ptr();
    P_bytes  ptr()  const;
    P_bytes cptr() const;

    t_bool release();

  private:
    template<typename, t_n_> friend class t_bytebuf;
    t_bytebuf& sized_copy_(P_bytes, t_n_);

    t_n_    max_   = 0;
    p_bytes store_ = nullptr;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_bytebuf<TAG, N>::t_bytebuf(const t_bytebuf<TAG, N1>& buf) {
    copy_(store_, N, buf.store_, get(buf.get_capacity()));
  }

  template<typename TAG, t_n_ N>
  template<t_n_ N1>
  inline
  t_bytebuf<TAG, N>& t_bytebuf<TAG, N>::operator=(const t_bytebuf<TAG, N1>& buf) {
    copy_(store_, N, buf.store_, get(buf.get_capacity()));
    return *this;
  }

  template<typename TAG, t_n_ N>
  inline
  t_n t_bytebuf<TAG, N>::get_capacity() const {
    return t_n{N};
  }

  template<typename TAG, t_n_ N>
  inline
  r_byte t_bytebuf<TAG, N>::operator[](t_ix ix) {
    return store_[get(ix)];
  }

  template<typename TAG, t_n_ N>
  inline
  R_byte t_bytebuf<TAG, N>::operator[](t_ix ix) const {
    return store_[get(ix)];
  }

  template<typename TAG, t_n_ N>
  inline
  p_bytes t_bytebuf<TAG, N>::ptr() {
    return store_;
  }

  template<typename TAG, t_n_ N>
  inline
  P_bytes t_bytebuf<TAG, N>::ptr() const {
    return store_;
  }

  template<typename TAG, t_n_ N>
  inline
  P_bytes t_bytebuf<TAG, N>::cptr() const {
    return store_;
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>::t_bytebuf(t_n max)
    : max_{get(max)}, store_{alloc_(max_)} {
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>::t_bytebuf(t_bytebuf&& buf)
    : max_{named::reset(buf.max_)}, store_{named::reset(buf.store_)} {
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>::t_bytebuf(const t_bytebuf& buf)
      : max_{get(buf.get_capacity())}, store_{alloc_(max_)} {
    copy_(store_, max_, buf.store_, max_);
  }

  template<typename TAG>
  template<t_n_ N1>
  inline
  t_bytebuf<TAG, 0>::t_bytebuf(const t_bytebuf<TAG, N1>& buf)
      : max_{get(buf.get_capacity())}, store_{alloc_(max_)} {
    copy_(store_, max_, buf.store_, max_);
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>::~t_bytebuf() {
    if (store_)
      dealloc_(store_);
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>& t_bytebuf<TAG, 0>::operator=(t_bytebuf&& buf) {
    if (store_)
      dealloc_(store_);
    max_   = named::reset(buf.max_);
    store_ = named::reset(buf.store_);
    return *this;
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>& t_bytebuf<TAG, 0>::sized_copy_(P_bytes bytes, t_n_ max) {
    t_n_ old = named::reset(max_, max);
    if (max_) {
      if (old < max_) {
        if (store_)
          dealloc_(store_);
        store_ = alloc_(max_);
      }
      copy_(store_, max_, bytes, max_);
    } else if (store_)
      dealloc_(named::reset(store_));
    return *this;
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>& t_bytebuf<TAG, 0>::operator=(const t_bytebuf& buf) {
    return sized_copy_(buf.store_, buf.max_);
  }

  template<typename TAG>
  template<t_n_ N1>
  inline
  t_bytebuf<TAG, 0>&
      t_bytebuf<TAG, 0>::operator=(const t_bytebuf<TAG, N1>& buf) {
    return sized_copy_(buf.store_, get(buf.get_capacity()));
  }

  template<typename TAG>
  inline
  t_bytebuf<TAG, 0>::operator t_validity() const {
    return store_ ? VALID : INVALID;
  }

  template<typename TAG>
  inline
  t_n t_bytebuf<TAG, 0>::get_capacity() const {
    return t_n{max_};
  }

  template<typename TAG>
  inline
  r_byte t_bytebuf<TAG, 0>::operator[](t_ix ix) {
    return store_[get(ix)];
  }

  template<typename TAG>
  inline
  R_byte t_bytebuf<TAG, 0>::operator[](t_ix ix) const {
    return store_[get(ix)];
  }

  template<typename TAG>
  inline
  p_bytes t_bytebuf<TAG, 0>::ptr() {
    return store_;
  }

  template<typename TAG>
  inline
  P_bytes t_bytebuf<TAG, 0>::ptr()  const {
    return store_;
  }

  template<typename TAG>
  inline
  P_bytes t_bytebuf<TAG, 0>::cptr() const {
    return store_;
  }

  template<typename TAG>
  inline
  t_bool t_bytebuf<TAG, 0>::release() {
    if (store_) {
      dealloc_(named::reset(store_));
      named::reset(max_);
      return true;
    }
    return false;
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
