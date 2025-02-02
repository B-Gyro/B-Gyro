# include "klibc/print.h"
# include "klibc/memory.h"
# include "terminal/tty.h"
# include "sshell/sshell.h"
# include "klibc/strings.h"
# include "time/rtc.h"

void	changeTimeMode(bool b);

void	time(char *args) {
	char *arg;
	_time	time;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Displays the current time.\n");
		return;
	}	

	// getTime(&time);
	// FILL_BUFFER("%2d:%2d:%2d\n", time.hours, time.minutes, time.hours);
	// changeTimeMode(0);
	getTime(&time);
	FILL_BUFFER("%2d:%2d:%2d\n", time.hours, time.minutes, time.hours);
	// changeTimeMode(1);
	// getTime(&time);
	// FILL_BUFFER("%2d:%2d:%2d\n", time.hours, time.minutes, time.hours);
	// changeTimeMode(0);
	// getTime(&time);
	// FILL_BUFFER("%2d:%2d:%2d\n", time.hours, time.minutes, time.hours);

}

void	date(char *args) {
	char *arg;
	_date	date;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Displays the current date.\n");
		return;
	}

	getDate(&date);
	FILL_BUFFER("%s %2d %s %d\n", g_days[date.weekDay - 1], date.day, g_months[date.month - 1], date.year);
}

void	datetime(char *args) {
	char *arg;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Displays the current date and time.\n");
		return;
	}
	
	FILL_BUFFER("%s   Date: %s", COLOR_CYAN, COLOR_RESET);
	date("");
	FILL_BUFFER("%s   Time: %s", COLOR_CYAN, COLOR_RESET);
	time("");
}

void	timer(char *args) {
	char *arg;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Display the time you spent on the screen.\nYou can reset it using '-r' or '--restart'.\n");
		return;
	}

	g_shellMode = 0;
	drawTimer();
	g_shellMode = 1;
	putTtyBuffer();
}

void	screentime(char *args) {
	char *arg;

	arg = strtok(args, " ");
	if (!strncmp(arg, "-h", 2) || !strncmp(arg, "--help", 6)) {
		FILL_BUFFER("Display the time you spent on the screen.\nYou can reset it using '-r' or '--restart'.\n");
		return;
	}

	FILL_BUFFER("%2d:%2d:%2d\n", g_screenTime.hours, g_screenTime.minutes, g_screenTime.seconds);
}