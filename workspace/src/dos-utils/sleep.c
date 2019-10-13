#include <stdio.h>
#include <stdlib.h>
#include "asm.h"
#include "timestamp.h"
#include "util.h"
#include "header/version.h"

#define BUFFER ((byte*)(0x8000))

byte* pbuffer = BUFFER;

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
  Z80_registers regs;
  DosCall(0, &regs, REGS_MAIN, REGS_NONE);
}

static char* version() {
  sprintf(pbuffer, app_version, TOOLS_VERSION);
  return pbuffer;
}

static void usage() {
  printf(app_usage, version());
  exit();
}

static void display_date(char* info) {
  if (verbose_on()) {
    DATE_TIME date_time;

    now(&date_time);
    printf("%s %i-%i-%i %i:%i:%i\r\n", info, date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
  }
}
static void sleep (unsigned long seconds) {
  unsigned long long current_seconds = 0l;
  unsigned long long scheduled_seconds = 0l;

  vprintf(version());

  display_date("Started at:");

  scheduled_seconds = now_ts() + seconds;

  while (scheduled_seconds > current_seconds) current_seconds = now_ts();

  display_date("Finished at:");
}

int main (char** argv, int argc) {
  unsigned long seconds = 0l;
  char paramLetter;
  int param;

  if (argc == 0) {
      printf("too few arguments\r\n");
      usage();
  }

  for(param=0; param<argc; param++) {
    if(argv[param][0] == '/') {
      paramLetter = lcase(argv[param][1]);
      if(paramLetter == 'v') {
          enable_verbose();
      } else if (paramLetter == 'h') {
        usage();
      }
    } else if (is_number(argv[param]) == 0) {
      seconds = atoi(argv[param]);
    } else {
      printf("invalid parameter\r\n");
      usage();
    }
  }

  if (seconds < 1) {
    printf("Seconds must be greater than 0\r\n");
    usage();
  }

  sleep(seconds);

  exit();

  return 0;
}
