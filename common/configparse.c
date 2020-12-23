//
// -----------------------------------------------------------------------------
// parse-config.c
// -----------------------------------------------------------------------------
//
// Description
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "common-string.h"
#include "dict.h"
#include "mem.h"
#include "assert.h"

static int parse_line(char *line, char **key, char **val) {

    assert(line && key && val);

    char *k = NULL, *v = NULL;
    bool is_key = true;

    while (*line) {
      if (*line == '#') {                   // comment variable
        if (!k && !v) return -1;            // comment at beginning of line
        else if (k && v) *line = '\0';      // comment after key and val are defined
        else return 0;                      // invalid co
      }
      else if (*line == '=') {              // if at delimiter
        if (k == NULL) return 0;            // check that key was set
        else *line = '\0', is_key = false;  // if it was set null-terminator
      } else if (is_key && !k) k = line;
      else if (!is_key && !v) v = line;

      line++;
    }

    *key = strtrim(k), *val = strtrim(v);

    return 1;

}

void load_configs(Dict_T configs, char *path) {

  assert(configs && path);

  FILE* fd = fopen(path, "r");
  if (!fd) {
    fprintf(stderr, "Unable to open configs\n");
    exit(EXIT_FAILURE);
  }

  int buflen = 256;
  char *buf = ALLOC(buflen*sizeof(char));
  int linenum = 0;

  while (fgets(buf, buflen, fd)) {

    char *key, *val;
    int exit_code;
    exit_code = parse_line(buf, &key, &val);

    if (exit_code == 0) {
      fprintf(stderr, "Synax error, lineno %d\n", linenum);
      exit(EXIT_FAILURE);
    } else if (exit_code == 1) {
      strlower(key);
      Dict_set(configs, key, val);
    }

    linenum++;

  }

}
   
#ifdef CONFIG_DEBUG
void Dict_print(Dict_T);

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  Dict_T configs = Dict_new();
  load_configs(configs, argv[1]);
  
  Dict_print(configs);

}
#endif
