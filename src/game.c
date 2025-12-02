#include "game.h"
#include "ai.h"
#include "board.h"
#include "history.h"
#include "io.h"
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

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

//Execute one AI move.
//Returns 1 on success and 0 if a bug happened (should not happen, but just in case, for debugging purposes)
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

//does one player move, but keep in mind that if it plays against another person, i disabled undo so no one copmplains about "unfairness"
static int do_human_move(Game *game, int allow_undo) {
    while (1) {
        int res = prompt_human_move(allow_undo);

        if (res == -1) {
            game->is_over = 1;
            return 0;
        } else if (res == -2 && allow_undo) {
            // Undo both AI's move and player's last move
            CellState undone_player;
            int count = 0;
            
            // Undo AI's last move
            if (history_undo(&game->board, &game->history, &undone_player)) {
                count++;
            }
            // Undo player's last move
            if (history_undo(&game->board, &game->history, &undone_player)) {
                count++;
                game->current_player = undone_player;
            }
            
            if (count > 0) {
                printf("%d move(s) undone.\n", count);
            } else {
                printf("No moves to undo.\n");
            }
            clear_screen();
            board_print(&game->board);
            continue;
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
            return 1;
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
            game->winner = EMPTY;
            game->is_draw = 0;
            break;
        }

        if (board_check_winner(&game->board, game->current_player)) {
            game->is_over = 1;
            game->winner = game->current_player;
            game->is_draw = 0;
        } else if (board_check_draw(&game->board)) {
            game->is_over = 1;
            game->winner = EMPTY;
            game->is_draw = 1;
        } else {
            switch_player(game);
        }
    }

    // Final screen
    clear_screen();
    board_print(&game->board);

    printf("\n");
    if (game->winner == PLAYER1) {
        printf("*** PLAYER 1 (X) WINS! ***\n");
    } else if (game->winner == PLAYER2) {
        printf("*** PLAYER 2 (O) WINS! ***\n");
    } else if (game->is_draw) {
        printf("*** IT'S A DRAW! ***\n");
    } else {
        printf("Game ended.\n");
    }
    printf("\n");
}