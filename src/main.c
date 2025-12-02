#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "game.h"
#include "board.h"
#include "ai.h"
#include "io.h"

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
