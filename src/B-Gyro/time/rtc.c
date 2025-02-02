#include "time/rtc.h"

char g_days[7][9] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char g_months[12][9] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

static uint8_t	convertBCDtoInt(uint8_t n) {
	return ((n & 0x0F) + ((n / 16) * 10));
}

static uint8_t	rtcRead(uint8_t register_) {
    portByteOut(RTC_INDEX_PORT, register_);	// Select register
    return (portByteIn(RTC_DATA_PORT));		// Read value from the data port
}

// send macro _12_HOURS_MODE / _24_HOURS_MODE as arg
void	changeTimeMode(bool b){
	uint8_t	r;

	r = rtcRead(STATUS_REGISTER_B);
	if (b)
		r &= ~(1 << 1);	// 24 hours
	else
		r |= (1 << 1); // 12 hours

	portByteOut(RTC_INDEX_PORT, STATUS_REGISTER_B);
	portByteOut(RTC_DATA_PORT, r);

}

void	getTime(_time *time) {
	while (rtcRead(STATUS_REGISTER_A) & 0x80);
	time->seconds = convertBCDtoInt(rtcRead(SECONDS_REGISTER));
	time->minutes = convertBCDtoInt(rtcRead(MINUTES_REGISTER));
	time->hours = convertBCDtoInt(rtcRead(HOURS_REGISTER)); // to do: | 0x80
}

void	getDate(_date *date) {
	while (rtcRead(STATUS_REGISTER_A) & 0x80);
	date->day = convertBCDtoInt(rtcRead(DAY_REGISTER));
	date->weekDay = convertBCDtoInt(rtcRead(WEEK_DAY_REGISTER));
	date->month = convertBCDtoInt(rtcRead(MONTH_REGISTER));
	date->year = convertBCDtoInt(rtcRead(CENTURY_REGISTER)) * 100 + convertBCDtoInt(rtcRead(YEAR_REGISTER));
}

void	getTimestamp(_timestamp *timestamp) {
	getTime(&timestamp->time);
	getDate(&timestamp->date);
}
