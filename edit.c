#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>

#define MAX	20

void finish(int);
void insert(int);
void insert_pos(int, int, int);
void delete();
void delete_pos(int, int);

char mat[MAX][MAX];
int linelengths[MAX];
int ROW, COL, NUMROWS;

void insert(int ch) {
	insert_pos(ROW, COL, ch);
	COL++;
}

void insert_pos(int r, int c, int ch) {
	int i;
	for(i = linelengths[r] - 1; i >= c; i--) {
		mvaddch(r, i + 1, mat[r][i]);
		mat[r][i + 1] = mat[r][i];
	}
	mvaddch(r, c, ch);
	mat[r][c] = ch;
	linelengths[r]++;
}


void delete() {
	delete_pos(ROW, COL - 1);
	COL--;
}

void delete_pos(int r, int c) {
	int i;
	mvdelch(r, c);
	for(i = c; i < linelengths[r] - 1; i++) {
		mat[r][i] = mat[r][i + 1];
	}
	linelengths[r]--;
}

// inserts newline after row ROW
void insert_newline() {
	int i, j, initial_len;
	for(i = NUMROWS - 1; i > ROW; i--) {
		linelengths[i + 1] = linelengths[i];
		for(j = 0; j < linelengths[i]; j++) {
			mvaddch(i + 1, j, mat[i][j]);
			mat[i + 1][j] = mat[i][j];
		}
	}
	for(i = COL; i < linelengths[ROW]; i++) {
		mvdelch(ROW, i);
	}
	linelengths[ROW + 1] = 0;
	for(i = COL; i < linelengths[ROW]; i++) {
		mvaddch(ROW + 1, i - COL, mat[ROW][i]);
		mat[ROW + 1][i - COL] = mat[ROW][i];
		linelengths[ROW + 1]++;
	}
	mvaddch(ROW, COL, '\n');
	COL = 0;
	NUMROWS++;
	ROW++;
}


// deletes newline after before ROW
void delete_newline() {
	int i, j;
	// content of row ROW is appended to content of row ROW - 1
	for(i = 0; i < linelengths[ROW]; i++) {
		mat[ROW - 1][i + linelengths[ROW - 1]] = mat[ROW][i];
	}
	linelengths[ROW - 1] += linelengths[ROW];
	for(i = ROW; i < NUMROWS - 1; i++) {
		linelengths[i] = linelengths[i + 1];
		for(j = 0; j < linelengths[i + 1]; j++) {
			mat[i][j] = mat[i + 1][j];
		}
	}
	COL = linelengths[ROW];
	ROW--;
	NUMROWS--;
}


void finish(int sig) {
	endwin();
	exit(0);
}

void debug() {
	int i;
	// printw("ROW=%d, linelengths=%d, NUMROWS=%d,", ROW, linelengths[ROW], NUMROWS);
	// for(i = 0; i < NUMROWS; i++) printw("%d ", linelengths[i]);
}

int main() {
	int ch;
	int i;

	for(i = 0; i < MAX; i++)
		linelengths[i] = 0;
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
				if(COL > 0) move(ROW, --COL);
				break;
			case KEY_RIGHT:
				if(COL < linelengths[ROW])
					move(ROW, ++COL);
				break;
			case KEY_UP:
				if(ROW > 0) {
					if(COL > linelengths[ROW - 1]) {
						COL = linelengths[ROW - 1];
					}
					ROW--;
					move(ROW, COL);
				}
				break;
			case KEY_DOWN:
				break;
			case KEY_BACKSPACE:
				if(COL == 0) {
					// handle moving all lines up (unless ROW == 0)
					if(ROW != 0) {
						delete_newline();
					}
				} else {
					delete();
					debug();
				}
				break;
			default:
				if(ch == '\r') {
					// handle moving lines down
					insert_newline();
				} else {
					insert(ch);
					debug();
				}
				break;
		}
		refresh();
	}
	getch();
	endwin();
	return 0;
}
