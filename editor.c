#include "editor.h"

// main function which updates screen
void render(TMatrix *tmtx) {
	int i = 0;
	clear();
	Line *iter = tmtx->head;
	for(; i < tmtx->y_offset; i++)
		iter = iter->next;
	i = 0;
	while(iter) {
		mvaddstr(i++, 0, iter->arr);
		iter = iter->next;
	}
	// the cursor needs to be positioned relative to the screen, not the actual text
	// in other words, we need to map the line number in the text (cursor_row) to a screen line number
	move(tmtx->cursor_row - tmtx->y_offset, tmtx->cursor_col);
	refresh();
}

// inserts character at cursor
void insert_at_cursor(TMatrix *tmtx, int ch) {
	int i;
	int len = strlen(tmtx->text_line->arr);

	// resize arr if necessary
	if(len >= tmtx->text_line->exp * BUF_SIZE - 1) {
		tmtx->text_line->exp *= 2;
		tmtx->text_line->arr = (char *)realloc(tmtx->text_line->arr, tmtx->text_line->exp * BUF_SIZE);
	}

	// shift all characters one ahead along with \0
	for(i = len; i >= tmtx->cursor_col; i--)
		tmtx->text_line->arr[i + 1] = tmtx->text_line->arr[i];

	// add in character
	tmtx->text_line->arr[tmtx->cursor_col] = ch;
	tmtx->cursor_col++;
}

// deletes character before cursor
void delete_before_cursor(TMatrix *tmtx) {
	int i;
	// shift all characters one back (loop includes the \0)
	for(i = tmtx->cursor_col - 1; tmtx->text_line->arr[i]; i++)
		tmtx->text_line->arr[i] = tmtx->text_line->arr[i + 1];
	tmtx->cursor_col--;
}

// insert newline
void insert_newline_at_cursor(TMatrix *tmtx) {
	// point where current line is to be broken
	char *breakpoint = &(tmtx->text_line->arr[tmtx->cursor_col]);

	// create a new line and insert into linked list
	Line *extra_line = new_line();
	extra_line->next = tmtx->text_line->next;
	if(tmtx->text_line->next)
		tmtx->text_line->next->prev = extra_line;
	extra_line->prev = tmtx->text_line;
	tmtx->text_line->next = extra_line;

	// make sure that the new line has enough space (account for the \0, hence using >= and not >) and then:
	// copy over content and set \0 to override the content of the previous line
	while(strlen(breakpoint) >= extra_line->exp * BUF_SIZE)
		extra_line->exp *= 2;
	extra_line->arr = (char *)realloc(extra_line->arr, extra_line->exp * BUF_SIZE);
	strcpy(extra_line->arr, breakpoint);
	extra_line->arr[strlen(breakpoint)] = '\0';
	tmtx->text_line->arr[tmtx->cursor_col] = '\0';

	// update tail if we need to
	if(extra_line->next == NULL) {
		tmtx->tail = extra_line;
	}

	// update cursor
	tmtx->text_line = tmtx->text_line->next;
	tmtx->cursor_row++;
	tmtx->cursor_col = 0;

	if(tmtx->cursor_row - tmtx->y_offset > LINES - 1)
		tmtx->y_offset++;
}

// delete newline
void delete_newline_at_cursor(TMatrix *tmtx) {
	// store this for moving cursor at the end
	int prev_line_length = strlen(tmtx->text_line->prev->arr);

	Line *temp = tmtx->text_line->prev;
	// concatenate this line to previous line
	strcat(tmtx->text_line->prev->arr, tmtx->text_line->arr);

	// update previous line to point next to next line
	tmtx->text_line->prev->next = tmtx->text_line->next;

	free(tmtx->text_line->arr);
	free(tmtx->text_line);

	// update cursor
	tmtx->text_line = temp;
	tmtx->cursor_row--;
	tmtx->cursor_col = prev_line_length;

	if(tmtx->cursor_row < tmtx->y_offset && tmtx->y_offset > 0)
		tmtx->y_offset--;
}

