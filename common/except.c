//
// -----------------------------------------------------------------------------
// except.c
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
#include <stdio.h>
#include "assert.h"
#include "except.h"

Except_Frame *Except_stack = NULL;
extern const Except_T Assert_Failed;

void Except_raise(const Except_T *e, const char *file, int line) {

  Except_Frame *p = Except_stack;

  assert(e);

  if (p == NULL) {
    fprintf(stderr, "Uncaught exception: ");
    if (e->reason)
      fprintf(stderr, " %s", e->reason);
    else
      fprintf(stderr, " at 0x%p", e);
    if (file && line > 0)
      fprintf(stderr, " raised at %s:%d\n", file, line);
    // fprintf(stderr, "aborting...\n");
    fflush(stderr);
    abort();
  }

  p->exception = e;
  p->file = file;
  p->line = line;
  Except_stack = Except_stack->prev;
  longjmp(p->env, Except_raised);

}
