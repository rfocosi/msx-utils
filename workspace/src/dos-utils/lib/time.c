#include "time.h"

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

unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second) {
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
