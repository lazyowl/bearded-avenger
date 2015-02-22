#include "tmatrix.h"

#define MAX_LINES	100

typedef struct {
	int screen_cursor_row;
	int screen_cursor_col;
	int y_offset;

	TMatrix *tmtx;
} SMatrix;


void render(SMatrix *);
int render_line(int, char *);

void move_cursor_up(SMatrix *);
void move_cursor_down(SMatrix *);
void move_cursor_left(SMatrix *);
void move_cursor_right(SMatrix *);
void move_cursor_to(SMatrix *, int, int);

void insert_at_cursor(SMatrix *, int ch);
void delete_before_cursor(SMatrix *);
void insert_newline_at_cursor(SMatrix *);

void init_smatrix(SMatrix *);
