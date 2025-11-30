#ifndef AI_H
#define AI_H
#include "board.h"
#include <stdlib.h>
#include <pthread.h>

typedef enum {
    AI_EASY,
    AI_MEDIUM,
    AI_HARD,
    AI_EXPERT
} AILevel;

typedef struct {
    Board board_copy;
    CellState ai_player;
    AILevel ai_level;
    int result;
} AIThread;

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
/**
 * @brief The famous thread function that runs all the AI computations in parallel
 */
void* ai_thread_function(void* arg);

#endif // AI_H 
