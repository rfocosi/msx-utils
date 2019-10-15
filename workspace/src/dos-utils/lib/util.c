#include <ctype.h>

int verbose_mode;

extern int verbose_on() {
  return verbose_mode;
}

extern void enable_verbose() {
  verbose_mode = 1;
}

extern int is_number (char* str) {
  int c = 0;
  for(c=0; c < sizeof(str); c++) {
    if (str[c] == '\0') break;
    if (!isdigit(str[c])) return 0;
  }
  return 1;
}
