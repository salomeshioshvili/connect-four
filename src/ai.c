#include "ai.h"
#include <stdio.h>
#include <time.h>
#include "board.h"
#include "game.h"
#include <stdlib.h>

int ai_easy(const Board *board, CellState ai_player) {
    int column;

    do {
        column = rand() % COLS;
    } while (board_is_valid_move(board, column) == 0);

    return column;
}

int ai_medium(const Board *board, CellState ai_player) {
    CellState opponent;

    if (ai_player == PLAYER1) {
        opponent = PLAYER2;
    } else {
        opponent = PLAYER1;
    }

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);
            if (board_check_winner(&temporary_board, ai_player) == 1) {
                return column;
            }
        }
    }

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, opponent);
            if (board_check_winner(&temporary_board, opponent) == 1) {
                return column;
            }
        }
    }

    return ai_easy(board, ai_player);
}

int score_position(const Board *board, int player_id) {
    int score = 0;
    int center_column = COLS / 2;

    for (int row = 0; row < ROWS; row++) {
        if (board->cells[row][center_column] == player_id) {
            score = score + 3;
        }
    }

    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column <= COLS - 4; column++) {
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (board->cells[row][column + i] == player_id) {
                    count = count + 1;
                }
            }
            if (count == 2) {
                score = score + 10;
            }
            if (count == 3) {
                score = score + 50;
            }
            if (count == 4) {
                score = score + 1000;
            }
        }
    }

    for (int column = 0; column < COLS; column++) {
        for (int row = 0; row <= ROWS - 4; row++) {
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (board->cells[row + i][column] == player_id) {
                    count = count + 1;
                }
            }
            if (count == 2) {
                score = score + 10;
            }
            if (count == 3) {
                score = score + 50;
            }
            if (count == 4) {
                score = score + 1000;
            }
        }
    }

    for (int row = 0; row <= ROWS - 4; row++) {
        for (int column = 0; column <= COLS - 4; column++) {
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (board->cells[row + i][column + i] == player_id) {
                    count = count + 1;
                }
            }
            if (count == 2) {
                score = score + 10;
            }
            if (count == 3) {
                score = score + 50;
            }
            if (count == 4) {
                score = score + 1000;
            }
        }
    }

    for (int row = 3; row < ROWS; row++) {
        for (int column = 0; column <= COLS - 4; column++) {
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (board->cells[row - i][column + i] == player_id) {
                    count = count + 1;
                }
            }
            if (count == 2) {
                score = score + 10;
            }
            if (count == 3) {
                score = score + 50;
            }
            if (count == 4) {
                score = score + 1000;
            }
        }
    }

    return score;
}

int ai_hard(const Board *board, CellState ai_player) {
    int best_column = -1;
    int best_score = 0;

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);
            int current_score = score_position(&temporary_board, ai_player);

            if (best_column == -1 || current_score > best_score) {
                best_score = current_score;
                best_column = column;
            }
        }
    }

    if (best_column == -1) {
        return ai_easy(board, ai_player);
    }

    return best_column;
}

int ai_expert(const Board *board, CellState ai_player) {
    return ai_hard(board, ai_player);
}

int ai_choose_move(const Board *board, CellState ai_player) {
    return ai_medium(board, ai_player);
}
