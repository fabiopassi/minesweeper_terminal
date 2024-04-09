#ifndef __LIB_H__
#define __LIB_H__


/* Constants */

#define INPUT_BUFFER_SIZE 4096
#define ROWS 20
#define COLS 20
#define NUM_BOMBS 55



/* Typedef */

typedef struct {
    int neigh_bombs;
    int bool_bomb;
    int bool_flag;
    int discovered;
} Block;



/* Headers of functions */

void init_bombs(Block** game_board, int x_init, int y_init, int dx_shown, int dy_shown);

void eval_neigh_bombs(Block** game_board, int n, int m);

void print_block(Block block);

void print_neighbours(Block** game_board);

void check_empty_neighbours(Block** game_board);

void init_display(Block** game_board, int x_cursor, int y_cursor, int dx_shown, int dy_shown);

void move_cursor(int* x_cursor, int* y_cursor, int dx, int dy);

int check_win(Block** game_board);

int read_keyboard(int* x_cursor, int* y_cursor, Block** game_board, int* gameover, int* win);

void print_all(Block** game_board);

#endif

