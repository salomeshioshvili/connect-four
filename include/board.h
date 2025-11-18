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

#endif // BOARD_H