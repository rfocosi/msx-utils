#include "msx_fusion.h"

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

unsigned long seconds_in_month(int month, int is_leap_year) {
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

extern unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second) {
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

extern void now(DATE *date, TIME *time){
  GetDate(date);
  GetTime(time);
}

extern unsigned long long now_ts() {
  DATE date;
  TIME time;
  now(&date, &time);
  return to_seconds(date.year, date.month, date.day, time.hour, time.min, time.sec);
}