#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <ncurses.h>
#include "lib.h"


void init_bombs(Block** game_board, int x_init, int y_init, int dx_shown, int dy_shown) {
    for (int i = 0; i < NUM_BOMBS; i++) {

        int x_bomb = rand() % (ROWS) + 1;
        int y_bomb = rand() % (COLS) + 1; 

        /* If the bomb is in the initially shown part of the board or if the bomb overlaps with another bomb, I move it */
		while ( (x_bomb >= x_init - dx_shown && x_bomb <= x_init + dx_shown && y_bomb >= y_init - dy_shown && y_bomb <= y_init + dy_shown) || (game_board[y_bomb][x_bomb].bool_bomb == 1) ) {
			
            x_bomb = rand() % (COLS) + 1;
			y_bomb = rand() % (ROWS) + 1;

		}

        game_board[y_bomb][x_bomb].bool_bomb = 1;

	}
}



void eval_neigh_bombs(Block** game_board, int n, int m){
    for (int i = -1; i <= 1; i++ ) {
        for (int j = -1; j <= 1; j++) {
            if (game_board[n+i][m+j].bool_bomb == 1) {
                game_board[n][m].neigh_bombs += 1;
            }
        }
    }
}



void print_neighbours(Block** game_board) {

    for(int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {

            if (game_board[i][j].discovered == 1 && game_board[i][j].neigh_bombs == 0) {

                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if ( i+di > 0 && i+di < ROWS + 1 && j+dj > 0 && j+dj < COLS + 1 ) {
                            game_board[i+di][j+dj].discovered = 1;
                            move(i+di, j+dj);
                            print_block(game_board[i+di][j+dj]);
                        }
                    }
                }

            }

        }
    }

    

}



void check_empty_neighbours(Block** game_board){
    
    int stop = 0;

    while (! stop) {

        stop = 1;

        for(int i = 1; i <= ROWS; i++) {
            for (int j = 1; j <= COLS; j++) {
                for (int n = -1; n <= 1; n++) {
                    for (int m = -1; m <= 1; m++) {

                        if ( game_board[i][j].neigh_bombs == 0 && game_board[i][j].discovered == 0 && game_board[i+n][j+m].discovered == 1) {
                            game_board[i][j].discovered = 1;
                            move(i, j);
                            print_block(game_board[i][j]);
                            stop = 0;
                            break;
                            break;
                        }

                    }
                }
            }
        }  
    }
}




void print_block(Block block){

    switch(block.neigh_bombs) {

        case 0 :
            attron(COLOR_PAIR(1));
            addch(' ');
            attroff(COLOR_PAIR(1));
            break;

        case 1 :
            attron(COLOR_PAIR(1));
            addch('1');
            attroff(COLOR_PAIR(1));
            break;
        
        case 2 :
            attron(COLOR_PAIR(2));
            addch('2');
            attroff(COLOR_PAIR(2));
            break;
        
        case 3 :
            attron(COLOR_PAIR(3));
            addch('3');
            attroff(COLOR_PAIR(3));
            break;

        case 4 :
            attron(COLOR_PAIR(4));
            addch('4');
            attroff(COLOR_PAIR(4));
            break;
        default :
            printw("%d", block.neigh_bombs);
            break;    
    }
}



void init_display(Block** game_board, int x_cursor, int y_cursor, int dx_shown, int dy_shown) {

    for (int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {

            attron(COLOR_PAIR(8));
            mvaddch(i, j, '.');
            attroff(COLOR_PAIR(8));

            if( y_cursor - dy_shown <= i && i <= y_cursor + dy_shown && x_cursor - dx_shown <= j && j <= x_cursor + dx_shown ){
                move(i,j);
                print_block(game_board[i][j]);
                game_board[i][j].discovered = 1;
            }

        }
    }
    
}



void move_cursor(int* x_cursor, int* y_cursor, int dx, int dy){

    if ( dy == -1 && *y_cursor != 1 ) {
        *y_cursor += dy;
    }

    if ( dy == +1 && *y_cursor != ROWS ) {
        *y_cursor += dy;
    }

    if ( dx == -1 && *x_cursor != 1 ) {
        *x_cursor += dx;
    }

    if ( dx == +1 && *x_cursor != COLS ) {
        *x_cursor += dx;
    }

}



int check_win(Block** game_board){
    int win = 1;

    for (int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {
            if ( game_board[i][j].bool_bomb == 0 && game_board[i][j].discovered == 0) {
                win = 0;
            }
        }
    }

    return win;
}



int read_keyboard(int* x_cursor, int* y_cursor, Block** game_board, int* gameover, int* win) {

    int ch = getch();

    switch (ch) {

    case KEY_UP :
        move_cursor(x_cursor, y_cursor, 0, -1);
        break;

    case KEY_LEFT :
        move_cursor(x_cursor, y_cursor, -1, 0);
        break;

    case KEY_DOWN :
        move_cursor(x_cursor, y_cursor, 0, +1);
        break;

    case KEY_RIGHT :
        move_cursor(x_cursor, y_cursor, +1, 0);
        break;

    case 'p' :
        int x_flag, y_flag;
        getyx(stdscr, y_flag, x_flag);
        if (game_board[y_flag][x_flag].bool_flag == 1) {

            game_board[y_flag][x_flag].bool_flag = 0;

            if (game_board[y_flag][x_flag].discovered == 0) {
                attron(COLOR_PAIR(8));
                mvaddch(y_flag, x_flag, '.');
                attroff(COLOR_PAIR(8));
            } else {
                move(y_flag, x_flag);
                print_block(game_board[y_flag][x_flag]);
            }

        } else {
            game_board[y_flag][x_flag].bool_flag = 1;
            attron(COLOR_PAIR(9));
            mvaddch(y_flag, x_flag, 'P');
            attroff(COLOR_PAIR(9));
        }
        break;

    case 'd' :
        int x_new_block, y_new_block;
        getyx(stdscr, y_new_block, x_new_block);

        if (game_board[y_new_block][x_new_block].bool_flag == 0) {

            if (game_board[y_new_block][x_new_block].bool_bomb == 1) {
                *gameover = 1;
            } else {
                game_board[y_new_block][x_new_block].discovered = 1;
                move(y_new_block, x_new_block);
                print_block(game_board[y_new_block][x_new_block]);
                *win = check_win(game_board);
                check_empty_neighbours(game_board);
                print_neighbours(game_board);
            }

        }

        break;

    case 'q' :
        return 1;
        break;
        
    }

    flushinp();

    return 0;
}



void print_all(Block** game_board){

    for(int i = 1; i <= ROWS; i++) {
        for (int j = 1; j <= COLS; j++) {

            move(i,j);
            if (game_board[i][j].bool_bomb == 0) {
                print_block(game_board[i][j]);
            } else {
                attron(COLOR_PAIR(10));
                addch('o');
                attroff(COLOR_PAIR(10));
            }
        }
    }
}



