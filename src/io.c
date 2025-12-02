#include "io.h"
#include <stdio.h>
#include <ctype.h>

void print_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("         CONNECT FOUR GAME\n");
    printf("========================================\n");
    printf("\n");
    printf("Select Game Mode:\n");
    printf("  1. Player vs Player (Console)\n");
    printf("  2. Player vs AI (Console)\n");
#ifdef HAS_GRAPHICS
    printf("  3. Player vs Player (Graphics)\n");
    printf("  4. Player vs AI (Graphics)\n");
#endif
    printf("  q. Quit\n");
    printf("\n");
    printf("Enter choice: ");
}

void print_ai_level_menu(void) {
    printf("\n");
    printf("Select AI Difficulty:\n");
    printf("  1. Easy   (random moves)\n");
    printf("  2. Medium (blocks + wins)\n");
    printf("  3. Hard   (strategic)\n");
    printf("  4. Expert (unbeatable)\n");
    printf("\n");
    printf("Enter choice: ");
}

void print_player_choice_menu(void) {
    printf("\n");
    printf("Who goes first?\n");
    printf("  1. You (Player 1 - X/Red)\n");
    printf("  2. AI (Player 2 - O/Yellow)\n");
    printf("\n");
    printf("Enter choice: ");
}

int get_menu_choice(int min, int max) {
    char buffer[64];
    
    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return -1;
        }
        
        char *p = buffer;
        while (*p && isspace((unsigned char)*p)) p++;
        
        if (*p == 'q' || *p == 'Q') return -1;
        
        int choice;
        if (sscanf(p, "%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        }
        
        printf("Invalid. Enter %d-%d or 'q': ", min, max);
    }
}

#ifdef HAS_GRAPHICS
#include "graphics.h"

void run_graphics_game(GameMode mode, CellState ai_player, AILevel ai_level) {
    Graphics gfx;
    
    if (graphics_init(&gfx, "Connect Four") != 0) {
        fprintf(stderr, "Failed to initialize graphics. Falling back to console mode.\n");
        return;
    }
    
    do {
        Game game;
        game_init(&game, mode, PLAYER1, ai_player, ai_level);
        
        while (!game.is_over && gfx.running) {
            int col = -1, quit = 0, undo = 0;
            
            graphics_render(&gfx, &game.board, game.current_player, 
                          game.is_over, game.winner, game.is_draw);
            
            int is_ai_turn = (game.mode == GAME_MODE_PVAI && 
                             game.current_player == game.ai_player);
            
            if (is_ai_turn) {
                SDL_Delay(300);
                
                int ai_col;
                if (game.ai_level == AI_EASY) {
                    ai_col = ai_easy(&game.board, game.current_player);
                } else if (game.ai_level == AI_MEDIUM) {
                    ai_col = ai_medium(&game.board, game.current_player);
                } else if (game.ai_level == AI_HARD) {
                    ai_col = ai_hard(&game.board, game.current_player);
                } else {
                    ai_col = ai_expert(&game.board, game.current_player);
                }
                
                if (ai_col >= 0 && ai_col < COLS && board_is_valid_move(&game.board, ai_col)) {
                    int row = board_drop_piece(&game.board, ai_col, game.current_player);
                    if (row >= 0) {
                        history_add_move(&game.history, row, ai_col, game.current_player);
                    }
                }
            } else {
                while (col < 0 && !quit && !undo && gfx.running) {
                    graphics_handle_events(&gfx, &col, &quit, &undo);
                    graphics_render(&gfx, &game.board, game.current_player,
                                  game.is_over, game.winner, game.is_draw);
                    SDL_Delay(16);
                }
                
                if (quit) {
                    game.is_over = 1;
                    break;
                }
                
                if (undo && game.mode == GAME_MODE_PVAI) {
                    CellState dummy;
                    history_undo(&game.board, &game.history, &dummy);
                    history_undo(&game.board, &game.history, &dummy);
                    continue;
                }
                
                if (col >= 0 && board_is_valid_move(&game.board, col)) {
                    int row = board_drop_piece(&game.board, col, game.current_player);
                    if (row >= 0) {
                        history_add_move(&game.history, row, col, game.current_player);
                    }
                } else {
                    continue;
                }
            }
            
            if (board_check_winner(&game.board, game.current_player)) {
                game.is_over = 1;
                game.winner = game.current_player;
            } else if (board_check_draw(&game.board)) {
                game.is_over = 1;
                game.is_draw = 1;
            } else {
                game.current_player = (game.current_player == PLAYER1) ? PLAYER2 : PLAYER1;
            }
        }
        
        graphics_render(&gfx, &game.board, game.current_player,
                      game.is_over, game.winner, game.is_draw);
        
        game_cleanup(&game);
        
    } while (gfx.running && graphics_wait_for_restart(&gfx));
    
    graphics_cleanup(&gfx);
}
#endif
