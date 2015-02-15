#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>

#define MAX	20
#define NEWLINE '\r'

static void finish(int);
static void insert_pos(int, int, int);
static void delete_pos(int, int);
static void insert_newline();
static void insert_newline_pos(int, int);
static void delete_newline();
static void delete_newline_pos(int);

static char mat[MAX][MAX];	// TODO replace with something more elegant (possibly a linked list)
static int cursor_row, cursor_col, numrows;

void render() {
	int i;
	clear();
	for(i = 0; i < numrows; i++)
		mvaddstr(i, 0, mat[i]);
	move(cursor_row, cursor_col);
	refresh();
}

void insert_pos(int r, int c, int ch) {
	int i;
	int len = strlen(mat[r]);
	for(i = len - 1; i >= c; i--)
		mat[r][i + 1] = mat[r][i];
	mat[r][len + 1] = '\0';
	mat[r][c] = ch;
}


void delete_pos(int r, int c) {
	int i;
	for(i = c; mat[r][i]; i++)
		mat[r][i] = mat[r][i + 1];
}


// inserts newline after row r
void insert_newline_pos(int r, int c) {
	int i, j;
	char *breakpoint;

	for(i = numrows; i > r; i--)
		strcpy(mat[i + 1], mat[i]);
	breakpoint = &mat[r][c];
	strcpy(mat[r + 1], breakpoint);
	mat[r + 1][strlen(breakpoint)] = '\0';
	mat[r][c] = '\0';
	numrows++;
}


// deletes new line at row r (and merges row with r - 1)
void delete_newline_pos(int r) {
	int i;
	strcat(mat[r - 1], mat[r]);
	for(i = r; i < numrows; i++)
		strcpy(mat[r], mat[r + 1]);
	numrows--;
}


void finish(int sig) {
	endwin();
	exit(0);
}

int main() {
	int ch;
	int i, temp;

	numrows = 1;	// the first row

	signal(SIGINT, finish);	// set signal handler

	initscr();	// initialize
	keypad(stdscr, TRUE);	// extra keys
	cbreak();
	nonl();
	noecho();

	while(1) {
		ch = wgetch(stdscr);
		switch(ch) {
			case KEY_LEFT:
				if(cursor_col > 0)
					cursor_col--;
				break;
			case KEY_RIGHT:
				if(mat[cursor_row][cursor_col] && cursor_col + 1 < MAX)
					cursor_col++;
				break;
			case KEY_UP:
				if(cursor_row > 0) {
					if(cursor_col > strlen(mat[cursor_row - 1]))
						cursor_col = strlen(mat[cursor_row - 1]);
					cursor_row--;
				}
				break;
			case KEY_DOWN:
				if(cursor_row < numrows - 1) {
					if(cursor_col > strlen(mat[cursor_row + 1]))
						cursor_col = strlen(mat[cursor_row + 1]);
					cursor_row++;
				}
				break;
			case KEY_BACKSPACE:
				if(cursor_col == 0) {
					// handle moving all lines up (unless cursor_row == 0)
					if(cursor_row != 0) {
						temp = strlen(mat[cursor_row - 1]);
						delete_newline_pos(cursor_row);
						cursor_row--;
						cursor_col = temp;
					}
				} else {
					delete_pos(cursor_row, cursor_col - 1);
					cursor_col--;
				}
				break;
			default:
				if(ch == NEWLINE) {
					insert_newline_pos(cursor_row, cursor_col);
					cursor_row++;
					cursor_col = 0;
				} else {
					insert_pos(cursor_row, cursor_col, ch);
					cursor_col++;
				}
				break;
		}
		render();
	}
	endwin();
	return 0;
}
