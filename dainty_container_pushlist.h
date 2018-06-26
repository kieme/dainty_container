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

#ifndef _DAINTY_CONTAINER_H_
#define _DAINTY_CONTAINER_H_

#include "dainty_named.h"
#include "dainty_container_pushlist_impl.h"

namespace dainty
{
namespace container
{
namespace pushlist
{
  using named::t_ix;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N = 0>
  class t_pushlist {
    using t_impl_ = t_pushlist_impl_<T>;
  public:
    using t_value  = typename t_impl_::t_value;
    using p_value  = typename t_impl_::p_value;
    using p_cvalue = typename t_impl_::p_cvalue;

    t_pushlist();
    ~t_pushlist();

    operator t_validity() const;

    p_value push_back();
    p_value push_back(const t_value&);
    p_value push_back(t_value&&);
    t_bool  pop_back();

    t_void clear();

    t_bool is_full      () const;
    t_bool is_empty     () const;
    t_n    get_size     () const;
    t_n    get_capacity () const;

          t_value& operator[](t_ix);
    const t_value& operator[](t_ix) const;

  private:
    typename t_impl_::t_store store_[N];
    t_impl_                    impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class t_pushlist<T, 0> {
    using t_impl_ = t_pushlist_impl_<T>;
  public:
    using t_value  = typename t_impl_::t_value;
    using p_value  = typename t_impl_::p_value;
    using p_cvalue = typename t_impl_::p_cvalue;

    t_pushlist(t_n max);
    ~t_pushlist();

    operator t_validity() const;

    p_value push_back();
    p_value push_back(const t_value&);
    p_value push_back(t_value&&);
    t_bool  pop_back();

    t_void clear();

    t_bool is_full      () const;
    t_bool is_empty     () const;
    t_n    get_size     () const;
    t_n    get_capacity () const;

          t_value& operator[](t_ix);
    const t_value& operator[](t_ix) const;

  private:
    t_n_                       max_;
    typename t_impl_::t_store* store_;
    t_impl_                    impl_;
  };

///////////////////////////////////////////////////////////////////////////////

  template<typename T, t_n_ N>
  inline
  t_pushlist<T, N>::t_pushlist() {
  }

  template<typename T, t_n_ N>
  inline
  t_pushlist<T, N>::~t_pushlist() {
    clear();
  }

  template<typename T, t_n_ N>
  inline
  t_pushlist<T, N>::operator t_validity() const {
    return VALID;
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value t_pushlist<T, N>::push_back() {
    return impl_.push_back(store_, N);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(const t_value& value) {
    return impl_.push_back(store_, N, value);
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::p_value
    t_pushlist<T, N>::push_back(t_value&& value) {
    return impl_.push_back(store_, N, std::move(value));
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_pushlist<T, N>::pop_back() {
    return impl_.pop_back(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_void t_pushlist<T, N>::clear() {
    return impl_.clear(store_);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_pushlist<T, N>::is_full() const {
    return impl_.is_full(N);
  }

  template<typename T, t_n_ N>
  inline
  t_bool t_pushlist<T, N>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T, t_n_ N>
  inline
  t_n t_pushlist<T, N>::get_size() const {
    return t_n{impl_.get_size(store_)};
  }

  template<typename T, t_n_ N>
  inline
  t_n t_pushlist<T, N>::get_capacity() const {
    return t_n{N};
  }

  template<typename T, t_n_ N>
  inline
  typename t_pushlist<T, N>::t_value&
    t_pushlist<T, N>::operator[](t_ix ix) {
    return impl_.get(store_, N, named::get(ix));
  }

  template<typename T, t_n_ N>
  inline
  const typename t_pushlist<T, N>::t_value&
    t_pushlist<T, N>::operator[](t_ix ix) const {
    return impl_.get(store_, N, named::get(ix));
  }

///////////////////////////////////////////////////////////////////////////////

  template<typename T>
  inline
  t_pushlist<T, 0>::t_pushlist(t_n max)
    : max_{get(max)}, store_{new typename t_impl_::t_store[max_]} {
  }

  template<typename T>
  inline
  t_pushlist<T, 0>::~t_pushlist() {
    clear();
    delete [] store_;
  }

  template<typename T>
  inline
  t_pushlist<T, 0>::operator t_validity() const {
    return store_ ? VALID : INVALID;
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value t_pushlist<T, 0>::push_back() {
    return impl_.push_back(store_, max_);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(const t_value& value) {
    return impl_.push_back(store_, max_, value);
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::p_value
    t_pushlist<T, 0>::push_back(t_value&& value) {
    return impl_.push_back(store_, max_, std::move(value));
  }

  template<typename T>
  inline
  t_bool t_pushlist<T, 0>::pop_back() {
    return impl_.pop_back(store_);
  }

  template<typename T>
  inline
  t_void t_pushlist<T, 0>::clear() {
    return impl_.clear(store_);
  }

  template<typename T>
  inline
  t_bool t_pushlist<T, 0>::is_full() const {
    return impl_.is_full(max_);
  }

  template<typename T>
  inline
  t_bool t_pushlist<T, 0>::is_empty() const {
    return impl_.is_empty();
  }

  template<typename T>
  inline
  t_n t_pushlist<T, 0>::get_size() const {
    return t_n{impl_.get_size(store_)};
  }

  template<typename T>
  inline
  t_n t_pushlist<T, 0>::get_capacity() const {
    return t_n{max_};
  }

  template<typename T>
  inline
  typename t_pushlist<T, 0>::t_value&
    t_pushlist<T, 0>::operator[](t_ix ix) {
    return impl_.get(store_, max_, named::get(ix));
  }

  template<typename T>
  inline
  const typename t_pushlist<T, 0>::t_value&
    t_pushlist<T, 0>::operator[](t_ix ix) const {
    return impl_.get(store_, max_, named::get(ix));
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
