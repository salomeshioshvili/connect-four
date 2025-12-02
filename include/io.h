#ifndef IO_H
#define IO_H

#include "game.h"

void print_menu(void);
void print_ai_level_menu(void);
void print_player_choice_menu(void);
int get_menu_choice(int min, int max);

void clear_screen(void);
const char *player_name(CellState p);
int prompt_human_move(int allow_undo);

#ifdef HAS_GRAPHICS
/**
 * @brief Run the game with graphics mode
 */
void run_graphics_game(GameMode mode, CellState ai_player, AILevel ai_level);
#endif

#endif
