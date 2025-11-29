#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "history.h"

typedef enum {
    GAME_MODE_PVP,   // Player vs Player
    GAME_MODE_PVAI   // Player vs AI
} GameMode;

typedef struct {
    Board board;
    CellState current_player;  // PLAYER1 or PLAYER2
    GameMode mode;
    CellState ai_player;       // which player is controlled by AI in PVAI (PLAYER1/PLAYER2), EMPTY if none
    Move *history;             // linked list of moves
    int is_over;               // 0 = running, 1 = finished
    CellState winner;          // EMPTY if none
    int is_draw;               // 1 if draw, 0 otherwise
} Game;

/**
 * @brief Initialize a new game
 * @param game Game struct to initialize
 * @param mode GAME_MODE_PVP or GAME_MODE_PVAI
 * @param starting_player PLAYER1 or PLAYER2
 * @param ai_player Which player is AI in PVAI mode (PLAYER1/PLAYER2). Ignored for PVP.
 */
void game_init(Game *game, GameMode mode, CellState starting_player, CellState ai_player);

/**
 * @brief Run the main game loop (blocking). Handles:
 *        - human/AI turns
 *        - undo (only in PVAI)
 *        - win/draw detection
 *        - quitting
 *        and prints to console using board_print.
 */
void game_run(Game *game);

/**
 * @brief Free any dynamically allocated resources in Game (history list).
 */
void game_cleanup(Game *game);

#endif
