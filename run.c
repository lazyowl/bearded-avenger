#include <stdio.h>
#include "tmatrix.h"
#include "smatrix.h"

#define KEY_NEWLINE	13

int main() {
	int ch;
	SMatrix matrix;
	init_smatrix(&matrix);

	TMatrix tmtx;
	init_tmatrix(&tmtx);

	matrix.tmtx = &tmtx;

	initscr();	// initialize
	keypad(stdscr, TRUE);	// extra keys
	cbreak();
	nonl();
	noecho();

	// create matrix and first line
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
