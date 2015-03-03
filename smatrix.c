#include "smatrix.h"
#define MAX	20

void render(SMatrix *smtx) {
	int i = 0, len;
	int text_line_number = 0, num_screen_lines;
	int starting_screen_line[MAX_LINES];
	clear();

	Line *iter = smtx->tmtx->head;
	while(iter) {
		starting_screen_line[text_line_number] = i;
		len = strlen(iter->arr);
		if(len == 0) {
			i++;
		} else {
			i += len / COLS + ((len % COLS == 0) ? 0 : 1);
		}
		text_line_number++;
		iter = iter->next;
	}

	smtx->screen_cursor_row = starting_screen_line[smtx->tmtx->logical_cursor_row];
	smtx->screen_cursor_row += smtx->tmtx->logical_cursor_col / COLS;
	smtx->screen_cursor_col = smtx->tmtx->logical_cursor_col % COLS;

	num_screen_lines = starting_screen_line[smtx->tmtx->logical_cursor_row] / COLS + (starting_screen_line[smtx->tmtx->logical_cursor_row] % COLS > 0);

	if(smtx->screen_cursor_row - smtx->y_offset >= LINES) {
		smtx->y_offset += num_screen_lines;
	}
	if(smtx->screen_cursor_row < smtx->y_offset) {
		smtx->y_offset--;
	}

	iter = smtx->tmtx->head;
	for(i = 0; i < smtx->y_offset; i++) {
		iter = iter->next;
	}
	i = 0;
	while(iter) {
		i += render_line(i, iter->arr);
		iter = iter->next;
	}
	// the cursor needs to be positioned relative to the screen, not the actual text
	// in other words, we need to map the line number in the text (cursor_line_int) to a screen line number
	move(smtx->screen_cursor_row - smtx->y_offset, smtx->screen_cursor_col);
	refresh();
}

int render_line(int pos, char *arr) {
	int i = 0;
	char *start = arr;
	while(strlen(start) > COLS) {
		mvaddnstr(pos + i, 0, start, COLS);
		i++;
		start += COLS;
	}
	if(strlen(start) > 0) {
		mvaddstr(pos + i, 0, start);
		i++;
	}
	return i > 0 ? i : 1;
}

void insert_at_cursor(SMatrix *smtx, int ch) {
	insert(smtx->tmtx, ch);
}

void delete_before_cursor(SMatrix *smtx) {
	if(smtx->tmtx->logical_cursor_col == 0)
		delete_newline(smtx->tmtx);
	else
		delete(smtx->tmtx);
}

void insert_newline_at_cursor(SMatrix *smtx) {
	insert_newline(smtx->tmtx);
}

void move_cursor_up(SMatrix *smtx) {
	move_logical_cursor_up(smtx->tmtx);
}

void move_cursor_down(SMatrix *smtx) {
	move_logical_cursor_down(smtx->tmtx);
}

void move_cursor_left(SMatrix *smtx) {
	move_logical_cursor_left(smtx->tmtx);
}

void move_cursor_right(SMatrix *smtx) {
	move_logical_cursor_right(smtx->tmtx);
}

void init_smatrix(SMatrix *smtx) {
	smtx->screen_cursor_row = smtx->screen_cursor_col = smtx->y_offset = 0;
}

void destroy_smatrix(SMatrix *smtx) {
	destroy_tmatrix(smtx->tmtx);
	endwin();
}
