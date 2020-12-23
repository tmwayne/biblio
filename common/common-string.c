//
// -----------------------------------------------------------------------------
// common-string.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2020
//

#include <limits.h>
#include <ctype.h>
#include "common-string.h"
#include "mem.h"
#include "assert.h"

int strmatch(const char *str, const char *target) {

  assert(str && target);

  do {
    if (*str != *target) return 0;
  } while (*str++ && *target++);

  return 1;

}

int extmatch(const char *path, const char *ext) {

  assert(path && ext);

  int path_len = strlen(path);
  int ext_len = strlen(ext);

  if (ext_len == 0 || ext_len > path_len)
    return 0;

  path += (path_len - ext_len);

  return strmatch(path, ext);
  
}

char *strtrim(char *str) {

  assert(str);
  char *start = NULL, *end = str;

  for ( ; *str ; str++) {
    if (!isspace(*str)) {
      if (!start) start = str;
      else end = str+1;
    }
  }
  *end = '\0';
  return start ? start : end;

}

void strlower(char *str) {
  assert(str);
  for ( ; *str; str++) *str = tolower(*str);
}

char *pathcat(char *path1, char *path2) {

  int path_max = PATH_MAX - 1; // Ensure we have room for null-terminator
  char *path = CALLOC(PATH_MAX, sizeof(char));
  char *head = path;

  // Copy first path
  for ( ; path_max && *path1; path_max--, path++, path1++)
    *path = *path1;

  // Ensure there is one and only one slash between paths
  if (*(path1-1) != '/' && *path2 != '/')
    *path++ = '/';
  else if (*(path1-1) == '/' && *path2 == '/')
    path2++;

  // Copy second path
  for ( ; path_max && *path2; path_max--, path++, path2++)
    *path = *path2;

  // Add null-terminator
  path = NULL;

  return head;

}
