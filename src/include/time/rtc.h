#pragma once

# include "time/pit.h"

# define RTC_DATA_PORT	0x71
# define RTC_INDEX_PORT	0x70

# define SECONDS_REGISTER	0x00
# define MINUTES_REGISTER	0x02
# define HOURS_REGISTER		0x04

# define WEEK_DAY_REGISTER	0x06
# define DAY_REGISTER		0x07
# define MONTH_REGISTER		0x08
# define YEAR_REGISTER		0x09
# define CENTURY_REGISTER	0x32

# define STATUS_REGISTER_A	0x0A
# define STATUS_REGISTER_B	0x0B

// REGISTER A
# define UPDATE_PROGRESS_BIT 0x80

// REGISTER B
# define HOURS_FORMAT_BIT	0x02
# define BINARY_MODE_BIT	0x04


# define _12_HOURS_MODE	0
# define _24_HOURS_MODE	1


typedef struct time {
    char    meridiem;
	uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} _time;

typedef struct date {
    uint8_t day;
	uint8_t weekDay;
    uint8_t month;
    uint16_t year;
} _date;

typedef struct timestamp {
	_time	time;
	_date	date;
} _timestamp;

extern bool	timeMode;
extern _time g_screenTime;
extern char g_days[7][9];
extern char g_months[12][9];

void	getTime(_time *time);
void	getDate(_date *date);
void	getTimestamp(_timestamp *timestamp);
void	changeTimeMode(bool b);
