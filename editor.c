#include "editor.h"

// main function which updates screen
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

// inserts character at cursor
void insert_at_cursor(CMatrix *cmtx, int ch) {
	int i;
	int len = strlen(cmtx->cursor_line->arr);

	// shift all characters one ahead and add \0
	for(i = len - 1; i >= cmtx->cursor_col; i--)
		cmtx->cursor_line->arr[i + 1] = cmtx->cursor_line->arr[i];
	cmtx->cursor_line->arr[len + 1] = '\0';

	// add in character
	cmtx->cursor_line->arr[cmtx->cursor_col] = ch;
	cmtx->cursor_col++;
}

// deletes character before cursor
void delete_before_cursor(CMatrix *cmtx) {
	int i;
	// shift all characters one back (loop includes the \0)
	for(i = cmtx->cursor_col - 1; cmtx->cursor_line->arr[i]; i++)
		cmtx->cursor_line->arr[i] = cmtx->cursor_line->arr[i + 1];
	cmtx->cursor_col--;
}

// insert newline
void insert_newline_at_cursor(CMatrix *cmtx) {
	// point where current line is to be broken
	char *breakpoint = &(cmtx->cursor_line->arr[cmtx->cursor_col]);

	// create a new line and insert into linked list
	Line *extra_line = new_line();
	extra_line->next = cmtx->cursor_line->next;
	extra_line->prev = cmtx->cursor_line;
	cmtx->cursor_line->next = extra_line;

	// copy over content and set \0 to override the content of the previous line
	strcpy(extra_line->arr, breakpoint);
	extra_line->arr[strlen(breakpoint)] = '\0';
	cmtx->cursor_line->arr[cmtx->cursor_col] = '\0';

	// update tail if we need to
	if(extra_line->next == NULL) {
		cmtx->tail = extra_line;
	}

	// update cursor
	cmtx->cursor_line = cmtx->cursor_line->next;
	cmtx->cursor_line_int++;
	cmtx->cursor_col = 0;
}

// delete newline
void delete_newline_at_cursor(CMatrix *cmtx) {
	// store this for moving cursor at the end
	int prev_line_length = strlen(cmtx->cursor_line->prev->arr);

	Line *temp = cmtx->cursor_line->prev;
	// concatenate this line to previous line
	strcat(cmtx->cursor_line->prev->arr, cmtx->cursor_line->arr);

	// update previous line to point next to next line
	cmtx->cursor_line->prev->next = cmtx->cursor_line->next;

	free(cmtx->cursor_line);

	// update cursor
	cmtx->cursor_line = temp;
	cmtx->cursor_line_int--;
	cmtx->cursor_col = prev_line_length;
}

// clean up
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

// initialize cmtx using contents of file
// TODO I assume that the line length < BUF_SIZE (i.e it will always read the entire line into buf) <= NEED TO CHANGE THIS
int init_from_file(CMatrix *cmtx, char *filename) {
	Line *current;
	char buf[BUF_SIZE];
	FILE *f = fopen(filename, "r");
	if(!f) {
		perror("open");
		return EXIT_FAILURE;
	}
	cmtx->cursor_line_int = 0;
	cmtx->cursor_col = 0;
	cmtx->head = cmtx->cursor_line = current = cmtx->tail = new_line();

	while(fgets(buf, BUF_SIZE, f)) {
		strcpy(current->arr, buf);
		current->next = new_line();
		current->next->prev = current;
		current = current->next;
	}
	// over-allocates one line (pointed to by current)
	cmtx->tail = current->prev;
	free(current);
	cmtx->tail->next = NULL;
	fclose(f);
	return EXIT_SUCCESS;
}

int init_blank(CMatrix *cmtx) {
	cmtx->head = cmtx->cursor_line = cmtx->tail = new_line();
	cmtx->cursor_line_int = 0;
	cmtx->cursor_col = 0;
	return EXIT_SUCCESS;
}

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
				if(matrix.cursor_line->arr[matrix.cursor_col] && matrix.cursor_col + 1 < MAX)
					matrix.cursor_col++;
				break;
			case KEY_UP:
				// move up unless already on first line
				if(matrix.cursor_line != matrix.head) {
					if(matrix.cursor_col > strlen(matrix.cursor_line->prev->arr))
						matrix.cursor_col = strlen(matrix.cursor_line->prev->arr);
					matrix.cursor_line = matrix.cursor_line->prev;
					matrix.cursor_line_int--;
				}
				break;
			case KEY_DOWN:
				// move down unless nothing below
				if(matrix.cursor_line->next) {
					if(matrix.cursor_col > strlen(matrix.cursor_line->next->arr))
						matrix.cursor_col = strlen(matrix.cursor_line->next->arr);
					matrix.cursor_line = matrix.cursor_line->next;
					matrix.cursor_line_int++;
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
