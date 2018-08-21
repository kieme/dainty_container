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

#ifndef _DAINTY_CONTAINER_BYTEBUF_IMPL_H_
#define _DAINTY_CONTAINER_BYTEBUF_IMPL_H_

#include "dainty_named.h"
#include "dainty_container_err.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
//////////////////////////////////////////////////////////////////////////////

  using named::t_validity;
  using named::t_void;
  using named::t_bool;
  using named::t_ix_;
  using named::t_ix;
  using named::t_n_;
  using named::t_n;

  using named::INVALID;
  using named::VALID;

  using t_byte = named::t_uchar;
  using p_byte = named::t_prefix<t_byte>::p_;
  using P_byte = named::t_prefix<t_byte>::P_;
  using r_byte = named::t_prefix<t_byte>::r_;
  using R_byte = named::t_prefix<t_byte>::R_;

///////////////////////////////////////////////////////////////////////////////

  p_byte alloc_  (t_n_);
  t_void dealloc_(p_byte);
  t_void copy_   (p_byte, t_n_, P_byte, t_n_);

///////////////////////////////////////////////////////////////////////////////
}
}
}

#endif
