#include <stdio.h>
#include "asm.h"
#include "util.h"
#include "unapi.h"
#include "header/version.h"

#define BUFFER ((byte*)(0x8000))

byte* pbuffer = BUFFER;

const char* app_version= "HOST %s\r\n";

const char* app_usage=
    "%s\r\n"
    "Usage: host [/h] <hostname>\r\n"
    "Resolve hostname to an IPV4 address\r\n"
    "\r\n"
    "/h: display this help and exit\r\n"
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
    char paramLetter;
    int param;
    char* server = '\0';
    DNS dns;

    if (argc == 0) {
        printf("too few arguments\r\n");
        usage();
    }

    for(param=0; param<argc; param++) {
        if(argv[param][0] == '/') {
            paramLetter = lcase(argv[param][1]);
            if(paramLetter == 'h') {
                usage();
            }
        } else {
            server = argv[param];
        }
    }

    if ( server == '\0' ) {
        printf("Missing parameter\r\n");
        usage;
    }

    host(server, &dns);

    if (dns.code == 0) {
        printf("%i.%i.%i.%i\r\n", dns.subnet1, dns.subnet2, dns.subnet3, dns.subnet4);
    } else {
        printf("%i: %s\r\n", dns.code, code_error_message(dns.code));
    }

    exit();

    return 0;
}
