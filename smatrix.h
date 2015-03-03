#include "tmatrix.h"

#define MAX_LINES	100

typedef struct {
	int screen_cursor_row;
	int screen_cursor_col;
	int y_offset;
} SMatrix;


void render(SMatrix *, TMatrix *);
int render_line(int, char *);
void init_smatrix(SMatrix *);
void destroy_smatrix(SMatrix *);
