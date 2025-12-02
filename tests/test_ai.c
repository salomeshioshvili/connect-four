#include "utest.h"
#include "ai.h"
#include "board.h" 
#include <stdlib.h>

UTEST(ai, valid_moves) {
    srand(0); // for reproducible results since the ai uses randomness

    Board board;
    board_init(&board);
    CellState ai_player = PLAYER1;

    for (int i = 0; i < 10; i++) {
        int column = ai_easy(&board, ai_player);
        ASSERT_TRUE(column >= 0);
        ASSERT_TRUE(column < COLS);
        ASSERT_TRUE(board_is_valid_move(&board, column) == 1);
    }
}

UTEST(ai, win) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER1;

    board_drop_piece(&board, 0, ai_player);
    board_drop_piece(&board, 1, ai_player);
    board_drop_piece(&board, 2, ai_player);

    int column = ai_medium(&board, ai_player);

    ASSERT_EQ(column, 3);
}

UTEST(ai, block) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER2;
    CellState opponent = PLAYER1;

    board_drop_piece(&board, 0, opponent);
    board_drop_piece(&board, 1, opponent);
    board_drop_piece(&board, 2, opponent);

    int column = ai_medium(&board, ai_player);

    ASSERT_EQ(column, 3);
}

UTEST(ai, center) {
    Board empty_board;
    Board center_board;
    Board edge_board;

    board_init(&empty_board);
    board_init(&center_board);
    board_init(&edge_board);

    int score_empty = score_position(&empty_board, PLAYER1);

    board_drop_piece(&center_board, COLS / 2, PLAYER1);
    int score_center = score_position(&center_board, PLAYER1);

    board_drop_piece(&edge_board, 0, PLAYER1);
    int score_edge = score_position(&edge_board, PLAYER1);

    ASSERT_TRUE(score_center > score_empty);
    ASSERT_TRUE(score_center > score_edge);
}

UTEST(ai, hard) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER1;

    int column = ai_hard(&board, ai_player);

    ASSERT_TRUE(column >= 0);
    ASSERT_TRUE(column < COLS);
    ASSERT_TRUE(board_is_valid_move(&board, column) == 1);
}

UTEST(ai, expert_win) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER1;

    board_drop_piece(&board, 0, ai_player);
    board_drop_piece(&board, 1, ai_player);
    board_drop_piece(&board, 2, ai_player);

    int column = ai_expert(&board, ai_player);

    ASSERT_EQ(column, 3);
}

UTEST(ai, expert_block) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER2;
    CellState opponent = PLAYER1;

    board_drop_piece(&board, 0, opponent);
    board_drop_piece(&board, 1, opponent);
    board_drop_piece(&board, 2, opponent);

    int column = ai_expert(&board, ai_player);

    ASSERT_EQ(column, 3);
}

UTEST(ai, thread) {
    Board board;
    board_init(&board);
    CellState ai_player = PLAYER1;

    AIThread task_hard;
    task_hard.board_copy = board;
    task_hard.ai_player = ai_player;
    task_hard.ai_level = AI_HARD;
    task_hard.result = -1;

    ai_thread_function(&task_hard);

    ASSERT_TRUE(task_hard.result >= 0);
    ASSERT_TRUE(task_hard.result < COLS);
    ASSERT_TRUE(board_is_valid_move(&board, task_hard.result) == 1);

    AIThread task_expert;
    task_expert.board_copy = board;
    task_expert.ai_player = ai_player;
    task_expert.ai_level = AI_EXPERT;
    task_expert.result = -1;

    ai_thread_function(&task_expert);

    ASSERT_TRUE(task_expert.result >= 0);
    ASSERT_TRUE(task_expert.result < COLS);
    ASSERT_TRUE(board_is_valid_move(&board, task_expert.result) == 1);
}

UTEST_MAIN()
