#include <stdio.h>
#include "asm.h"
#include "timestamp.h"
#include "util.h"
#include "header/version.h"

#define BUFFER ((byte*)(0x8000))

byte* pbuffer = BUFFER;

const char* app_version= "NOW %s\r\n";

const char* app_usage=
    "%s\r\n"
    "Usage: now [/h] [/d] [/t] [/s]\r\n"
    "Show current date and time\r\n"
    "\r\n"
    "/h: display this help and exit\r\n"
    "/d: show date\r\n"
    "/t: show time\r\n"
    "/s: show seconds from epoch time (1970-01-01)\r\n"
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

int main (char** argv, int argc) {
    DATE_TIME date_time;
    unsigned long long now = now_ts();

    char paramLetter;
    int param;
    unsigned int show_date = 0;
    unsigned int show_time = 0;
    unsigned int show_timestamp = 0;

    if (argc == 0) {
        show_date = 1;
        show_time = 1;
        show_timestamp = 0;
    }

    for(param=0; param<argc; param++) {
        if(argv[param][0] == '/') {
            paramLetter = lcase(argv[param][1]);
            if(paramLetter == 'h') {
                usage();
            } else if (paramLetter == 'd') {
                show_date = 1;
            } else if (paramLetter == 't') {
                show_time = 1;
            } else if (paramLetter == 's') {
                show_timestamp = 1;
            }
        } else {
            printf("invalid parameter\r\n");
            usage();
        }
    }

    ts_to_date(now, &date_time);

    if (show_date) printf("%i-%i-%i ", date_time.year, date_time.month, date_time.day);

    if (show_time) printf("%i:%i:%i ", date_time.hour, date_time.min, date_time.sec);

    if (show_timestamp) printf("%s", tstoa(now));

    printf("\r\n");

    exit();

    return 0;
}
