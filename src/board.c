#include "board.h"
#include <string.h>
#include <stdio.h>

void board_init(Board *board) {
    // Set all cells to EMPTY (value 0)
    memset(board->cells, EMPTY, sizeof(board->cells));
}

int board_drop_piece(Board *board, int col, CellState player) {
    // Check if column number is valid (0-6)
    if (col < 0 || col >= COLS) {
        return -1;
    }

    for (int row = ROWS - 1; row >= 0; row--) {
        if (board->cells[row][col] == EMPTY) {
            board->cells[row][col] = player;  
            return row;
        }
    }
    
    return -1;
}

int board_is_valid_move(const Board *board, int col) {
    // Check column number is in range
    if (col < 0 || col >= COLS) {
        return 0;
    }

    return board->cells[0][col] == EMPTY;
}

int board_is_full(const Board *board) {
    // Check each column's top row
    for (int col = 0; col < COLS; col++) {
        if (board->cells[0][col] == EMPTY) {
            return 0;
        }
    }
    return 1; 
}

void board_print(const Board *board) {
    printf("\n");
    printf(" 0 1 2 3 4 5 6\n");
    printf(" =============\n");

    for (int row = 0; row < ROWS; row++) {
        printf("|");
        for (int col = 0; col < COLS; col++) {
            if (board->cells[row][col] == EMPTY) {
                printf(" ");
            } else if (board->cells[row][col] == PLAYER1) {
                printf("X");
            } else {
                printf("O");
            }
            printf("|");
        }
        printf("\n");
    }
}

int board_check_winner(const Board *board, CellState player) {
    // Check rows
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col <= COLS - 4; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row][col + 1] == player &&
                board->cells[row][col + 2] == player &&
                board->cells[row][col + 3] == player) {
                return 1;  // win
            }
        }
    }
    
    // Check cols
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row + 1][col] == player &&
                board->cells[row + 2][col] == player &&
                board->cells[row + 3][col] == player) {
                return 1; 
            }
        }
    }
    
    // Check diagonal going down-right
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col <= COLS - 4; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row + 1][col + 1] == player &&
                board->cells[row + 2][col + 2] == player &&
                board->cells[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }
    
    // Check diagonal going down-left
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 3; col < COLS; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row + 1][col - 1] == player &&
                board->cells[row + 2][col - 2] == player &&
                board->cells[row + 3][col - 3] == player) {
                return 1; 
            }
        }
    }
    
    return 0;
}

int board_check_draw(const Board *board) {
    if (!board_is_full(board)) {
        return 0; 
    }
    if (board_check_winner(board, PLAYER1)) {
        return 0;
    }
    if (board_check_winner(board, PLAYER2)) {
        return 0; 
    }
    
    return 1; 
}
