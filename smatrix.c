#include "smatrix.h"
#define MAX	20

void render(SMatrix *smtx, TMatrix *tmtx) {
	int i = 0, len;
	int text_line_number = 0, num_screen_lines;
	int starting_screen_line[MAX_LINES];
	clear();

	Line *iter = tmtx->head;
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

	smtx->screen_cursor_row = starting_screen_line[tmtx->logical_cursor_row];
	smtx->screen_cursor_row += tmtx->logical_cursor_col / COLS;
	smtx->screen_cursor_col = tmtx->logical_cursor_col % COLS;

	num_screen_lines = starting_screen_line[tmtx->logical_cursor_row] / COLS + (starting_screen_line[tmtx->logical_cursor_row] % COLS > 0);

	if(smtx->screen_cursor_row - smtx->y_offset >= LINES) {
		smtx->y_offset += num_screen_lines;
	}
	if(smtx->screen_cursor_row < smtx->y_offset) {
		smtx->y_offset--;
	}

	iter = tmtx->head;
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

void init_smatrix(SMatrix *smtx) {
	smtx->screen_cursor_row = smtx->screen_cursor_col = smtx->y_offset = 0;
}

void destroy_smatrix(SMatrix *smtx) {
	endwin();
}
