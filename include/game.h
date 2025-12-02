#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "history.h"
#include "ai.h"

typedef enum {
    GAME_MODE_PVP, 
    GAME_MODE_PVAI
} GameMode;

typedef struct {
    Board board;
    CellState current_player;
    GameMode mode;
    CellState ai_player; 
    AILevel ai_level;          
    Move *history;  
    int is_over;
    CellState winner;
    int is_draw;  
} Game;

/**
 * @brief Initialize a new game
 * @param game Game struct to initialize
 * @param mode GAME_MODE_PVP or GAME_MODE_PVAI
 * @param starting_player PLAYER1 or PLAYER2
 * @param ai_player Which player is AI in PVAI mode (PLAYER1/PLAYER2). Ignored for PVP.
 */
void game_init(Game *game, GameMode mode, CellState starting_player, CellState ai_player, AILevel ai_level);

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
