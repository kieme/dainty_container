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

#ifndef _DAINTY_CONTAINER_MAP_H_
#define _DAINTY_CONTAINER_MAP_H_

#include "dainty_named.h"
#include "dainty_container_map_impl.h"

namespace dainty
{
namespace container
{
namespace map
{
  //////////////////////////////////////////////////////////////////////////

  template<typename K>
  struct t_less_ {
    t_bool operator()(const K& lh, const K& rh) const {
      return lh < rh;
    }
  };

  //////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename K, typename T, typename C = t_less_<K> >
  class t_map {
    using t_impl_ = t_map_impl_<N, K, T, C, t_is_builtin_<K>::RESULT>;
  public:
    using t_compare   = typename t_impl_::t_compare;
    //using t_id        = typename t_impl_::t_id;
    //using t_ix        = typename t_impl_::t_ix;
    //using t_n         = typename t_impl_::t_n;
    using t_keyvalue  = typename t_impl_::t_keyvalue;
    using r_ckeyvalue = typename t_impl_::r_ckeyvalue;
    using p_keyvalue  = typename t_impl_::p_keyvalue;
    using p_ckeyvalue = typename t_impl_::p_ckeyvalue;
    using t_key       = typename t_impl_::t_key;
    using r_ckey      = typename t_impl_::r_ckey;
    using t_value     = typename t_impl_::t_value;
    using t_result    = typename t_impl_::t_result;
    using t_cresult   = typename t_impl_::t_cresult;

    t_result insert(r_ckey);
    t_result insert(r_ckeyvalue);
    t_result insert(t_keyvalue&&);

    t_bool erase(r_ckey);
    t_bool erase(t_ix);
    t_bool erase(t_id);
    t_void clear();

    t_result  find(r_ckey);
    t_cresult find(r_ckey) const;

    t_n    get_size() const;
    t_n    get_capacity() const;
    t_bool is_empty() const;

    p_keyvalue   get(t_ix);
    p_ckeyvalue  get(t_ix) const;
    p_ckeyvalue cget(t_ix) const;

    p_keyvalue   get(t_id);
    p_ckeyvalue  get(t_id) const;
    p_ckeyvalue cget(t_id) const;

    template<typename F> t_void  each(F);
    template<typename F> t_void  each(F) const;
    template<typename F> t_void ceach(F) const;

    template<typename F> t_void ordered_each(F);
    template<typename F> t_void ordered_each(F) const;
    template<typename F> t_void ordered_ceach(F) const;

  private:
    t_impl_ impl_;
  };

  //////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result t_map<N, K, T, C>::insert(r_ckey key) {
    return impl_.insert(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(r_ckeyvalue keyvalue) {
    return impl_.insert(keyvalue);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::insert(t_keyvalue&& keyvalue) {
    return impl_.insert(std::move(keyvalue));
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(r_ckey key) {
    return impl_.erase(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_ix ix) {
    return impl_.erase(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::erase(t_id id) {
    return impl_.erase(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_void t_map<N, K, T, C>::clear() {
    impl_.clear();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_result
    t_map<N, K, T, C>::find(r_ckey key) {
    return impl_.find(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::t_cresult
    t_map<N, K, T, C>::find(r_ckey key) const {
    return impl_.find(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_n t_map<N, K, T, C>::get_size() const {
    return impl_.get_size();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_n t_map<N, K, T, C>::get_capacity() const {
    return impl_.get_capacity();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  t_bool t_map<N, K, T, C>::is_empty() const {
    return impl_.is_empty();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_ix ix) {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_ckeyvalue
    t_map<N, K, T, C>::get(t_ix ix) const {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_ckeyvalue
    t_map<N, K, T, C>::cget(t_ix ix) const {
    return impl_.get(ix);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_keyvalue
    t_map<N, K, T, C>::get(t_id id) {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_ckeyvalue
    t_map<N, K, T, C>::get(t_id id) const {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline
  typename t_map<N, K, T, C>::p_ckeyvalue
    t_map<N, K, T, C>::cget(t_id id) const {
    return impl_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(F f) {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::each(F f) const {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ceach(F f) const {
    impl_.each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(F f) {
    impl_.ordered_each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_each(F f) const {
    impl_.ordered_each(f);
  }

  template<t_n_ N, typename K, typename T, typename C>
  template<typename F>
  inline
  t_void t_map<N, K, T, C>::ordered_ceach(F f) const {
    impl_.ordered_each(f);
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
