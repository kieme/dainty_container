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

#ifndef _DAINTY_CONTAINER_MAP_IMPL_H_
#define _DAINTY_CONTAINER_MAP_IMPL_H_

#include <utility>
#include "dainty_named.h"
#include "dainty_container_list.h"
#include "dainty_container_freelist.h"

namespace dainty
{
namespace container
{
namespace map
{
  using named::t_bool;
  using named::t_void;
  using named::t_ix_;
  using named::t_ix;
  using named::t_n_;
  using named::t_n;
  using freelist::t_id_;
  using freelist::t_id;

  //////////////////////////////////////////////////////////////////////////

  template<typename T> struct t_is_builtin_        { enum { RESULT = 0 }; };
  template<> struct t_is_builtin_<named::t_char>   { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_uchar>  { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_short>  { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_ushort> { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_int>    { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_uint>   { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_long>   { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_ulong>  { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_llong>  { enum { RESULT = 1 }; };
  template<> struct t_is_builtin_<named::t_ullong> { enum { RESULT = 1 }; };

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T>
  struct t_keyvalue {
    using t_key    = K;
    using t_value  = T;

    const t_key key;
    t_value     value;

    constexpr
    t_keyvalue(const t_keyvalue& keyvalue)
      : key(keyvalue.key), value(keyvalue.value) {
    }

    constexpr
    t_keyvalue(t_keyvalue&& keyvalue)
      : key(std::move(keyvalue.key)), value(std::move(keyvalue.value)) {
    }

    template<typename K1>
    constexpr
    t_keyvalue(K1&& _key) : key(std::forward<K1>(_key)) {
    }

    template<typename K1, typename V1>
    constexpr
    t_keyvalue(K1&& _key, V1&& _value)
      : key(std::forward<K1>(_key)), value(std::forward<V1>(_value)) {
    }

    t_keyvalue& operator=(const t_keyvalue&) = delete;
    t_keyvalue& operator=(t_keyvalue&&) = delete;
  };

  //////////////////////////////////////////////////////////////////////////

  template<typename K, typename T>
  struct t_result {
    using r_keyvalue  = t_keyvalue<K, T>&;
    using r_ckeyvalue = const t_keyvalue<K, T>&;
    using p_keyvalue  = t_keyvalue<K, T>*;
    using p_ckeyvalue = const t_keyvalue<K, T>*;

    t_id       id;
    t_ix       ix;
    p_keyvalue keyvalue;

    inline
    t_result() : id(0), ix(0), keyvalue(nullptr) {
    }

    inline
    t_result(t_id _id, t_ix _ix, p_keyvalue _keyvalue)
      : id(_id), ix(_ix), keyvalue(_keyvalue) {
    }

    inline operator t_bool() const        { return  keyvalue; }
    inline r_keyvalue  operator* ()       { return *keyvalue; }
    inline r_ckeyvalue operator* () const { return *keyvalue; }
    inline p_keyvalue  operator->()       { return  keyvalue; }
    inline p_ckeyvalue operator->() const { return  keyvalue; }
  };

  template<typename K, typename T>
  struct t_cresult {
    using r_ckeyvalue = const t_keyvalue<K, T>&;
    using p_ckeyvalue = const t_keyvalue<K, T>*;

    t_id        id;
    t_ix        ix;
    p_ckeyvalue keyvalue;

    inline
    t_cresult() : id(0), ix(0), keyvalue(nullptr) {
    }

    inline
    t_cresult(t_id _id, t_ix _ix, p_ckeyvalue _keyvalue)
      : id(_id), ix(_ix), keyvalue(_keyvalue) {
    }

    inline operator t_bool() const        { return  keyvalue; }
    inline r_ckeyvalue operator* () const { return *keyvalue; }
    inline p_ckeyvalue operator->() const { return  keyvalue; }
  };

  template<typename K, typename T>
  struct t_entry_ {
    using t_keyvalue  = map::t_keyvalue<K, T>;
    using t_key       = typename t_keyvalue::t_key;
    using r_ckeyvalue = const t_keyvalue&;
    using r_ckey      = const t_key&;

    t_keyvalue keyvalue;
    t_ix       ix;

    t_entry_(r_ckey _key, t_ix _ix) : keyvalue{_key}, ix{_ix} {
    }

    t_entry_(t_key&& _key, t_ix _ix) : keyvalue{std::move(_key)}, ix{_ix} {
    }

    t_entry_(r_ckeyvalue _keyvalue, t_ix _ix) : keyvalue{_keyvalue}, ix{_ix} {
    }

    t_entry_(t_keyvalue&& _keyvalue, t_ix _ix)
      : keyvalue{std::move(_keyvalue)}, ix{_ix} {
    }

    t_entry_(t_entry_&& entry)
      : keyvalue{std::move(entry.keyvalue)}, ix{entry.ix} {
    }

    t_entry_(const t_entry_&) = delete;
    t_entry_& operator=(const t_entry_&) = delete;
    t_entry_& operator=(t_entry_&&) = delete;
  };

  //////////////////////////////////////////////////////////////////////////

  template<t_n_ N, typename K, typename T, typename C,
           bool = t_is_builtin_<K>::value>
  class t_map_impl_ : private C {
    using t_entry_ = map::t_entry_<K, T>;
    using t_store_ = freelist::t_freelist<t_entry_, N>;
    using t_ids_   = list::t_list<t_id, N>;
  public:
    using t_compare   = C;
    //using t_id        = map::t_id;
  //  using t_ix        = map::t_ix;
  //  using t_n         = map::t_n;
    using t_keyvalue  = map::t_keyvalue<K, T>;
    using r_ckeyvalue = const t_keyvalue&;
    using p_keyvalue  = t_keyvalue*;
    using p_ckeyvalue = const t_keyvalue*;
    using t_key       = typename t_keyvalue::t_key;
    using r_ckey      = const t_key&;
    using t_value     = typename t_keyvalue::t_value;
    using t_result    = map::t_result<t_key,  t_value>;
    using t_cresult   = map::t_cresult<t_key, t_value>;

    inline
    t_result insert(r_ckey key) {
      t_n_ n = named::get(ids_.get_size());
      if (n < N) {
        t_ix ix = lowerbound_(key);
        if (named::get(ix) == n ||
            C::operator()(key,
                          store_.get(*ids_.get(ix))->keyvalue.key)) {
          auto result = store_.insert(t_entry_{key, ix});
          ids_.insert(ix, result.id);
          return t_result{result.id, ix, &result->keyvalue};
        }
      }
      return {};
    }

    inline
    t_result insert(r_ckeyvalue keyvalue) {
      t_n_ n = named::get(ids_.get_size());
      if (n < N) {
        t_ix ix = lowerbound_(keyvalue.key);
        if (named::get(ix) == n ||
            C::operator()(keyvalue.key,
                          store_.get(*ids_.get(ix))->keyvalue.key)) {
          auto result = store_.insert(t_entry_{keyvalue, ix});
          ids_.insert(ix, result.id);
          return t_result{result.id, ix, &result->keyvalue};
        }
      }
      return {};
    }

    inline
    t_result insert(t_keyvalue&& keyvalue) {
      t_n_ n = named::get(ids_.get_size());
      if (n < N) {
        t_ix ix = lowerbound_(keyvalue.key);
        if (named::get(ix) == n ||
            C::operator()(keyvalue.key,
                          store_.get(*ids_.get(ix))->keyvalue.key)) {
          auto result = store_.insert(t_entry_{std::move(keyvalue), ix});
          ids_.insert(ix, result.id);
          return t_result{result.id, ix, &result->keyvalue};
        }
      }
      return {};
    }

    inline
    t_bool erase(r_ckey key) {
      t_n_ n = named::get(ids_.get_size());
      if (n) {
        t_ix ix = lowerbound(key);
        if (named::get(ix) != n &&
            !C::operator()(key,
                           store_.get(*ids_.get(ix))->keyvalue.key)) {
          store_.erase(*ids_.get(ix));
          ids_.erase(ix);
          return true;
        }
      }
      return false;
    }

    inline
    t_bool erase(t_ix ix) {
      t_n_ n = named::get(ids_.get_size());
      if (named::get(ix) < n) {
        store_.erase(*ids_.get(ix));
        ids_.erase(ix);
        return true;
      }
      return false;
    }

    inline
    t_bool erase(t_id id) {
      auto entry = store_.get(id);
      if (entry)
        return erase(entry->ix);
      return false;
    }

    inline
    t_void clear() {
      ids_.clear();
      store_.clear();
    }

    inline
    t_result find(r_ckey key) {
      t_n_ n = named::get(ids_.get_size());
      if (n) {
        t_n_ p = lowerbound(key);
        if (p < n) {
          t_id id{ids_[p]};
          p_keyvalue ptr = store_.get(id);
          if (!C::operator()(key, ptr->key))
            return {id, t_ix{p}, ptr};
        }
      }
      return {};
    }

    inline
    t_cresult find(r_ckey key) const {
      t_n_ n = named::get(ids_.get_size());
      if (n) {
        t_n_ p = lowerbound(key);
        if (p < n) {
          t_id id{ids_[p]};
          p_ckeyvalue ptr = store_.get(id);
          if (!C::operator()(key, ptr->key))
            return {id, t_ix{p}, ptr};
        }
      }
      return {};
    }

    inline
    t_n get_size() const {
      return ids_.get_size();
    }

    inline
    t_n get_capacity() const {
      return t_n{N};
    }

    inline
    t_bool is_empty() const {
      return ids_.is_empty();
    }

    inline
    p_keyvalue get(t_ix ix) {
      if (named::get(ix) < named::get(ids_.get_size()))
        return &store_.get(*ids_.get(ix))->keyvalue;
      return nullptr;
    }

    inline
    p_ckeyvalue get(t_ix ix) const {
      if (named::get(ix) < named::get(ids_.get_size()))
        return &store_.get(*ids_.get(ix))->keyvalue;
      return nullptr;
    }

    inline
    p_keyvalue get(t_id id) {
      auto entry = store_.get(id);
      if (entry)
        return &entry->keyvalue;
      return nullptr;
    }

    inline
    p_ckeyvalue get(t_id id) const {
      auto entry = store_.get(id);
      if (entry)
        return &entry->keyvalue;
      return nullptr;
    }

    template<typename F>
    inline
    t_void each(F f) {
      store_.each([func = std::move(f)](auto id, auto& entry) {
        func(id, entry.ix, entry.keyvalue); });
    }

    template<typename F>
    inline
    t_void each(F f) const {
      store_.each([func = std::move(f)](auto id, const auto& entry) {
        func(id, entry.ix, entry.keyvalue); });
    }

    template<typename F>
    inline
    t_void ordered_each(F f) {
      ids_.each([this, func = std::move(f), ix = 0](auto id) mutable {
        func(id, t_ix{ix++}, *this->store_.get(id)); });
    }

    template<typename F>
    inline
    t_void ordered_each(F f) const {
      ids_.each([this, func = std::move(f), ix = 0](auto id) mutable {
        func(id, t_ix{ix++}, *this->store_.get(id)); });
    }

  private:
    inline
    t_ix lowerbound_(r_ckey key) const {
      t_ix_ ix = 0;
      for (t_n_ range = named::get(get_size()); range;) {
        t_n_ leap = range/2;
        if (C::operator()(store_.get(*ids_.get(t_ix{ix + leap}))->keyvalue.key,
            key)) {
          t_n_ k = leap + 1;
          ix += k;
          range -= k;
        } else
          range = leap;
      }
      return t_ix{ix};
    }

    t_ids_   ids_;
    t_store_ store_;
  };

  //////////////////////////////////////////////////////////////////////////

  /*
  template<t_n_ N, typename K, typename T, typename C>
  class t_map_impl_<N,K,T,C,true> : private C {
  public:
    typedef K                                     t_key;
    typedef T                                     t_value;
    typedef C                                     t_compare;
    typedef t_id                             t_id;
    typedef t_keyvalue<t_key, t_value>       t_keyvalue;
    typedef t_result  <t_key, t_value>       t_result;
    typedef t_result  <t_key, const t_value> t_cresult;

    t_result insert(const t_key&);
    t_result insert(const t_keyvalue&);

    t_bool   erase (const t_key&);
    t_bool   erase (t_n_ idx);
    t_bool   erase (t_id);

    t_void   clear ();

          t_result       find(const t_key&);
    t_cresult       find(const t_key&) const;
    t_cresult const_find(const t_key&) const;

    t_n_ size    () const;
    t_n_ capacity() const;
    t_bool empty   () const;

          t_keyvalue*       get(t_n_ idx);
    const t_keyvalue*       get(t_n_ idx) const;
    const t_keyvalue* const_get(t_n_ idx) const;

          t_keyvalue*       get(t_id);
    const t_keyvalue*       get(t_id) const;
    const t_keyvalue* const_get(t_id) const;

  private:
    t_n_ lowerbound(const t_key&) const;

    typedef t_freelist<t_keyvalue, N>      list_t;
    typedef typename list_t::t_id::native_t native_t_id;

    struct entry_t {
      native_t_id id_;
      t_key       key_;
      entry_t() : id_(0) { }
      entry_t(native_t_id id, t_key key) : id_(id), key_(key) { }
    };

    entry_t entry_[N];
    list_t  keyvalues_;
  };
  */

  //////////////////////////////////////////////////////////////////////////

  /*
  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_result
    t_map_impl_<N, K, T, C, true>::insert(const t_key& key) {
    t_n_ n = keyvalues_.size();
    if (n < N) {
      t_n_ p = lowerbound(key);
      if (p == n || C::operator()(key, entry_[p].key_)) {
        for (; n != p; --n)
          entry_[n] = entry_[n-1];
        typename list_t::t_result r(keyvalues_.insert(key));
        entry_[p].id_  = types::get(r.id_);
        entry_[p].key_ = key;
        return t_result(r.id_, p, r.ptr_);
      }
    }
    return t_result();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_result
    t_map_impl_<N, K, T, C, true>::insert(const t_keyvalue& keyvalue) {
    t_n_ n = keyvalues_.size();
    if (n < N) {
      t_n_ p = lowerbound(keyvalue.key_);
      if (p == n || C::operator()(keyvalue.key_, entry_[p].key_)) {
        for (; n != p; --n)
          entry_[n] = entry_[n-1];
        typename list_t::t_result r(keyvalues_.insert(keyvalue));
        entry_[p].id_  = types::get(r.id_);
        entry_[p].key_ = keyvalue.key_;
        return t_result(r.id_, p, r.ptr_);
      }
    }
    return t_result();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_bool t_map_impl_<N, K, T, C, true>::erase(const t_key& key) {
    t_n_ n = keyvalues_.size();
    if (n) {
      t_n_ p = lowerbound(key);
      if (p != n && !C::operator()(key, entry_[p].key_)) {
        keyvalues_.erase(t_id(entry_[p].id_));
        for (; p != n; ++p)
          entry_[p] = entry_[p+1];
        return true;
      }
    }
    return false;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_bool t_map_impl_<N, K, T, C, true>::erase(t_n_ idx) {
    t_n_ n = keyvalues_.size();
    if (idx < n) {
      keyvalues_.erase(t_id(entry_[idx].id_));
      for (; idx != n; ++idx)
        entry_[idx] = entry_[idx+1];
      return true;
    }
    return false;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_bool t_map_impl_<N, K, T, C, true>::erase(t_id id) {
    t_keyvalue* keyvalue = keyvalues_.get(id);
    if (keyvalue) {
      t_n_ p = lowerbound(keyvalue->key_), n = keyvalues_.size();
      if (p != n && !C::operator()(keyvalue->key_,
                                   keyvalues_.get(t_id(entry_[p].id_))->key_)) {
        keyvalues_.erase(id);
        for (; p != n; ++p)
          entry_[p] = entry_[p+1];
        return true;
      }
    }
    return false;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_void t_map_impl_<N, K, T, C, true>::clear() {
    keyvalues_.clear();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_result
    t_map_impl_<N, K, T, C, true>::find(const t_key& key) {
    t_n_ n = keyvalues_.size();
    if (n) {
      t_n_ p = lowerbound(key);
      if (p < n) {
        t_id id(entry_[p].id_);
        t_keyvalue* ptr = keyvalues_.get(id);
        if (!C::operator()(key, ptr->key_))
          return t_result(id, p, ptr);
      }
    }
    return t_result();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_cresult
    t_map_impl_<N, K, T, C, true>::find(const t_key& key) const {
    return const_find(key);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_cresult
    t_map_impl_<N, K, T, C, true>::const_find(const t_key& key) const {
    t_n_ n = keyvalues_.size();
    if (n) {
      t_n_ p = lowerbound(key);
      if (p < n) {
        t_id id(entry_[p].id_);
        const t_keyvalue* ptr = keyvalues_.get(id);
        if (!C::operator()(key, ptr->key_))
          return t_cresult(id, p, ptr);
      }
    }
    return t_cresult();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_n_ t_map_impl_<N, K, T, C, true>::size() const {
    return keyvalues_.size();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_n_ t_map_impl_<N, K, T, C, true>::capacity() const {
    return keyvalues_.capacity();
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_bool t_map_impl_<N, K, T, C, true>::empty() const {
    return keyvalues_.size() == 0;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::get(t_n_ idx) {
    return idx < keyvalues_.size() ?
             keyvalues_.get(t_id(entry_[idx].id_)) : 0;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline const typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::get(t_n_ idx) const {
    return const_get(idx);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline const typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::const_get(t_n_ idx) const {
    return idx < keyvalues_.size() ?
             keyvalues_.const_get(t_id(entry_[idx].id_)) : 0;
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::get(t_id id) {
    return keyvalues_.get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline const typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::get(t_id id) const {
    return const_get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline const typename t_map_impl_<N, K, T, C, true>::t_keyvalue*
    t_map_impl_<N, K, T, C, true>::const_get(t_id id) const {
    return keyvalues_.const_get(id);
  }

  template<t_n_ N, typename K, typename T, typename C>
  inline t_n_ t_map_impl_<N, K, T, C, true>::lowerbound(const t_key& key) const {
    t_n_ p = 0;
    for (t_n_ range = size(); range;) {
      t_n_ leap = range/2;
      if (C::operator()(entry_[p + leap].key_, key)) {
        t_n_ k = leap + 1;
        p += k;
        range -= k;
      } else
        range = leap;
    }
    return p;
  }
  */

  //////////////////////////////////////////////////////////////////////////

}
}
}

#endif
