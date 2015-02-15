#include "editor.h"

void render(CMatrix *cmtx) {
	int i = 0;
	clear();
	Line *iter = cmtx->head;
	while(iter) {
		mvaddstr(i++, 0, iter->arr);
		iter = iter->next;
	}
	move(cmtx->cursor_line_int, cmtx->cursor_col);
}

void insert_at_cursor(CMatrix *cmtx, int ch) {
	int i;
	int len = strlen(cmtx->cursor_line->arr);
	for(i = len - 1; i >= cmtx->cursor_col; i--)
		cmtx->cursor_line->arr[i + 1] = cmtx->cursor_line->arr[i];
	cmtx->cursor_line->arr[len + 1] = '\0';
	cmtx->cursor_line->arr[cmtx->cursor_col] = ch;
}

void delete_before_cursor(CMatrix *cmtx) {
	int i;
	for(i = cmtx->cursor_col - 1; cmtx->cursor_line->arr[i]; i++)
		cmtx->cursor_line->arr[i] = cmtx->cursor_line->arr[i + 1];
}


void insert_newline_at_cursor(CMatrix *cmtx) {
	char *breakpoint = &(cmtx->cursor_line->arr[cmtx->cursor_col]);
	Line *extra_line = new_line();
	extra_line->next = cmtx->cursor_line->next;
	extra_line->prev = cmtx->cursor_line;
	cmtx->cursor_line->next = extra_line;

	strcpy(extra_line->arr, breakpoint);
	extra_line->arr[strlen(breakpoint)] = '\0';
	cmtx->cursor_line->arr[cmtx->cursor_col] = '\0';

	if(extra_line->next == NULL) {
		// we appended a line
		cmtx->tail = extra_line;
	}
}


void delete_newline_at_cursor(CMatrix *cmtx) {
	int prev_line_length = strlen(cmtx->cursor_line->prev->arr);
	Line *temp = cmtx->cursor_line->prev;
	strcat(cmtx->cursor_line->prev->arr, cmtx->cursor_line->arr);
	cmtx->cursor_line->prev->next = cmtx->cursor_line->next;
	free(cmtx->cursor_line);
	cmtx->cursor_line = temp;
	cmtx->cursor_line_int--;

	cmtx->cursor_col = prev_line_length;
}


void finish(CMatrix *cmtx) {
	// free data structure
	Line *current = cmtx->head, *temp;
	while(current) {
		temp = current->next;
		free(current);
		current = temp;
	}
	endwin();
	exit(0);
}

// this method allocates memory!
Line *new_line() {
	Line *l = (Line *)malloc(sizeof(Line));
	l->next = l->prev = NULL;
	l->arr[0] = '\0';
	return l;
}

int main() {
	int ch;

	initscr();	// initialize
	keypad(stdscr, TRUE);	// extra keys
	cbreak();
	nonl();
	noecho();

	// create matrix and first line
	CMatrix matrix;
	matrix.head = new_line();
	matrix.tail = matrix.head;
	matrix.cursor_line = matrix.head;
	matrix.cursor_line_int = 0;
	matrix.cursor_col = 0;

	while(1) {
		ch = wgetch(stdscr);
		switch(ch) {
			case ESC_KEY:
				finish(&matrix);
			case KEY_LEFT:
				if(matrix.cursor_col > 0)
					matrix.cursor_col--;
				break;
			case KEY_RIGHT:
				if(matrix.cursor_line->arr[matrix.cursor_col] && matrix.cursor_col + 1 < MAX)
					matrix.cursor_col++;
				break;
			case KEY_UP:
				if(matrix.cursor_line != matrix.head) {
					if(matrix.cursor_col > strlen(matrix.cursor_line->prev->arr));
						matrix.cursor_col = strlen(matrix.cursor_line->prev->arr);
					matrix.cursor_line = matrix.cursor_line->prev;
					matrix.cursor_line_int--;
				}
				break;
			case KEY_DOWN:
				if(matrix.cursor_line->next) {
					if(matrix.cursor_col > strlen(matrix.cursor_line->next->arr));
						matrix.cursor_col = strlen(matrix.cursor_line->next->arr);
					matrix.cursor_line = matrix.cursor_line->next;
					matrix.cursor_line_int++;
				}
				break;
			case KEY_BACKSPACE:
				if(matrix.cursor_col == 0) {
					// handle moving all lines up (unless cursor_row == 0)
					if(matrix.cursor_line != matrix.head) {
						delete_newline_at_cursor(&matrix);
					}
				} else {
					delete_before_cursor(&matrix);
					matrix.cursor_col--;
				}
				break;
			default:
				if(ch == NEWLINE_KEY) {
					insert_newline_at_cursor(&matrix);
					matrix.cursor_line = matrix.cursor_line->next;
					matrix.cursor_line_int++;
					matrix.cursor_col = 0;
				} else {
					insert_at_cursor(&matrix, ch);
					matrix.cursor_col++;
				}
				break;
		}
		render(&matrix);
	}
	endwin();
	return 0;
}
