#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUF_SIZE	20	// read buffer size

// key mappings
#define KEY_NEWLINE	13
#define KEY_ESC	27

typedef struct Line {
	char *arr;
	int exp;	// stores the power of 2 currently reached for realloc
	struct Line *next;
	struct Line *prev;
} Line;

typedef struct {
	Line *cursor_line;
	int cursor_line_int;
	int cursor_col;
	int y_offset;
	Line *head;
	Line *tail;
} CMatrix;

void render(CMatrix *);
void insert_at_cursor(CMatrix *, int);
void delete_before_cursor(CMatrix *);
void insert_newline_at_cursor(CMatrix *);
void delete_newline_at_cursor(CMatrix *);
void move_cursor_up(CMatrix *);
void move_cursor_down(CMatrix *);
void move_cursor_to(CMatrix *, int, int);
void finish(CMatrix *);
Line *new_line();
