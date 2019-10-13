#include <stdlib.h>
#include "date_time.h"

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

extern unsigned int is_leap_year(int year) {
    return ((year & 3) == 0);
}

extern char* tstoa(unsigned long ts) {
    char base = 10;
    static char buffer[16];

    _ultoa(ts, buffer, base);

    return buffer;
}

unsigned long seconds_in_month(int month, int year) {
  switch(month) {
    case 2:
      return (is_leap_year(year) ? SECS_IN_MONTH_29 : SECS_IN_MONTH_28);
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

unsigned long months_seconds(int current_month, int year) {
    unsigned long seconds = 0;
    for( int month = 1; month < current_month; month++ ) {
        seconds = seconds + seconds_in_month(month, year);
    }
    return seconds;
}

unsigned long years_seconds(int current_year) {
    unsigned long seconds = 0;
    for( int year = EPOCH_YEAR; year < current_year; year++ ) {
        seconds = seconds + (is_leap_year(year) ? SECS_IN_LYEAR : SECS_IN_YEAR);
    }
    return seconds;
}

extern unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second) {
  return
    (years_seconds(year)) +
    (months_seconds(month, year)) +
    ((day-1) * SECS_IN_DAY) +
    (hour * SECS_IN_HOUR) +
    (minute * SECS_IN_MINUTE) +
    second;
}

extern void now(DATE_TIME *date_time){
  DATE date;
  TIME time;
  GetDate(&date);
  GetTime(&time);
  date_time->year = date.year;
  date_time->month = date.month;
  date_time->day = date.day;
  date_time->dow = date.dow;
  date_time->hour = time.hour;
  date_time->min = time.min;
  date_time->sec = time.sec;
}

extern unsigned long long now_ts() {
  DATE_TIME date_time;
  now(&date_time);
  return to_seconds(date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
}
