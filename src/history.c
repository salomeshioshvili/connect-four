#include <stdlib.h>
#include "history.h"
#include <stdio.h>


/* allocate and initialize a new move node. */
static Move *create_move_node(int row, int col, CellState player)
{
    Move *m = (Move *)malloc(sizeof(Move));
    if (m == NULL) {
        return NULL;    // allocation failed
    }

    m->row = row;
    m->col = col;
    m->player = player;
    m->next = NULL;

    return m;
}

void history_add_move(Move **head, int row, int col, CellState player)
{
    if (head == NULL) {
        return;
    }

    Move *new_node = create_move_node(row, col, player);
    if (new_node == NULL) {
        // allocation failed, do nothing
        return;
    }

    // If list is empty, new node becomes the head.
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    // Otherwise, append at the end.
    Move *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int history_undo(Board *board, Move **head, CellState *current_player)
{
    Move *current;
    Move *prev;

    if (board == NULL || head == NULL || current_player == NULL) {
        return 0;
    }

    if (*head == NULL) {
        // No moves to undo.
        return 0;
    }

    current = *head;
    prev = NULL;

    // Find the last node (current) and its predecessor (prev).
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    // current is now the last move.
    // Remove the piece from the board at that move's position.
    if (current->row >= 0 && current->row < ROWS &&
        current->col >= 0 && current->col < COLS) {
        board->cells[current->row][current->col] = EMPTY;
    }

    // Set current_player back to whoever made that move.
    *current_player = current->player;

    // Remove the node from the list.
    if (prev == NULL) {
        // Only one node in the list.
        *head = NULL;
    } else {
        prev->next = NULL;
    }

    free(current);
    return 1;
}

void history_replay(const Move *head, Board *board)
{
    if (board == NULL) {
        return;
    }

    // Start from a clean board.
    board_init(board);

    // Reapply each move in order.
    const Move *current = head;
    while (current != NULL) {
        // We can rely on board_drop_piece to place the piece correctly
        // based on the column and the current board state.
        board_drop_piece(board, current->col, current->player);
        current = current->next;
    }
}

void history_free(Move **head)
{
    if (head == NULL) {
        return;
    }

    Move *current = *head;
    while (current != NULL) {
        Move *next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

void history_print(const Move *head, const char *filename) {
    const Move *current = head;
    FILE *file;
    int move_number = 1;

    file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }

    if (current == NULL) {
        fprintf(file, "No moves were played.\n");
        fclose(file);
        return 1;
    }

    while (current != NULL) {
        const char *player_name;

        if (current->player == PLAYER1) {
            player_name = "Player 1 (X)";
        } else if (current->player == PLAYER2) {
            player_name = "Player 2 (O)";
        } else {
            player_name = "Unknown";
        }

        fprintf(file,
                "Move %d: %s -> column %d, row %d\n",
                move_number,
                player_name,
                current->col,
                current->row);

        move_number = move_number + 1;
        current = current->next;
    }

    fclose(file);
    return 1;
}