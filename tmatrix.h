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

typedef struct Line {
	char *arr;
	int exp;	// stores the power of 2 currently reached for realloc
	struct Line *next;
	struct Line *prev;
} Line;

typedef struct {
	Line *head;
	Line *tail;
} TMatrix;


void insert_at(TMatrix *, int, int);
void delete_before(TMatrix *, int, int);
void insert_newline_at(TMatrix *, int, int);
void delete_newline_at(TMatrix *, int, int);
Line *new_line();
