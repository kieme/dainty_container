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

#ifndef _DAINTY_CONTAINER_ANY_H_
#define _DAINTY_CONTAINER_ANY_H_

#include <utility>
#include "dainty_named.h"

namespace dainty
{
namespace container
{
namespace any
{
  using named::t_bool;
  using named::t_validity;
  using named::VALID;
  using named::INVALID;

///////////////////////////////////////////////////////////////////////////////

  struct t_erase_it_ {
    virtual ~t_erase_it_() {}
    virtual t_bool is_equal (const t_erase_it_&) const = 0;
  };

  t_bool same_type_(const t_erase_it_&, const t_erase_it_& it);

  template<class T>
  struct t_store_ final : t_erase_it_ {
    T value_;

    template<class U> t_store_(U&& u) : value_(std::forward<U>(u)) { }
    t_store_()                           = delete;
    t_store_(const t_store_&)            = delete;
    t_store_& operator=(const t_store_&) = delete;
    t_store_(t_store_&&)                 = delete;
    t_store_& operator=(t_store_&&)      = delete;

    virtual t_bool is_equal(const t_erase_it_& it) const override {
      return same_type_(*this, it) &&
             value_ == static_cast<const t_store_<T>&>(it).value_;
    };
  };

///////////////////////////////////////////////////////////////////////////////

  struct t_user {
    union {
      named::t_int64 id;
      named::p_void  ptr;
      named::p_cvoid cptr;
    };
    t_user() : id(0) { }
    inline t_user(named::t_int64 _id)   : id  (_id)   { }
    inline t_user(named::p_void  _ptr)  : ptr (_ptr)  { }
    inline t_user(named::p_cvoid _cptr) : cptr(_cptr) { }
  };

///////////////////////////////////////////////////////////////////////////////

  class t_any final {
  public:
     t_any() = default;
     t_any(t_user);
     template<typename T>
     t_any(t_user, T&&);
     t_any(t_any&&);
    ~t_any();

    t_any& operator=(t_any&&);
    t_any(const t_any&)            = delete;
    t_any& operator=(const t_any&) = delete;

    t_any& assign(t_user);
    template<typename T>
    T&     assign(t_user, T&&);

    operator t_validity() const;
    t_user   get_user  () const;
    t_bool   same_type (const t_any&) const;
    t_bool   is_equal  (const t_any&) const;

    template<class T> T&        ref();
    template<class T> const T&  ref() const;
    template<class T> const T& cref() const;

  private:
    t_user       user_;
    t_erase_it_* store_  = nullptr;
  };

////////////////////////////////////////////////////////////////////////////////

  inline
  t_bool operator==(const t_any& lh, const t_any& rh) {
    return lh.is_equal(rh);
  }

  inline
  t_bool operator!=(const t_any& lh, const t_any& rh) {
    return !lh.is_equal(rh);
  }

////////////////////////////////////////////////////////////////////////////////

  inline
  t_any::t_any(t_user user) : user_{user} {
  }

  template<typename T>
  inline
  t_any::t_any(t_user user, T&& value)
    : user_{user}, store_(new t_store_<T>(std::forward<T>(value))) {
  }

  inline
  t_any::t_any(t_any&& any) : user_{any.user_}, store_{any.store_} {
    any.store_   = nullptr;
    any.user_.id = 0;
  }

  inline
  t_any& t_any::operator=(t_any&& any) {
    if (store_)
      delete store_;
    user_  = any.user_;
    store_ = any.store_;
    return *this;
  }

  inline
  t_any::~t_any() {
    if (store_)
      delete store_;
  }

  inline
  t_any::operator t_validity () const {
    return store_ ? VALID : INVALID;
  }

  inline
  t_user t_any::get_user() const {
    return user_;
  }

  inline
  t_bool t_any::same_type(const t_any& any) const {
    if (user_.id == any.user_.id) {
       if (store_ && any.store_)
         return same_type_(*store_, *any.store_);
       if (!store_ && !any.store_)
         return true;
    }
    return false;
  }

  inline
  t_bool t_any::is_equal(const t_any& any) const {
    if (user_.id == any.user_.id) {
       if (store_ && any.store_)
         return store_->is_equal(*any.store_);
       if (!store_ && !any.store_)
         return true;
    }
    return false;
  }

  inline
  t_any& t_any::assign(t_user user) {
    if (store_) {
      delete store_;
      store_ = nullptr;
    }
    user_ = user;
    return *this;
  }

  template<typename T>
  inline
  T& t_any::assign(t_user user, T&& value) {
    if (store_)
      delete store_;
    user_  = user;
    store_ = new t_store_<T>(std::forward<T>(value));
    return ref<T>();
  }

  template<typename T>
  inline
  T& t_any::ref() {
    return (static_cast<t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_any::ref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

  template<typename T>
  inline
  const T& t_any::cref() const {
    return (static_cast<const t_store_<T>*>(store_))->value_;
  }

////////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
