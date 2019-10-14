#include <stdio.h>
#include <stdlib.h>
#include "asm.h"
#include "timestamp.h"
#include "util.h"
#include "unapi.h"
#include "header/version.h"

#define BUFFER ((byte*)(0x8000))

#define _GENV 0x6B
#define _TERM0 0
#define _SDATE 0x2B
#define _STIME 0x2D

#define SNTP_PORT 123
#define TICKS_TO_WAIT (3*50)
#define SYSTIMER ((uint*)0xFC9E)

const char* app_version=
    "sntp %s\r\n"
    "for the TCP/IP UNAPI 1.1\r\n";

const char* app_usage=
    "%s\r\n"
    "Usage: sntp [<host>] [/z <time zone>] [/r <retries>] [/d] [/v] [/h]\r\n"
    "\r\n"
    "<host>: Name or IP address of the SNTP time server.\r\n"
    "        If nothing is specified, the environment item TIMESERVER will be used.\r\n"
    "/z <time zone>: Formatted as [+|-]hh:mm where hh=00-12, mm=00-59\r\n"
    "    This value will be added or substracted from the received time.\r\n"
    "    The time zone can also be specified in the environment item TIMEZONE.\r\n"
    "/r <retries>: Number of retries to get information from time server (Default:1)\r\n"
    "/d: Do not change MSX clock, only display the received value\r\n"
    "/v: Verbose mode\r\n"
    "/h: This help\r\n"
    "\r\n";

const char* strInvalidParameter = "Invalid parameter(s)";
const char* strNoNetwork = "No network connection available";
const char* strInvalidTimeZone = "Invalid time zone";
const char* strInvalidRetriesSize = "Retries must be between 0 and 10";
const char* strOK = "OK\r\n";

Z80_registers regs;
int i;
int param;
unapi_code_block codeBlock;
int verbose;
int displayOnly;
char paramLetter;
uint conn;
byte* buffer;
int year;
uint timeZoneSeconds;
uint timeZoneHours;
uint timeZoneMinutes;
int ticksWaited;
int sysTimerHold;
int retries;

byte* timeServerBuffer;
byte timeZoneBuffer[8];
byte paramsBlock[8];

int IsValidTimeZone(byte* timeZoneString);
int IsDigit(char theChar);
unsigned long getTime(DNS *dns);
unsigned long queryTimeServer(DNS *dns, int *error_code);
void CheckYear();
void CloseConnection();
void Terminate(char* errorMessage);
char* TimeServerEnv();
char* TimeZoneEnv();

byte* pbuffer = BUFFER;

