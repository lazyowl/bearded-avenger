#include <stdio.h>
#include "editor.h"

int main(int argc, char **argv) {
	int ch;
	CMatrix matrix;

	initscr();	// initialize
	keypad(stdscr, TRUE);	// extra keys
	cbreak();
	nonl();
	noecho();

	// create matrix and first line
	if(argc == 1)
		init_blank(&matrix);
	else
		init_from_file(&matrix, argv[1]);
	render(&matrix);

	while(1) {
		ch = wgetch(stdscr);
		switch(ch) {
			case KEY_LEFT:
				move_cursor_left(&matrix);
				break;
			case KEY_RIGHT:
				move_cursor_right(&matrix);
				break;
			case KEY_UP:
				move_cursor_up(&matrix);
				break;
			case KEY_DOWN:
				move_cursor_down(&matrix);
				break;
			case KEY_BACKSPACE:
				delete_before_cursor(&matrix);
				break;
			case KEY_ESC:
				finish(&matrix);
				break;
			case KEY_NEWLINE:
				insert_newline_at_cursor(&matrix);
				break;
			default:
				insert_at_cursor(&matrix, ch);
				break;
		}
		render(&matrix);
	}
	endwin();
	return 0;
}
