#include "date_time.h"

extern char* tstoa(unsigned long ts);

extern unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second);

extern void now(DATE_TIME *date_time);

extern unsigned long long now_ts();

extern void ts_to_date(unsigned long long seconds, DATE_TIME* date_time);