static void exit() {
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

char* TimeServerEnv()
{
  timeServerBuffer = BUFFER;
  timeServerBuffer[0] = '\0';
  regs.Words.HL = (int)"TIMESERVER";
  regs.Words.DE = (int)timeServerBuffer;
  regs.Bytes.B = 255;
  DosCall(_GENV, &regs, REGS_MAIN, REGS_AF);

  return timeServerBuffer;
}

char* TimeZoneEnv()
{
  timeZoneBuffer[0] = '\0';
  regs.Words.HL = (int)"TIMEZONE";
  regs.Words.DE = (int)timeZoneBuffer;
  regs.Bytes.B = 8;
  DosCall(_GENV, &regs, REGS_MAIN, REGS_AF);

  return (IsValidTimeZone(timeZoneBuffer) ? timeZoneBuffer : "+00:00");
}

unsigned long getTime(DNS *dns)
{
    int queryErrorCode;
    int currentTry;
    unsigned long seconds = 0;

    currentTry = 0;

    vprintf("Querying the time server...");

    do {
        vprintf(".");
        currentTry++;
        seconds = queryTimeServer(dns, &queryErrorCode);
    } while (queryErrorCode != 0 && currentTry <= retries);

    switch(queryErrorCode) {
        case 1 :
            sprintf(buffer, "Unknown error when sending request to time server (code %i)", regs.Bytes.A);
            Terminate(buffer);
            break;
        case 2 :
            sprintf(buffer, "Unknown error when waiting a reply from the time server (code %i)", regs.Bytes.A);
            Terminate(buffer);
            break;
        case 3 :
            Terminate("The server returned a too short packet");
            break;
        case 4 :
            Terminate("The server returned a \"Kiss of death\" packet\r\n(are you querying the server too often?)");
            break;
    }

    return seconds;
}

unsigned long queryTimeServer(DNS *dns, int *error_code) {
    unsigned long seconds = 0;
    //* Open a new UDP connection and send request
    *buffer=0x1B;
    for(i=1; i<48; i++) {
        buffer[i]=0;
    }

    paramsBlock[0] = dns->subnet1;
    paramsBlock[1] = dns->subnet2;
    paramsBlock[2] = dns->subnet3;
    paramsBlock[3] = dns->subnet4;

    paramsBlock[4] = SNTP_PORT;
    paramsBlock[5] = 0;
    paramsBlock[6] = 48;
    paramsBlock[7] = 0;

    regs.Bytes.B = conn;
    regs.Words.HL=(int)buffer;
    regs.Words.DE=(int)paramsBlock;

    UnapiCall(&codeBlock, TCPIP_UDP_SEND, &regs, REGS_MAIN, REGS_MAIN);
    if(regs.Bytes.A != 0) {
        *error_code = 1;
        return seconds;
    }

    //* Wait for a reply and check that it is correct
    ticksWaited = 0;
    do {
        sysTimerHold = *SYSTIMER;

        UnapiCall(&codeBlock, TCPIP_WAIT, &regs, REGS_MAIN, REGS_NONE);
        while(*SYSTIMER == sysTimerHold);

        ticksWaited++;
        if(ticksWaited >= TICKS_TO_WAIT) {
            break;
        }
        regs.Bytes.B = conn;
        regs.Words.HL = (int)buffer;
        regs.Words.DE = 48;
        UnapiCall(&codeBlock, TCPIP_UDP_RCV, &regs, REGS_MAIN, REGS_MAIN);
    } while(regs.Bytes.A == ERR_NO_DATA);

    if(regs.Bytes.A != 0) {
        *error_code = 2;
        return 0;
    }

    if(regs.UWords.BC < 48) {
        *error_code = 3;
        return 0;
    }

    if(buffer[1] == 0) {
        *error_code = 4;
        return 0;
    }

    vprintf(strOK);

    if(buffer[0] & 0xC0 == 0xC0 && verbose) {
        printf("WARNING: Error returned by server: clock is not synchronized\r\n");
    }

    ((byte*)&seconds)[0]=buffer[43];
    ((byte*)&seconds)[1]=buffer[42];
    ((byte*)&seconds)[2]=buffer[41];
    ((byte*)&seconds)[3]=buffer[40];

    return seconds;
}

int IsValidTimeZone(byte* timeZoneString)
{
    if (timeZoneString == NULL) return 0;

    if(!(timeZoneString[0]==(byte)'+' || timeZoneString[0]==(byte)'-')) {
        return 0;
    }

    if(!(IsDigit(timeZoneString[1]) && IsDigit(timeZoneString[2]) && IsDigit(timeZoneString[4]) && IsDigit(timeZoneString[5])))
    {
        return 0;
    }

    if(timeZoneString[3] != (byte)':' || timeZoneString[6] != 0)
    {
        return 0;
    }

    return 1;
}


int IsDigit(char theChar)
{
    return (theChar>='0' && theChar<='9');
}


void Terminate(char* errorMessage)
{
    if(errorMessage != NULL) {
        if(verbose) {
            printf("\r\n*** ERROR: %s\r\n", errorMessage);
        } else {
            printf("*** SNTP ERROR: %s\r\n", errorMessage);
        }
    }

    CloseConnection();

    exit();
}


void CheckYear()
{
    if(year < 2010) {
        Terminate("The server returned a date that is before year 2010");
    }

    if(year > 2079) {
        Terminate("The server returned a date that is after year 2079");
    }
}

void CloseConnection()
{
    regs.Bytes.B = conn;
    UnapiCall(&codeBlock, TCPIP_UDP_CLOSE, &regs, REGS_MAIN, REGS_NONE);
}

void initializeCodeBlock() {
    int i = UnapiGetCount("TCP/IP");
    if (i==0) {
        Terminate("No TCP/IP UNAPI implementations found");
    }
    UnapiBuildCodeBlock(NULL, i, &codeBlock);
}

void parseTimeZone(byte* timeZoneString) {
    if(IsValidTimeZone(timeZoneString)) {
        timeZoneHours = (((byte)(timeZoneString[1])-'0')*10) + (byte)(timeZoneString[2]-'0');
        if(timeZoneHours > 12) {
            Terminate(strInvalidTimeZone);
        }

        timeZoneMinutes = (((byte)(timeZoneString[4])-'0')*10) + (byte)(timeZoneString[5]-'0');
        if(timeZoneMinutes > 59) {
            Terminate(strInvalidTimeZone);
        }

        timeZoneSeconds = (((timeZoneHours * (int)SECS_IN_HOUR)) + ((timeZoneMinutes * (int)SECS_IN_MINUTE))) * (timeZoneString[0] == '-' ? -1 : 1);
    }
}

int main(char** argv, int argc)
{
    char paramLetter;
    int skipNext;
    char* timeZoneString;
    char* timeServerString;
    unsigned long long seconds;
    DATE_TIME date_time;
    DNS dns;

    //* Initialize variables
    skipNext = 0;
    verbose = 0;
    displayOnly = 0;
    timeZoneString = TimeZoneEnv();
    timeServerString = TimeServerEnv();
    buffer = BUFFER;
    conn = 0;
    retries = 1;

    if(argc == 0 && timeServerString[0] == '\0') {
        usage();
    }

    for(param=0; param<argc; param++) {
        if (skipNext == 1) {
            skipNext = 0;
            continue;
        }
        if(argv[param][0] == '/') {
            paramLetter = lcase(argv[param][1]);
            if(paramLetter == 'v') {
                enable_verbose();
            } else if(paramLetter == 'd') {
                displayOnly = 1;
            } else if(paramLetter == 'r') {
                retries = atoi(argv[param + 1]);
                skipNext = 1;
                if(retries < 0 || retries > 10) {
                    Terminate(strInvalidRetriesSize);
                }
            } else if(paramLetter == 'z') {
                timeZoneString = argv[param + 1];
                skipNext = 1;
                if(!IsValidTimeZone(timeZoneString)) {
                    Terminate(strInvalidTimeZone);
                }
            } else if(paramLetter == 'h') {
                usage();
            } else {
                Terminate(strInvalidParameter);
            }
        } else {
          timeServerString = argv[param];
        }
    }

    vprintf(version());

    parseTimeZone(timeZoneString);

    if(timeServerString[0] == '\0') {
        Terminate("No time server specified and no TIMESERVER environment item was found.");
    }

    sprintf(pbuffer, "Time server is: %s\r\n", timeServerString);
    vprintf(pbuffer);

    initializeCodeBlock();

    //* Initialize TCP/IP, close transient UDP connections, open connection for time server port
    CloseConnection();

    if(regs.Bytes.A == ERR_NOT_IMP) {
        Terminate("This TCP/IP UNAPI implementation does not support UDP connections");
    }

    regs.Words.HL = SNTP_PORT;
    regs.Bytes.B = 0;
    UnapiCall(&codeBlock, TCPIP_UDP_OPEN, &regs, REGS_MAIN, REGS_MAIN);
    if(regs.Bytes.A == ERR_NO_FREE_CONN) {
        Terminate("No free UDP connections available");
    }
    else if(regs.Bytes.A == ERR_CONN_EXISTS) {
        Terminate("There is a resident UDP connection which uses the SNTP port");
    }
    else if(regs.Bytes.A != 0) {
        sprintf(buffer, "Unknown error when opening UDP connection (code %i)", regs.Bytes.A);
        Terminate(buffer);
    }
    conn = regs.Bytes.B;

    host(timeServerString, &dns);

    sprintf(pbuffer, "OK, %i.%i.%i.%i\r\n", dns.subnet1, dns.subnet2, dns.subnet3, dns.subnet4);
    vprintf(pbuffer);

    seconds = getTime(&dns);

    printf("Seconds: %s", tstoa(seconds));
    CloseConnection();

    exit();

    ts_to_date(seconds, &date_time);

    printf("Ok 3");
    CheckYear();

    printf("Ok 4");
    sprintf(pbuffer, "Time returned by time server: %i-%i-%i, %i:%i:%i\r\n", date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
    vprintf(pbuffer);

    seconds += timeZoneSeconds;

    ts_to_date(seconds, &date_time);
    CheckYear();
    sprintf(pbuffer, "Time adjusted to time zone:   %i-%i-%i, %i:%i:%i\r\n", date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
    vprintf(pbuffer);

    //* Change the MSX clock if necessary

    if(displayOnly) {
        if(!verbose_on()) {
            printf("Time obtained from time server: %i-%i-%i, %i:%i:%i\r\n", date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
        }
    } else {
        regs.UWords.HL = date_time.year;
        regs.Bytes.D = date_time.month;
        regs.Bytes.E = date_time.day;
        DosCall(_SDATE, &regs, REGS_MAIN, REGS_AF);
        if(regs.Bytes.A != 0) {
            Terminate("Invalid date for the MSX clock");
        }

        regs.Bytes.H = date_time.hour;
        regs.Bytes.L = date_time.min;
        regs.Bytes.D = date_time.sec;
        DosCall(_STIME, &regs, REGS_MAIN, REGS_AF);
        if(regs.Bytes.A != 0) {
            Terminate("Invalid time for the MSX clock");
        }

        if(!verbose_on()) {
            printf("The clock has been set to the adjusted time.");
        } else {
            printf("The clock has been set to: %i-%i-%i, %i:%i:%i\r\n", date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
        }
    }

    Terminate(NULL);
    return 0;
}