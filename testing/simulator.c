// simulates performing the key strokes as indicated in the specified file
#include <stdio.h>
#include <stdlib.h>
#include "../editor.h"

#define BUF_LEN	100
#define MAX	10

typedef enum {
	INSSTR, BP, INSNL, MVCRSR, MVCRSRUP, MVCRSRDN, MVCRSRRT, MVCRSRLFT, INVALID
} Op;

typedef union Arg {
	char str[MAX];
	int num1;
	int num2;
} Arg;

typedef struct Command {
	Op op;
	Arg arg;
} Command;

// parse commands from file and put into array
int parse_file(char *filename, Command commands[]) {
	char buf[BUF_LEN];
	char *piece1, *piece2, *piece3;
	int i = 0;
	FILE *f = fopen(filename, "r");
	// I assume that BUF_LEN (100) is sufficient to hold the line
	while(fgets(buf, BUF_LEN, f)) {
		if(buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
		piece1 = strtok(buf, " ");
		if(!piece1)
			continue;
		if(piece1[0] == '#')
			continue;
		commands[i].op = INVALID;
		if(strcmp(piece1, "INSSTR") == 0) commands[i].op = INSSTR;
		if(strcmp(piece1, "MVCRSR") == 0) commands[i].op = MVCRSR;
		if(strcmp(piece1, "MVCRSRUP") == 0) commands[i].op = MVCRSRUP;
		if(strcmp(piece1, "MVCRSRDN") == 0) commands[i].op = MVCRSRDN;
		if(strcmp(piece1, "MVCRSRRT") == 0) commands[i].op = MVCRSRRT;
		if(strcmp(piece1, "MVCRSRLFT") == 0) commands[i].op = MVCRSRLFT;
		if(strcmp(piece1, "BP") == 0) commands[i].op = BP;
		if(strcmp(piece1, "INSNL") == 0) commands[i].op = INSNL;

		switch(commands[i].op) {
			case INSSTR:
				piece2 = strtok(NULL, " ");
				strcpy(commands[i].arg.str, piece2);
				break;
			case MVCRSR:
				piece2 = strtok(NULL, " ");	// row
				piece3 = strtok(NULL, " ");	// col
				commands[i].arg.num1 = atoi(piece2);
				commands[i].arg.num2 = atoi(piece3);
				break;
			case BP:
			case INSNL:
			case MVCRSRUP:
			case MVCRSRDN:
			case MVCRSRRT:
			case MVCRSRLFT:
				break;
			default:
				break;
		}
		i++;
	}
	fclose(f);
	return i;
}

void insert_string(CMatrix *cmtx, char str[]) {
	int i;
	for(i = 0; str[i]; i++)
		insert_at_cursor(cmtx, str[i]);
}

void print_matrix(CMatrix *cmtx) {
	Line *current = cmtx->head;
	while(current) {
		printf("%s\n", current->arr);
		current = current->next;
	}
}

int main(int argc, char **argv) {
	int count, i;
	CMatrix matrix;

	if(argc != 2) return 1;

	init_blank(&matrix);

	Command commands[MAX];
	count = parse_file(argv[1], commands);
	for(i = 0; i < count; i++) {
		switch(commands[i].op) {
			case INSSTR:
				insert_string(&matrix, commands[i].arg.str);
				break;
			case BP:
				if(matrix.cursor_col == 0) {
					if(matrix.cursor_line != matrix.head) {
						delete_newline_at_cursor(&matrix);
					} 
				} else {
					delete_before_cursor(&matrix);
				}
				break;
			case INSNL:
				insert_newline_at_cursor(&matrix);
				break;
			case MVCRSR:
				move_cursor_to(&matrix, commands[i].arg.num1, commands[i].arg.num2);
				break;
			case MVCRSRUP:
				if(matrix.cursor_line != matrix.head) {
					move_cursor_up(&matrix);
				}
				break;
			case MVCRSRDN:
				if(matrix.cursor_line->next) {
					move_cursor_down(&matrix);
				}
				break;
			case MVCRSRLFT:
				if(matrix.cursor_col > 0)
					matrix.cursor_col--;
				break;
			case MVCRSRRT:
				if(matrix.cursor_line->arr[matrix.cursor_col] && matrix.cursor_col < strlen(matrix.cursor_line->arr))
					matrix.cursor_col++;
				break;
			default:
				printf("Invalid command!\n");
				break;
		}
	}

	print_matrix(&matrix);
	printf("(%d, %d)\n", matrix.cursor_line_int, matrix.cursor_col);
	return 0;
}

