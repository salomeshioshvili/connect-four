#include "utest.h"
#include "game.h"
#include "board.h"
#include "history.h"

UTEST(game, setfields) {
    Game game;

    game_init(&game, GAME_MODE_PVP, PLAYER1, PLAYER2, AI_HARD);

    ASSERT_EQ(game.mode, GAME_MODE_PVP);
    ASSERT_EQ(game.current_player, PLAYER1);
    ASSERT_EQ(game.ai_player, EMPTY);
    ASSERT_EQ(game.ai_level, AI_EASY);

    ASSERT_TRUE(game.history == NULL);
    ASSERT_EQ(game.is_over, 0);
    ASSERT_EQ(game.winner, EMPTY);
    ASSERT_EQ(game.is_draw, 0);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            ASSERT_EQ(game.board.cells[row][col], EMPTY);
        }
    }

    game_cleanup(&game);
}

UTEST(game, setaifields) {
    Game game;

    game_init(&game, GAME_MODE_PVAI, PLAYER2, PLAYER1, AI_EXPERT);

    ASSERT_EQ(game.mode, GAME_MODE_PVAI);
    ASSERT_EQ(game.current_player, PLAYER2);
    ASSERT_EQ(game.ai_player, PLAYER1);
    ASSERT_EQ(game.ai_level, AI_EXPERT);

    ASSERT_TRUE(game.history == NULL);
    ASSERT_EQ(game.is_over, 0);
    ASSERT_EQ(game.winner, EMPTY);
    ASSERT_EQ(game.is_draw, 0);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            ASSERT_EQ(game.board.cells[row][col], EMPTY);
        }
    }

    game_cleanup(&game);
}

UTEST(game, cleanup) {
    Game game;

    game_init(&game, GAME_MODE_PVP, PLAYER1, EMPTY, AI_EASY);

    history_add_move(&game.history, 0, 0, PLAYER1);
    history_add_move(&game.history, 1, 1, PLAYER2);
    history_add_move(&game.history, 2, 2, PLAYER1);

    ASSERT_TRUE(game.history != NULL);

    game_cleanup(&game);

    ASSERT_TRUE(game.history == NULL);
}

UTEST(game, cleanuptwice) {
    Game game;

    game_init(&game, GAME_MODE_PVP, PLAYER1, EMPTY, AI_EASY);

    history_add_move(&game.history, 0, 0, PLAYER1);

    game_cleanup(&game);
    game_cleanup(&game);

    ASSERT_TRUE(game.history == NULL);
}
