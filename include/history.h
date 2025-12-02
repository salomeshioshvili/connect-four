#ifndef HISTORY_H
#define HISTORY_H

#include "board.h"

typedef struct Move {
    int row;   
    int col;  
    CellState player; 
    struct Move *next;
} Move;

void history_add_move(Move **head, int row, int col, CellState player);

int history_undo(Board *board, Move **head, CellState *current_player);

void history_replay(const Move *head, Board *board);

void history_free(Move **head);

int history_print(const Move *head, const char *filename);

#endif
