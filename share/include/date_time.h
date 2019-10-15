typedef struct {
	int hour;	/* Hours 0..23 */
	int min;	/* Minutes 0..59 */
	int sec;	/* Seconds 0..59 */
} TIME;

typedef struct {
	int year;	/* Year 1980...2079 */
	int month;	/* Month 1=Jan..12=Dec */
	int day;	/* Day of the month 1...31 */
	int dow;	/* On getdate() gets Day of week 0=Sun...6=Sat */
} DATE;

typedef struct {
	int year;	/* Year 1980...2079 */
	int month;	/* Month 1=Jan..12=Dec */
	int day;	/* Day of the month 1...31 */
	int dow;	/* On getdate() gets Day of week 0=Sun...6=Sat */
	int hour;	/* Hours 0..23 */
	int min;	/* Minutes 0..59 */
	int sec;	/* Seconds 0..59 */
} DATE_TIME;

void				GetDate (DATE *date);  										// get date
void				GetTime (TIME *time);										// get time
int					SetDate (int Year, char month, char day);  										// set date, returns 0 if valid
int					SetTime (TIME *time);										// set time, returns 0 if valid
