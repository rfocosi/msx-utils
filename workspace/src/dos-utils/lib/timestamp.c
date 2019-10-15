#include <stdlib.h>
#include "../header/timestamp.h"

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

extern unsigned long to_seconds(int year, int month, int day, int hour, int minute, int second) {
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

extern unsigned long now_ts() {
  DATE_TIME date_time;
  now(&date_time);
  return to_seconds(date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
}

extern void ts_to_date(unsigned long seconds, DATE_TIME* date_time) {
    date_time->year = EPOCH_YEAR;

    while(1) {
        if((!is_leap_year(date_time->year) && seconds < SECS_IN_YEAR) || (is_leap_year(date_time->year) && seconds < SECS_IN_LYEAR)) {
            break;
        }
        seconds -= (is_leap_year(date_time->year) ? SECS_IN_LYEAR : SECS_IN_YEAR);
        date_time->year++;
    }

    date_time->month = 1;

    while(seconds > seconds_in_month(date_time->month, date_time->year)) {
        seconds -= seconds_in_month(date_time->month, date_time->year);
        date_time->month++;
    }

    date_time->day = 1;

     while(seconds > SECS_IN_DAY) {
         seconds -= SECS_IN_DAY;
         date_time->day++;
     }

     date_time->hour = 0;

     while(seconds > SECS_IN_HOUR) {
         seconds -= SECS_IN_HOUR;
         date_time->hour++;
     }

     date_time->min = 0;

     while(seconds >= SECS_IN_MINUTE) {
         seconds -= SECS_IN_MINUTE;
         date_time->min++;
     }

     date_time->sec = seconds;
}