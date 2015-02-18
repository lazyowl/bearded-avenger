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
				// move left if possible
				if(matrix.cursor_col > 0)
					matrix.cursor_col--;
				break;
			case KEY_RIGHT:
				// move right if we haven't reached the end of the text or MAX
				if(matrix.cursor_line->arr[matrix.cursor_col] && matrix.cursor_col < strlen(matrix.cursor_line->arr))
					matrix.cursor_col++;
				break;
			case KEY_UP:
				// move up unless already on first line
				if(matrix.cursor_line != matrix.head) {
					move_cursor_up(&matrix);
				}
				break;
			case KEY_DOWN:
				// move down unless nothing below
				if(matrix.cursor_line->next) {
					move_cursor_down(&matrix);
				}
				break;
			case KEY_BACKSPACE:
				// if at beginning of line, handle moving all lines up (unless cursor_row == 0)
				// else just delete
				if(matrix.cursor_col == 0) {
					if(matrix.cursor_line != matrix.head) {
						delete_newline_at_cursor(&matrix);
					}
				} else {
					delete_before_cursor(&matrix);
				}
				break;
			case KEY_ESC:
				finish(&matrix);
				break;
			case KEY_NEWLINE:
				// if char is new line, handle it separately
				insert_newline_at_cursor(&matrix);
				break;
			case KEY_HOME:
				// beginning
				matrix.cursor_col = 0;
				break;
			case KEY_END:
				// end
				matrix.cursor_col = strlen(matrix.cursor_line->arr);
				break;
			case KEY_PPAGE:
			case KEY_NPAGE:
				// ignore for now
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
