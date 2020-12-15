//
// -----------------------------------------------------------------------------
// common-string.c
// -----------------------------------------------------------------------------
//
// Common string functions across Biblio project 
//

#include "common-string.h"

int strmatch(const char *str, const char *target) {

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

int extmatch(const char *path, const char *ext) {

  int path_len = strlen(path);
  int ext_len = strlen(ext);

  if (path && ext && ext_len > path_len)
    return 0;
  else 
    return strcmp(path + (path_len - ext_len), ext) ? 0 : 1;
  
}
