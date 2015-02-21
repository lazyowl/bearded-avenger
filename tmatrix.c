#include "tmatrix.h"

void insert_at(TMatrix *tmtx, int row, int col, int ch) {
	int i = 0;
	int len;
	Line *current = tmtx->head;

	while(i++ < row)
		current = current->next;
	len = strlen(current->arr);

	// resize arr if necessary
	if(len >= current->exp * BUF_SIZE - 1) {
		current->exp *= 2;
		current->arr = (char *)realloc(current->arr, current->exp * BUF_SIZE);
	}

	// shift all characters one ahead along with \0
	for(i = len; i >= col; i--)
		current->arr[i + 1] = current->arr[i];

	// add in character
	current->arr[col] = ch;
}

void delete_before(TMatrix *tmtx, int row, int col) {
	int i;
	Line *current = tmtx->head;

	while(i++ < row)
		current = current->next;

	// shift all characters one back (loop includes the \0)
	for(i = col - 1; current->arr[i]; i++)
		current->arr[i] = current->arr[i + 1];
}

void insert_newline_at(TMatrix *tmtx, int row, int col) {
	int i;
	Line *current = tmtx->head;

	while(i++ < row)
		current = current->next;

	// point where current line is to be broken
	char *breakpoint = &(current->arr[col]);

	// create a new line and insert into linked list
	Line *extra_line = new_line();
	extra_line->next = current->next;
	if(current->next)
		current->next->prev = extra_line;
	extra_line->prev = current;
	current->next = extra_line;

	// make sure that the new line has enough space (account for the \0, hence using >= and not >) and then:
	// copy over content and set \0 to override the content of the previous line
	while(strlen(breakpoint) >= extra_line->exp * BUF_SIZE)
		extra_line->exp *= 2;
	extra_line->arr = (char *)realloc(extra_line->arr, extra_line->exp * BUF_SIZE);
	strcpy(extra_line->arr, breakpoint);
	extra_line->arr[strlen(breakpoint)] = '\0';
	current->arr[col] = '\0';

	// update tail if we need to
	if(extra_line->next == NULL) {
		tmtx->tail = extra_line;
	}
}

void delete_newline_at(TMatrix *tmtx, int row, int col) {
	int i;
	Line *current = tmtx->head;

	while(i++ < row)
		current = current->next;

	// store this for moving cursor at the end
	int prev_line_length = strlen(current->prev->arr);

	Line *temp = current->prev;
	// concatenate this line to previous line
	strcat(current->prev->arr, current->arr);

	// update previous line to point next to next line
	current->prev->next = current->next;

	free(current->arr);
	free(current);
}

// this method allocates memory!
Line *new_line() {
	Line *l = (Line *)malloc(sizeof(Line));
	l->next = l->prev = NULL;
	l->exp = 1;
	l->arr = (char *)malloc(sizeof(char) * BUF_SIZE);
	l->arr[0] = '\0';
	return l;
}
