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
        return -1;  // Invalid column
    }
    
    // Start from bottom row and go up, find first empty spot
    for (int row = ROWS - 1; row >= 0; row--) {
        if (board->cells[row][col] == EMPTY) {
            board->cells[row][col] = player;  
            return row;
        }
    }
    
    return -1;  // Column is full
}

int board_is_valid_move(const Board *board, int col) {
    // Check column number is in range
    if (col < 0 || col >= COLS) {
        return 0;  // Invalid
    }
    
    // Check if top row of this column is empty
    return board->cells[0][col] == EMPTY;
}

int board_is_full(const Board *board) {
    // Check each column's top row
    for (int col = 0; col < COLS; col++) {
        if (board->cells[0][col] == EMPTY) {
            return 0;  // Found an empty spot
        }
    }
    return 1;  // All columns full
}

void board_print(const Board *board) {
    printf("\n");
    printf(" 0 1 2 3 4 5 6\n");  // Column numbers
    printf(" =============\n");
    
    // Print each row
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
    // Check horizontal (rows)
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col <= COLS - 4; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row][col + 1] == player &&
                board->cells[row][col + 2] == player &&
                board->cells[row][col + 3] == player) {
                return 1;  // Found 4 in a row
            }
        }
    }
    
    // Check vertical (columns)
    for (int row = 0; row <= ROWS - 4; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board->cells[row][col] == player &&
                board->cells[row + 1][col] == player &&
                board->cells[row + 2][col] == player &&
                board->cells[row + 3][col] == player) {
                return 1;  // Found 4 in a column
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
                return 1;  // Found diagonal
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
                return 1;  // Found diagonal
            }
        }
    }
    
    return 0;  // No winner yet
}

int board_check_draw(const Board *board) {
    // Check if board is full
    if (!board_is_full(board)) {
        return 0;  // Not full, can't be a draw
    }
    
    // Check if either player won
    if (board_check_winner(board, PLAYER1)) {
        return 0;  // Player 1 won, not a draw
    }
    if (board_check_winner(board, PLAYER2)) {
        return 0;  // Player 2 won, not a draw
    }
    
    return 1;  // Board full with no winner = draw
}
