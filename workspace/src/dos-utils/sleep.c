#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "asm.h"
#include "msx_fusion.h"
#include "header/version.h"

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

static unsigned long long current_seconds = 0l;
static unsigned long long scheduled_seconds = 0l;

byte* pbuffer = BUFFER;

Z80_registers regs;
TIME time;
DATE date;

const char* app_version= "SLEEP %s \r\n";

const char* app_usage=
    "%s\r\n"
    "Usage: SLEEP [/V] [/H] <NUMBER>\r\n"
    "Pause for NUMBER seconds.\r\n"
    "\r\n"
    "/H: display this help and exit\r\n"
    "/V: verbose mode\r\n"
    "\r\n";

static void exit() {
  DosCall(0, &regs, REGS_MAIN, REGS_NONE);
}

static char* version() {
  sprintf(pbuffer, app_version, TOOLS_VERSION);
  return pbuffer;
}

static void usage () {
  printf(app_usage, version());
  exit();
}

static unsigned long seconds_in_month(int month, int is_leap_year) {
  switch(month) {
    case 2:
      return (is_leap_year ? SECS_IN_MONTH_29 : SECS_IN_MONTH_28);
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return SECS_IN_MONTH_31;
  }
  return SECS_IN_MONTH_30;
}

static unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second) {
  int is_leap_year = ((year & 3) == 0);
  unsigned long long SecondsInYears = (year - EPOCH_YEAR) * (is_leap_year ? SECS_IN_LYEAR : SECS_IN_YEAR);
  unsigned long long SecondsInMonths = month * seconds_in_month(month, is_leap_year);
  unsigned long long SecondsInDays = day * SECS_IN_DAY;
  unsigned long long SecondsInHours = hour * SECS_IN_HOUR;
  unsigned long long SecondsInMinutes = minute * SECS_IN_MINUTE;

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
  vprintf(version());
  GetTime(&time);
  GetDate(&date);
  sprintf(pbuffer, "Started at: %i-%i-%i %i:%i:%i\r\n", date.year, date.month, date.day, time.hour, time.min, time.sec);
  vprintf(pbuffer);
  scheduled_seconds = to_seconds(date.year, date.month, date.day, time.hour, time.min, time.sec) + seconds;

  while (scheduled_seconds > current_seconds) time_check();
  sprintf(pbuffer, "Finished at: %i-%i-%i %i:%i:%i\r\n", date.year, date.month, date.day, time.hour, time.min, time.sec);
  vprintf(pbuffer);
}

static int isnumber (char* str) {
  int c = 0;
  for(c=0; c < sizeof(str); c++) {
    if (str[c] == '\0') break;
    if (!isdigit(str[c])) return -1;
  }
  return 0;
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
    } else if (isnumber(argv[param]) == 0) {
      seconds = atoi(argv[param]);
    } else {
      printf("invalid parameter\r\n");
      usage ();
    }
  }

  if (seconds < 1) {
    printf("Seconds must be greater than 0\r\n");
    usage ();
  }

  if (show_help == 1)
    usage ();

  sleep (seconds);

  exit ();

  return 0;
}
