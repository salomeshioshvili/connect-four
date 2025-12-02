#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "game.h"
#include "board.h"
#include "ai.h"

#ifdef HAS_GRAPHICS
#include "graphics.h"

static void run_graphics_game(GameMode mode, CellState ai_player, AILevel ai_level) {
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
            
            // current state
            graphics_render(&gfx, &game.board, game.current_player, 
                          game.is_over, game.winner, game.is_draw);
            
            int is_ai_turn = (game.mode == GAME_MODE_PVAI && 
                             game.current_player == game.ai_player);
            
            if (is_ai_turn) {
                // Delay so player can see the board
                SDL_Delay(300);
                
                // AI makes move
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
                // our's turn - wait for input
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
                    // Undo both AI and player moves
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
                    continue; // Invalid move, try again
                }
            }
            
            // Check for win/draw
            if (board_check_winner(&game.board, game.current_player)) {
                game.is_over = 1;
                game.winner = game.current_player;
            } else if (board_check_draw(&game.board)) {
                game.is_over = 1;
                game.is_draw = 1;
            } else {
                // Switch player
                game.current_player = (game.current_player == PLAYER1) ? PLAYER2 : PLAYER1;
            }
        }
        
        // Show final state
        graphics_render(&gfx, &game.board, game.current_player,
                      game.is_over, game.winner, game.is_draw);
        
        game_cleanup(&game);
        
    } while (gfx.running && graphics_wait_for_restart(&gfx));
    
    graphics_cleanup(&gfx);
}
#endif

static void print_menu(void) {
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

static void print_ai_level_menu(void) {
    printf("\n");
    printf("Select AI Difficulty:\n");
    printf("  1. Easy   (random moves)\n");
    printf("  2. Medium (blocks + wins)\n");
    printf("  3. Hard   (strategic)\n");
    printf("  4. Expert (unbeatable)\n");
    printf("\n");
    printf("Enter choice: ");
}

static void print_player_choice_menu(void) {
    printf("\n");
    printf("Who goes first?\n");
    printf("  1. You (Player 1 - X/Red)\n");
    printf("  2. AI (Player 2 - O/Yellow)\n");
    printf("\n");
    printf("Enter choice: ");
}

static int get_menu_choice(int min, int max) {
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

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    srand((unsigned int)time(NULL));
    
    while (1) {
        print_menu();
        
#ifdef HAS_GRAPHICS
        int max_choice = 4;
#else
        int max_choice = 2;
#endif
        
        int mode_choice = get_menu_choice(1, max_choice);
        if (mode_choice == -1) {
            printf("Goodbye!\n");
            break;
        }
        
        GameMode mode;
        CellState ai_player = EMPTY;
        AILevel ai_level = AI_EASY;
        int use_graphics = 0;
        
        // Determine mode and graphics
#ifdef HAS_GRAPHICS
        if (mode_choice == 3) {
            mode = GAME_MODE_PVP;
            use_graphics = 1;
        } else if (mode_choice == 4) {
            mode = GAME_MODE_PVAI;
            use_graphics = 1;
        } else
#endif
        if (mode_choice == 1) {
            mode = GAME_MODE_PVP;
        } else {
            mode = GAME_MODE_PVAI;
        }
        
        // Get AI settings if needed
        if (mode == GAME_MODE_PVAI) {
            print_ai_level_menu();
            int level = get_menu_choice(1, 4);
            if (level == -1) { printf("Goodbye!\n"); break; }
            
            ai_level = (AILevel)(level - 1);
            
            print_player_choice_menu();
            int who_first = get_menu_choice(1, 2);
            if (who_first == -1) { printf("Goodbye!\n"); break; }
            
            ai_player = (who_first == 1) ? PLAYER2 : PLAYER1;
        }
        
#ifdef HAS_GRAPHICS
        if (use_graphics) {
            run_graphics_game(mode, ai_player, ai_level);
            continue;
        }
#endif
        
        // Console mode
        Game game;
        game_init(&game, mode, PLAYER1, ai_player, ai_level);
        game_run(&game);
        game_cleanup(&game);
        
        printf("\nPlay again? (y/n): ");
        char buffer[64];
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        char *p = buffer;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p != 'y' && *p != 'Y') {
            printf("Thanks for playing!\n");
            break;
        }
    }
    
    return 0;
}
