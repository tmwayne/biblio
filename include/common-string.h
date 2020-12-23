// 
// -----------------------------------------------------------------------------
// common-string.h
// -----------------------------------------------------------------------------
//
// Common string functions.
//
// Tyler Wayne © 2020
//

#ifndef STRING_INCLUDED
#define STRING_INCLUDED

#include <string.h>

extern int   strmatch(const char *str, const char *target);
extern char *strtrim(char *str);
extern void  strlower(char *);
extern int   extmatch(const char *path, const char *ext);
extern char *pathcat(char *path1, char *path2);

#endif
