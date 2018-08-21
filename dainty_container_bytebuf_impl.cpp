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

#include <stdlib.h>
#include <string.h>
#include "dainty_named_assert.h"
#include "dainty_container_bytebuf_impl.h"

namespace dainty
{
namespace container
{
namespace bytebuf
{
  using named::P_cstr;

//////////////////////////////////////////////////////////////////////////////

  p_byte alloc_(t_n_ n) {
    p_byte byte = nullptr;
    if (n) {
      byte = (p_byte)::malloc(n);
      if (!byte)
        named::assert_now(P_cstr(""));
    } else
      named::assert_now(P_cstr(""));
    return byte;
  }

  t_void dealloc_(p_byte byte) {
    if (byte)
      ::free(byte);
    else
      named::assert_now(P_cstr(""));
  }

  t_void copy_(p_byte dst, t_n_ dst_n, P_byte src, t_n_ src_n) {
    if (src_n <= dst_n)
      ::memcpy(dst, src, src_n);
    else
      named::assert_now(P_cstr(""));
  }

///////////////////////////////////////////////////////////////////////////////
}
}
}
