#ifndef BOARD_H
#define BOARD_H

#define ROWS 6
#define COLS 7

typedef enum {
    EMPTY,
    PLAYER1,
    PLAYER2
} CellState;

typedef struct {
    CellState cells[ROWS][COLS];
} Board;

/**
 * @brief Initialize an empty board
 */
void board_init(Board *board);

/**
 * @brief Drop a piece in the specified column
 * @return Row where piece was placed, or -1 if column is full/invalid
 */
int board_drop_piece(Board *board, int col, CellState player);

/**
 * @brief Check if a move is valid
 * @return 1 if valid, 0 otherwise
 */
int board_is_valid_move(const Board *board, int col);

/**
 * @brief Check if the board is completely full
 * @return 1 if full, 0 otherwise
 */
int board_is_full(const Board *board);

/**
 * @brief Print the board to console (for debugging)
 */
void board_print(const Board *board);

/**
 * @brief Check if a player has won the game
 * @return 1 if player has won, 0 otherwise
 */
int board_check_winner(const Board *board, CellState player);

/**
 * @brief Check if the game is a draw (board full with no winner)
 * @return 1 if draw, 0 otherwise
 */
int board_check_draw(const Board *board);

#endif // BOARD_H