// move cursor up
void move_cursor_up(TMatrix *tmtx) {
	if(tmtx->text_line->next == tmtx->head) return;
	if(tmtx->cursor_col > strlen(tmtx->text_line->prev->arr))
		tmtx->cursor_col = strlen(tmtx->text_line->prev->arr);
	tmtx->text_line = tmtx->text_line->prev;
	tmtx->cursor_row--;
	if(tmtx->cursor_row < tmtx->y_offset) tmtx->y_offset--;
}


// move cursor down
void move_cursor_down(TMatrix *tmtx) {
	if(!tmtx->text_line->next) return;
	if(tmtx->cursor_col > strlen(tmtx->text_line->next->arr))
		tmtx->cursor_col = strlen(tmtx->text_line->next->arr);
	tmtx->text_line = tmtx->text_line->next;
	tmtx->cursor_row++;
	if(tmtx->cursor_row - tmtx->y_offset > LINES - 1)
		tmtx->y_offset++;
}


void move_cursor_left(TMatrix *tmtx) {
	if(tmtx->cursor_col > 0) {
		tmtx->cursor_col--;
	} else {
	}
}

void move_cursor_right(TMatrix *tmtx) {
	if(tmtx->cursor_col < strlen(tmtx->text_line->arr)) tmtx->cursor_col++;
	if(tmtx->cursor_col > COLS) {
		tmtx->cursor_col = 0;
		tmtx->cursor_row++;
	}
}


void move_cursor_to(TMatrix *tmtx, int row, int col) {
	if(row < 0 || col < 0) return;
	while(tmtx->cursor_row > row) {
		tmtx->cursor_row--;
		tmtx->text_line = tmtx->text_line->prev;
	}
	while(tmtx->cursor_row < row) {
		tmtx->cursor_row++;
		tmtx->text_line = tmtx->text_line->next;
	}
	tmtx->cursor_col = col;
}

// clean up
void finish(TMatrix *tmtx) {
	// free data structure
	Line *current = tmtx->head, *temp;
	while(current) {
		temp = current->next;
		free(current->arr);
		free(current);
		current = temp;
	}
	endwin();
	exit(0);
}


// initialize tmtx using contents of file
int init_from_file(TMatrix *tmtx, char *filename) {
	Line *current;
	int len;
	char buf[BUF_SIZE];
	FILE *f = fopen(filename, "r");
	if(!f) {
		perror("open");
		return EXIT_FAILURE;
	}
	tmtx->cursor_row = 0;
	tmtx->cursor_col = 0;
	tmtx->y_offset = 0;
	tmtx->head = tmtx->text_line = current = tmtx->tail = new_line();

	while(fgets(buf, BUF_SIZE / 2, f)) {
		len = strlen(buf);
		if(strlen(current->arr) + len >= current->exp * BUF_SIZE) {
			current->exp *= 2;
			current->arr = (char *)realloc(current->arr, current->exp * BUF_SIZE);
		}
		strcat(current->arr, buf);
		if(buf[len - 1] == '\n') {
			current->arr[strlen(current->arr) - 1] = '\0';	// remove newline from matrix representation
			current->next = new_line();
			current->next->prev = current;
			current = current->next;
			break;
		}
		// otherwise continue
	}

	tmtx->tail = current->prev;
	free(current->arr);
	free(current);
	tmtx->tail->next = NULL;
	fclose(f);
	return EXIT_SUCCESS;
}

int init_blank(TMatrix *tmtx) {
	tmtx->head = tmtx->text_line = tmtx->tail = new_line();
	tmtx->cursor_row = 0;
	tmtx->cursor_col = 0;
	tmtx->y_offset = 0;
	return EXIT_SUCCESS;
}

void delete_behind_cursor(TMatrix *tmtx) {
	if(tmtx->cursor_col == 0) {
		delete_newline_at_cursor(tmtx);
	} else {
		delete_before_cursor(tmtx);
	}
}

