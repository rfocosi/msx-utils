/*
Compile with:
sdcc --code-loc 0x180 --data-loc 0 -mz80 --disable-warning 196 --no-std-crt0 $SDCC_LIB/crt0_msxdos_advanced.rel $SDCC_LIB/printf.rel $SDCC_LIB/putchar_msxdos.rel asm.lib fusion.lib sleep.c

And use:
hex2bin -e com sleep.ihx

To generate 'sleep.com'

*/

#include <stdio.h>
#include <stdlib.h>
#include "asm.h"
#include "msx_fusion.h"

#define lcase(c) ((c) | 32)
#define vprintf(msg) if(verbose_mode == 1) { printf(msg); }

#define EPOCH_YEAR ((unsigned long) 1970L)
#define SECS_IN_MINUTE ((unsigned long)(60L))
#define SECS_IN_HOUR ((unsigned long)(SECS_IN_MINUTE * 60L))
#define SECS_IN_DAY ((unsigned long)(SECS_IN_HOUR * 24L))
#define SECS_IN_MONTH_28 ((unsigned long)(SECS_IN_DAY * 28L))
#define SECS_IN_MONTH_29 ((unsigned long)(SECS_IN_MONTH_28 + SECS_IN_DAY))
#define SECS_IN_MONTH_30 ((unsigned long)(SECS_IN_MONTH_29 + SECS_IN_DAY))
#define SECS_IN_MONTH_31 ((unsigned long)(SECS_IN_MONTH_30 + SECS_IN_DAY))
#define SECS_IN_YEAR ((unsigned long)(SECS_IN_DAY * 365L))
#define SECS_IN_LYEAR ((unsigned long)(SECS_IN_YEAR + SECS_IN_DAY))

#define BUFFER ((byte*)(0x8000))

static int show_help;
static int verbose_mode;

static unsigned long current_seconds = 0l;
static unsigned long scheduled_seconds = 0l;

byte* pbuffer = BUFFER;

Z80_registers regs;
TIME time;
DATE date;


const char* app_version= "SLEEP v1.0\r\n";

const char* app_usage=
    "%s"
    "\r\n"
    "Usage: SLEEP [/V] [/H] <NUMBER>\r\n"
    "Pause for NUMBER seconds.\r\n"
    "\r\n"
    "/H: display this help and exit\r\n"
    "/V: verbose mode\r\n"
    "\r\n";

static void exit() {
  DosCall(0, &regs, REGS_MAIN, REGS_NONE);
}

static void usage () {
  printf(app_usage, app_version);
  exit();
}

static unsigned long to_seconds(int year, int month, int day, int hour, int minute, int second) {
  int is_leap_year = ((year & 3) == 0);
  unsigned long SecondsInYears = (year - EPOCH_YEAR) * (is_leap_year ? SECS_IN_LYEAR : SECS_IN_YEAR);
  unsigned long SecondsInMonths = month * SECS_IN_MONTH_28;
  unsigned long SecondsInDays = day * SECS_IN_DAY;
  unsigned long SecondsInHours = hour * SECS_IN_HOUR;
  unsigned long SecondsInMinutes = minute * SECS_IN_MINUTE;

  return
    SecondsInYears +
    SecondsInMonths +
    SecondsInDays +
    SecondsInHours +
    SecondsInMinutes +
    second;
}

static void time_check() {
  GetTime(&time);
  GetDate(&date);
  current_seconds = to_seconds(date.year, date.month, date.day, time.hour, time.min, time.sec);
}

static void sleep (unsigned long seconds) {
  vprintf(app_version);
  GetTime(&time);
  GetDate(&date);
  sprintf(pbuffer, "Started at: %i-%i-%i %i:%i:%i\r\n", date.year, date.month, date.day, time.hour, time.min, time.sec);
  vprintf(pbuffer);
  scheduled_seconds = to_seconds(date.year, date.month, date.day, time.hour, time.min, time.sec) + seconds;

  while (scheduled_seconds > current_seconds) time_check();
  sprintf(pbuffer, "Finished at: %i-%i-%i %i:%i:%i\r\n", date.year, date.month, date.day, time.hour, time.min, time.sec);
  vprintf(pbuffer);
}

int main (char** argv, int argc) {
  unsigned long seconds = 0l;
  char paramLetter;
  int param;

  if (argc == 0) {
      printf("too few arguments\r\n");
      usage ();
  }

  for(param=0; param<argc; param++) {
    if(argv[param][0] == '/') {
      paramLetter = lcase(argv[param][1]);
      if(paramLetter == 'v') {
          verbose_mode = 1;
      } else if (paramLetter == 'h') {
        show_help = 1;
      }
    } else {
      seconds = atoi(argv[param]);
    }
  }

  if (show_help == 1)
    usage ();

  sleep (seconds);

  exit ();

  return 0;
}
