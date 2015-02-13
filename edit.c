#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>

// [signal handler] clean up
static void finish(int sig) {
	endwin();
	exit(0);
}

int main() {
	signal(SIGINT, finish);	// attach signal handler

	initscr();	// initialize
	keypad(stdscr, TRUE);
	cbreak();
	echo();	// echo getch to stdout

	while(1) {
		getch();
	}
	finish(0);

	return 0;
}

