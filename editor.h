#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tmatrix.h"

// key mappings
#define KEY_NEWLINE	13
#define KEY_ESC	27


typedef struct {
	int row;
	int col;
} Cursor;

void render(CMatrix *);
int render_line(int pos, char *);



void finish(CMatrix *);

int init_blank(CMatrix *);
int init_from_file(CMatrix *, char *);
