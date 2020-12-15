// 
// -----------------------------------------------------------------------------
// common-string.h
// -----------------------------------------------------------------------------
//
// Common string functions across Biblio project 
//

#ifndef STRING_INCLUDED
#define STRING_INCLUDED

#include <string.h>

extern int strmatch(const char *str, const char *target);
extern int extmatch(const char *path, const char *ext);

#endif
