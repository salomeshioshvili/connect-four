#include "game.h"
#include "ai.h"
#include "board.h"
#include "history.h"
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

//clear the screen, should work, but if odesn't I'll try to do something else, chat gave this to me
static void clear_screen(void) {
    printf("\033[2J\033[H");
}

static const char *player_name(CellState p) {
    switch (p) {
        case PLAYER1: return "Player 1 (X)";
        case PLAYER2: return "Player 2 (O)";
        default:      return "Unknown";
    }
}

/**
 * Prompt a player for a move:
 *  - enter column 0..COLS-1
 *  - 'q' to quit
 *  - 'u' to undo (only if playing against ai)
 *
 * Returns:
 *   0..COLS-1 -> chosen column
 *   -1        -> quit
 *   -2        -> undo
 */
static int prompt_human_move(int allow_undo) {
    char buffer[64];

    while (1) {
        if (allow_undo) {
            printf("Enter column (0-%d), 'u' to undo, or 'q' to quit: ", COLS - 1);
        } else {
            printf("Enter column (0-%d), or 'q' to quit: ", COLS - 1);
        }

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            // EOF -> treat as quit can also implement errno here to make raul happy
            return -1;
        }

        char *p = buffer;//give p the fgets thing from the if
        while (*p && isspace((unsigned char)*p)) {
            p++;
        }
        //check the actual input
        if (*p == 'q' || *p == 'Q') {
            return -1;
        }
        if (allow_undo && (*p == 'u' || *p == 'U')) {
            return -2;
        }

        int col;
        if (sscanf(p, "%d", &col) == 1) {
            if (col >= 0 && col < COLS) {
                return col;
            }
        }

        printf("Invalid input. Please try again.\n");
    }
}

//GAME

void game_init(Game *game, GameMode mode, CellState starting_player, CellState ai_player, AILevel ai_level) {
    if (!game) return;

    board_init(&game->board);

    game->current_player = starting_player;
    game->mode = mode;
    game->ai_player = (mode == GAME_MODE_PVAI) ? ai_player : EMPTY;
    game->ai_level = (mode == GAME_MODE_PVAI) ? ai_level : AI_EASY;
    game->history = NULL;
    game->is_over = 0;
    game->winner = EMPTY;
    game->is_draw = 0;
}

void game_cleanup(Game *game) {
    if (!game) return;
    history_free(&game->history);
}

// Switch current player
static void switch_player(Game *game) {
    if (game->current_player == PLAYER1) {
        game->current_player = PLAYER2;
    } else {
        game->current_player = PLAYER1;
    }
}

/**
 * Execute one AI move.
 * Returns 1 on success, 0 if something went badly wrong (should not happen, but just in case).
 */
static int do_ai_move(Game *game) {
    int col;

    if (game->ai_level == AI_EASY) {
        col = ai_easy(&game->board, game->current_player);
    } else if (game->ai_level == AI_MEDIUM) {
        col = ai_medium(&game->board, game->current_player);
    } else {
        AIThread task;
        pthread_t thread;

        task.board_copy = game->board;
        task.ai_player = game->current_player;
        task.ai_level = game->ai_level;
        task.result = -1;

        if (pthread_create(&thread, NULL, ai_thread_function, &task) != 0) {
            fprintf(stderr, "Failed to create the AI thread. Falling back to medium AI.\n");
            col = ai_medium(&game->board, game->current_player);
        } else {
            pthread_join(thread, NULL);
            col = task.result;
        }
    }

    if (col < 0 || col >= COLS) {
        fprintf(stderr, "AI chose invalid column %d.\n", col);
        return 0;
    }

    if (!board_is_valid_move(&game->board, col)) {
        fprintf(stderr, "AI chose a full/invalid column %d.\n", col);
        return 0;
    }

    int row = board_drop_piece(&game->board, col, game->current_player);
    if (row < 0) {
        fprintf(stderr, "AI move failed when dropping piece in column %d.\n", col);
        return 0;
    }

    history_add_move(&game->history, row, col, game->current_player);
    printf("%s (AI) plays column %d\n", player_name(game->current_player), col);
    return 1;
}

/**
 * Execute one player move:
 *  - ask for input
 *  - support quit always
 *  - support undo only if playing ai (can even romove feature if y'all don't like it)
 *
 * Returns:
 *   1 -> turn finished with a new piece
 *   0 -> player chose to quit
 */
static int do_human_move(Game *game, int allow_undo) {
    while (1) {
        int res = prompt_human_move(allow_undo);

        if (res == -1) {
            // quit
            game->is_over = 1;
            return 0;
        } else if (res == -2 && allow_undo) {
            // undo last move
            CellState dummy_player;  // we ignore this; we keep game->current_player as-is
            if (!history_undo(&game->board, &game->history, &dummy_player)) {
                printf("No moves to undo.\n");
            } else {
                printf("Last move undone.\n");
            }
            clear_screen();
            board_print(&game->board);
            continue; // still same player's turn, no new piece placed yet
        } else {
            int col = res;

            if (!board_is_valid_move(&game->board, col)) {
                printf("Invalid move: column %d is full or out of range.\n", col);
                continue;
            }

            int row = board_drop_piece(&game->board, col, game->current_player);
            if (row < 0) {
                printf("Error dropping piece in column %d.\n", col);
                continue;
            }

            history_add_move(&game->history, row, col, game->current_player);
            return 1; // successful move
        }
    }
}

void game_run(Game *game) {
    if (!game) return;

    while (!game->is_over) {
        clear_screen();
        board_print(&game->board);
        printf("Current turn: %s\n\n", player_name(game->current_player));

        int move_ok = 0;
        int is_ai_turn = (game->mode == GAME_MODE_PVAI &&
                          game->current_player == game->ai_player);

        if (is_ai_turn) {
            move_ok = do_ai_move(game);
        } else {
            int allow_undo = (game->mode == GAME_MODE_PVAI);
            move_ok = do_human_move(game, allow_undo);
        }

        if (!move_ok) {
            // someone quit or AI failure -> end with no winner
            game->winner = EMPTY;
            game->is_draw = 0;
            break;
        }

        // After a successful move, check for winner or draw
        if (board_check_winner(&game->board, game->current_player)) {
            game->is_over = 1;
            game->winner = game->current_player;
            game->is_draw = 0;
        } else if (board_check_draw(&game->board)) {
            game->is_over = 1;
            game->winner = EMPTY;
            game->is_draw = 1;
        } else {
            //Keep playing, it's not over
            switch_player(game);
        }
    }

    // Final screen
    clear_screen();
    board_print(&game->board);

    if (game->winner == PLAYER1 || game->winner == PLAYER2) {
        printf("%s wins!\n", player_name(game->winner));
    } else if (game->is_draw) {
        printf("It's a draw! Board is full, no winner.\n");
    } else {
        printf("Game ended without a winner.\n");
    }
}
