#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>

#define NEWLINE_KEY '\r'
#define MAX	20
#define ESC_KEY	27

typedef struct Line {
	char arr[MAX];
	struct Line *next;
	struct Line *prev;
} Line;

typedef struct {
	Line *cursor_line;
	int cursor_line_int;
	int cursor_col;
	Line *head;
	Line *tail;
} CMatrix;

void render(CMatrix *);
void insert_at_cursor(CMatrix *, int);
void delete_before_cursor(CMatrix *);
void insert_newline_at_cursor(CMatrix *);
void delete_newline_at_cursor(CMatrix *);
void finish(CMatrix *);
Line *new_line();
