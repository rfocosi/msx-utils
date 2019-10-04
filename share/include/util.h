#define lcase(c) ((c) | 32)
#define vprintf(msg) if (verbose_on()) { printf(msg); }

extern int verbose_on();

extern void enable_verbose();

extern int is_number (char* str);
