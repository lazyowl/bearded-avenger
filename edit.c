#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>

#define MAX	20

static void finish(int);
static void insert(int);
static void insert_pos(int, int, int);
static void delete();
static void delete_pos(int, int);
static void insert_newline();
static void insert_newline_pos(int, int);
static void delete_newline();
static void delete_newline_pos(int);

static char mat[MAX][MAX];
static int ROW, COL, NUMROWS;

void render() {
	clear();
	int i;
	for(i = 0; i < NUMROWS; i++)
		mvaddstr(i, 0, mat[i]);
	move(ROW, COL);
	refresh();
}

void insert(int ch) {
	insert_pos(ROW, COL, ch);
	COL++;
}

void insert_pos(int r, int c, int ch) {
	int i;
	int len = strlen(mat[r]);
	for(i = len - 1; i >= c; i--)
		mat[r][i + 1] = mat[r][i];
	mat[r][len + 1] = '\0';
	mat[r][c] = ch;
}


void delete() {
	delete_pos(ROW, COL - 1);
	COL--;
}

void delete_pos(int r, int c) {
	int i;
	for(i = c; mat[r][i]; i++) {
		mat[r][i] = mat[r][i + 1];
	}
}

void insert_newline() {
	insert_newline_pos(ROW, COL);
	ROW++;
	COL = 0;
}

// inserts newline after row r
void insert_newline_pos(int r, int c) {
	int i, j;
	char *breakpoint;

	for(i = NUMROWS; i > r; i--)
		strcpy(mat[i + 1], mat[i]);
	breakpoint = &mat[r][c];
	strcpy(mat[r + 1], breakpoint);
	mat[r + 1][strlen(breakpoint)] = '\0';
	mat[r][c] = '\0';
	NUMROWS++;
}


// deletes newline after before ROW
void delete_newline() {
	int temp = strlen(mat[ROW - 1]);
	delete_newline_pos(ROW);
	ROW--;
	COL = temp;
}

// deletes new line at row r (and merges row with r - 1)
void delete_newline_pos(int r) {
	int i;
	strcat(mat[r - 1], mat[r]);
	for(i = r; i < NUMROWS; i++)
		strcpy(mat[r], mat[r + 1]);
	NUMROWS--;
}


void finish(int sig) {
	endwin();
	exit(0);
}

int main() {
	int ch;
	int i;

	NUMROWS = 1;

	signal(SIGINT, finish);

	initscr();	// initialize
	keypad(stdscr, TRUE);	// extra keys
	cbreak();
	nonl();
	noecho();

	while(1) {
		ch = wgetch(stdscr);
		switch(ch) {
			case KEY_LEFT:
				if(COL > 0) COL--;
				break;
			case KEY_RIGHT:
				if(mat[ROW][COL] && COL + 1 < MAX) COL++;
				break;
			case KEY_UP:
				if(ROW > 0) {
					if(COL > strlen(mat[ROW - 1]))
						COL = strlen(mat[ROW - 1]);
					ROW--;
				}
				break;
			case KEY_DOWN:
				if(ROW < NUMROWS - 1) {
					if(COL > strlen(mat[ROW + 1]))
						COL = strlen(mat[ROW + 1]);
					ROW++;
				}
				break;
			case KEY_BACKSPACE:
				if(COL == 0) {
					// handle moving all lines up (unless ROW == 0)
					if(ROW != 0) {
						delete_newline();
					}
				} else {
					delete();
				}
				break;
			default:
				if(ch == '\r')
					insert_newline();
				else
					insert(ch);
				break;
		}
		render();
	}
	getch();
	endwin();
	return 0;
}
