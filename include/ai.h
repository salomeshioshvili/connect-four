#ifndef AI_H
#define AI_H
#include "board.h"
#include <stdlib.h>
#include "game.h"
// #include <pthread.h> // For future threading implementation

typedef enum {
    AI_EASY,
    AI_MEDIUM,
    AI_HARD,
    AI_EXPERT
} AILevel;

/**
 * @brief Choose the best move for the AI player based on difficulty level
 * @return Column index (0-based) where the AI chooses to drop its piece
 */
int ai_choose_move(const Board *board, CellState ai_player);

/**
 * @brief Easy level AI: Chooses a random valid move, still smart, but does not have advanced strategies. It can lose, but it does the bare minimum (blocks and plays a valid move)
 * @return Column index (0-based)
 */
int ai_easy(const Board *board, CellState ai_player);

/**
 * @brief Medium level AI: Blocks opponent's winning move, can still lose but rarely, thinks ahead.
 * @return Column index (0-based)
 */
int ai_medium(const Board *board, CellState ai_player);

/**
 * @brief Utility based approach for the hard and expert AI levels. 
 * @return Score as an integer
 */
int score_position(const Board *board, int player_id);

/**
 * @brief Hard level AI: Uses minimax algorithm, thinks multiple moves ahead.
 * @return Column index (0-based)
 */
int ai_hard(const Board *board, CellState ai_player);

/**
 * @brief Expert level AI: does not lose, forces a draw if it calculates no winning capabilities. it uses traps, thinks ahead, always blocks attacks, keeps track of the player's pieces and predicts their next move to use it as an advantage, etc.
 * @return Column index (0-based)
 */
int ai_expert(const Board *board, CellState ai_player);

#endif // AI_H 

// Implementing threading: 
// I want to implement threading in the ai_hard and ai_expert functions to allow the AI to think ahead without blocking the main game loop. This will improve the user experience by keeping the game responsive while the AI is calculating its move.
// I don't want to break the project though, so I want to make sure that the logic works without threading first, and then I can add threading later to make sure I did not break anything. 
// To implement threading, I will use the pthread library to create a separate thread for the AI's move calculation. This thread will run the minimax algorithm or other complex logic without interfering with the main game loop.
// typedef struct {
//     Board board_copy;
//     CellState ai_player;
//     AILevel ai_level;
//     int result;
// } AIThread;
// void* ai_thread_function(void* arg);