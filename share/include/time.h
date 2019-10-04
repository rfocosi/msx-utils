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

extern unsigned long long to_seconds(int year, int month, int day, int hour, int minute, int second);

extern void now(DATE *date, TIME *time);

extern unsigned long long now_ts();
