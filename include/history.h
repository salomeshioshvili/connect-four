#ifndef HISTORY_H
#define HISTORY_H

#include "board.h"

/* A single move in the game */
typedef struct Move {
    int row;              // row where piece landed
    int col;              // column where piece was dropped
    CellState player;     // which player made the move
    struct Move *next;    // next move in the list
} Move;

/* Add a new move to the end of the list */
void history_add_move(Move **head, int row, int col, CellState player);

/* Undo the last move (remove piece + update current_player).
   Returns 1 if undone, 0 if no moves. */
int history_undo(Board *board, Move **head, CellState *current_player);

/* Replay all moves on a fresh board */
void history_replay(const Move *head, Board *board);

/* Free all moves in the list */
void history_free(Move **head);

/* print all the moves in the list*/
int history_print(const Move *head, const char *filename);

#endif
