//
// -----------------------------------------------------------------------------
// common-string.c
// -----------------------------------------------------------------------------
//
// Common string functions across Biblio project 
//

#include <limits.h>
#include <ctype.h>
#include "common-string.h"
#include "mem.h"
#include "assert.h"

int strmatch(const char *str, const char *target) {

  assert(str && target);

  // If strncmp returns 0 then the strings are the same so return 1
  return strncmp(str, target, strlen(target)) ? 0 : 1;

}

int extmatch(const char *path, const char *ext) {

  assert(path && ext);

  int path_len = strlen(path);
  int ext_len = strlen(ext);

  if (path && ext && ext_len > path_len)
    return 0;
  else 
    return strcmp(path + (path_len - ext_len), ext) ? 0 : 1;
  
}

char *strtrim(char *str, int len) {

  assert(str);

  char *start = NULL;

  for (int i=0; str[i] && i<len; i++) {
    if (!start && !isspace(str[i])) {
      start = str + i;
      continue;
    } else if (start && isspace(str[i])) {
      str[i] = 0;
      break;
    }
  }

  return start;

}

void strlower(char *str) {
  assert(str);
  for ( ; *str; ++str) *str = tolower(*str);
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

  path = 0;

  return head;

}
