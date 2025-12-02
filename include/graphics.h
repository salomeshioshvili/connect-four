#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "board.h"
#include <SDL2/SDL.h>

#define CELL_SIZE 80
#define BOARD_PADDING 40
#define WINDOW_WIDTH (COLS * CELL_SIZE + 2 * BOARD_PADDING)
#define WINDOW_HEIGHT (ROWS * CELL_SIZE + 2 * BOARD_PADDING + 80)

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
    int selected_column;  // Column currently hovered (-1 if none)
} Graphics;

/**
 * @brief Initialize SDL2 and create window/renderer
 * @return 0 on success, -1 on failure
 */
int graphics_init(Graphics *gfx, const char *title);

/**
 * @brief Clean up SDL2 resources
 */
void graphics_cleanup(Graphics *gfx);

/**
 * @brief Render the game board
 * @param gfx Graphics context
 * @param board Current board state
 * @param current_player Whose turn it is
 * @param game_over Whether the game has ended
 * @param winner The winner (EMPTY if none/draw)
 * @param is_draw 1 if game ended in draw
 */
void graphics_render(Graphics *gfx, const Board *board, CellState current_player, 
                     int game_over, CellState winner, int is_draw);

/**
 * @brief Handle SDL events
 * @param gfx Graphics context
 * @param col_out Output: column clicked (-1 if none)
 * @param quit_out Output: 1 if user wants to quit
 * @param undo_out Output: 1 if user pressed undo
 */
void graphics_handle_events(Graphics *gfx, int *col_out, int *quit_out, int *undo_out);

/**
 * @brief Convert mouse x position to column index
 * @return Column index (0 to COLS-1), or -1 if outside board
 */
int graphics_get_column_from_x(int mouse_x);

/**
 * @brief Wait for user to click or press key to continue
 * @return 1 if user wants to play again, 0 to quit
 */
int graphics_wait_for_restart(Graphics *gfx);

#endif
