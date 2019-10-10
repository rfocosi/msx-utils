#include "date_time.h"

extern unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second);

extern void now(DATE *date, TIME *time);

extern unsigned long long now_ts();
