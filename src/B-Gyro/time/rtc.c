#include "time/rtc.h"

bool timeMode = _24_HOURS_MODE;
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
	if (b == _12_HOURS_MODE)
		r &= ~(1 << 1); // change HOURS_FORMAT_BIT to 0
	else
		r |= (1 << 1);  // change HOURS_FORMAT_BIT to 1

	portByteOut(RTC_INDEX_PORT, STATUS_REGISTER_B);
	portByteOut(RTC_DATA_PORT, r);
	timeMode = b;
}

void	getTime(_time *time) {
	uint8_t	registerB;
	
	while (rtcRead(STATUS_REGISTER_A) & UPDATE_PROGRESS_BIT);

	registerB = rtcRead(STATUS_REGISTER_B);
	time->seconds = rtcRead(SECONDS_REGISTER);
	time->minutes = rtcRead(MINUTES_REGISTER);
	time->hours	= rtcRead(HOURS_REGISTER);

	if (!(registerB & BINARY_MODE_BIT)) {
		time->seconds = convertBCDtoInt(time->seconds);
		time->minutes = convertBCDtoInt(time->minutes);
		if ((registerB & HOURS_FORMAT_BIT) == _12_HOURS_MODE)
			time->hours = ((time->hours & 0x0F) + (((time->hours & 0x70) / 16) * 10)) | (time->hours & (1 << 7));
		else
			time->hours = convertBCDtoInt(time->hours);
	}

	if ((registerB & HOURS_FORMAT_BIT) == _12_HOURS_MODE) {
		time->meridiem = (!!(time->hours & (1 << 7)) * 'P') + (!(time->hours & (1 << 7)) * 'A');
		time->hours &= ~(1 << 7);
	}
}

void	getDate(_date *date) {
	while (rtcRead(STATUS_REGISTER_A) & UPDATE_PROGRESS_BIT);
	date->day = convertBCDtoInt(rtcRead(DAY_REGISTER));
	date->weekDay = convertBCDtoInt(rtcRead(WEEK_DAY_REGISTER));
	date->month = convertBCDtoInt(rtcRead(MONTH_REGISTER));
	date->year = convertBCDtoInt(rtcRead(CENTURY_REGISTER)) * 100 + convertBCDtoInt(rtcRead(YEAR_REGISTER));
}

void	getTimestamp(_timestamp *timestamp) {
	getTime(&timestamp->time);
	getDate(&timestamp->date);
}
