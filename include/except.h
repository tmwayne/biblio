// 
// -----------------------------------------------------------------------------
// except.h
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

#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED

#include <setjmp.h>

typedef struct Except_T {
  const char *reason;
} Except_T;

typedef struct Except_Frame Except_Frame;

struct Except_Frame {
  Except_Frame *prev;
  jmp_buf env;
  const char *file;
  int line;
  const Except_T *exception;
};

enum { Except_entered=0, Except_raised,
       Except_handled,   Except_finalized };

extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;

void Except_raise(const Except_T *e, const char *file, int line);

#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)

#define RERAISE Except_raise(Except_frame.exception, \
  Except_frame.file, Except_frame.line)

// #define RETURN switch (0) default: return

#define TRY do { \
  volatile int Except_flag; \
  Except_Frame Except_frame; \
  Except_frame.prev = Except_stack; \
  Except_stack = &Except_frame; \
  Except_flag = setjmp(Except_frame.env); \
  if (Except_flag == Except_entered) {

#define EXCEPT(e) \
    if (Except_flag == Except_entered) \
      Except_stack = Except_stack->prev; \
  } else if (Except_frame.exception == &(e)) { \
    Except_flag = Except_handled;

#define ELSE \
    if (Except_flag == Except_entered) \
      Except_stack = Except_stack->prev; \
  } else { \
    Except_flag = Except_handled;

#define FINALLY \
    if (Except_flag == Except_entered) \
      Except_stack = Except_stack->prev; \
    } { \
      if (Except_flag == Except_entered); \
        Except_flag = Except_finalized;

#define END_TRY \
    if (Except_flag == Except_entered) \
      Except_stack = Except_stack->prev; \
    } if (Except_flag == Except_raised) \
        RERAISE; \
} while (0)

#endif
