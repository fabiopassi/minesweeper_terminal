#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <sys/time.h>
#include <time.h>
#include "lib.h"

/* MINESWEEPER GAME */

int main (int argc, char **argv) {

	srand(time(NULL));
	
	/* Variables */
	int quit = 0;
	int gameover = 0;
	int win = 0;
	int dx_shown = COLS / 10;
	int dy_shown = ROWS / 10;
	int x_cursor = rand() % ( (COLS - dx_shown) - dx_shown ) + dx_shown + 1;
	int y_cursor = rand() % ( (ROWS - dy_shown) - dy_shown ) + dy_shown + 1;

	Block** game_board = malloc( (ROWS + 2) * sizeof(*game_board));
	for (int i = 0; i < ROWS + 2; i++) {
		game_board[i] = malloc( (COLS + 2) * sizeof(*game_board[i]));
	}

	/* Initialize blocks */
	for(int i = 0; i < ROWS + 2; i++) {
		for (int j = 0; j < COLS + 2; j++) {
			game_board[i][j].bool_bomb = 0;
			game_board[i][j].bool_flag = 0;
			game_board[i][j].neigh_bombs = 0;
			game_board[i][j].discovered = 0;
			
			if (i == 0 || i == ROWS + 1 || j == 0 || j == COLS + 1) {
				game_board[i][j].discovered = -1;		/* The first/last row/column are not accessible, they just make cycles easier in checking neighbouring bombs */
			}
		}
	}

	/* Initialize bombs */
	init_bombs(game_board, x_cursor, y_cursor, dx_shown, dy_shown);

	/* Evaluate the number of neighbouring bombs for each block */
	for(int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {
            eval_neigh_bombs(game_board, i, j);
        }   
    }

	/* Initialize ncurses */
	initscr();

	/* ncurses settings and colors */
	noecho();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);

	if (!has_colors()) {
		printf("Error : you program does not support colors.\n\n");
		endwin();
		return EXIT_FAILURE;
	}
	if (start_color() != OK) {
		printf("Error : start_color() function did not coomplete succesfully.\n\n");
		endwin();
		return EXIT_FAILURE;
	}

	init_color(24, 999, 600, 0);		/* Orange */

	init_pair(8, COLOR_BLACK, COLOR_CYAN);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(9, COLOR_BLACK, 24);
	init_pair(10, COLOR_WHITE, COLOR_RED);

	/* Print the commands */
	mvprintw(COLS/2 - 2, ROWS + 2 + 15, "Commands:");
	mvprintw(COLS/2    , ROWS + 2 + 15, "arrows : move");
	mvprintw(COLS/2 + 1, ROWS + 2 + 15, "  p    : flag");
	mvprintw(COLS/2 + 2, ROWS + 2 + 15, "  d    : select the block");
	mvprintw(COLS/2 + 3, ROWS + 2 + 15, "  q    : quit");
	refresh();

	/* Show the initial blocks and move the cursor to the correct position */
	init_display(game_board, x_cursor, y_cursor, dx_shown, dy_shown);
	for (int i = 0; i < 5; i++) {
		check_empty_neighbours(game_board);
		print_neighbours(game_board);
	}
	move(y_cursor, x_cursor);

	/* Game */
	while ( ! gameover && ! win && ! quit ) {

		quit = read_keyboard(&x_cursor, &y_cursor, game_board, &gameover, &win);
		move(y_cursor, x_cursor);
		refresh();
		napms(30);
	
	}

	/* Closing sentence */
	nodelay(stdscr, FALSE);

	if (win == 1) {
		print_all(game_board);
		mvprintw(COLS + 4, 1, "Congratulations! You won!");
		mvprintw(COLS + 6, 1, "Press any key to exit");
		refresh();
	}
	
	if (gameover == 1) {
		print_all(game_board);
		mvprintw(COLS + 4, 1, "Gameover!");
		mvprintw(COLS + 6, 1, "Press any key to exit");
		refresh();
	}

	if (quit == 1) {
		mvprintw(COLS + 4, 1, "Thank you for having played minesweeper!");
		mvprintw(COLS + 6, 1, "Press any key to exit");
		refresh();
	}

	getchar();

	/* Closing ncurses */
	endwin();

	/* Free memory */
	for (int i = 0; i < ROWS + 2; i++) {
		free(game_board[i]);
	}
	free(game_board);

	exit(0);
}

