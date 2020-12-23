//
// -----------------------------------------------------------------------------
// mem.c
// -----------------------------------------------------------------------------
//
// Copyright (c) 1994,1995,1996,1997 by David R. Hanson.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// <http://www.opensource.org/licenses/mit-license.php>
//

#include <stdlib.h>
#include <stddef.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

// Data
const Except_T Mem_Failed = { "Allocation Failed" };

// Functions
void *Mem_alloc(long nbytes, const char *file, int line) {
  void *ptr;

  assert(nbytes > 0);
  ptr = malloc(nbytes);
  if (ptr == NULL) {
    if (file == NULL) RAISE(Mem_Failed);
    else Except_raise(&Mem_Failed, file, line);
  }
  return ptr;
}

void *Mem_calloc(long count, long nbytes, const char *file, int line) {
  void *ptr;

  assert(count > 0);
  assert(nbytes > 0);
  ptr = calloc(count, nbytes);
  if (ptr == NULL) {
    if (file == NULL) RAISE(Mem_Failed);
    else Except_raise(&Mem_Failed, file, line);
  }
  return ptr;
}

void Mem_free(void *ptr, const char *file, int line) {
  if (ptr) free(ptr);
}

void *Mem_resize(void *ptr, long nbytes, const char *file, int line) {
  assert(ptr);
  assert(nbytes > 0);
  ptr = realloc(ptr, nbytes);
  if (ptr == NULL) {
    if (file == NULL) RAISE(Mem_Failed);
    else Except_raise(&Mem_Failed, file, line);
  }
  return ptr;
}


