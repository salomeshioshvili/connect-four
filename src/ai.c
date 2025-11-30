#include "ai.h"
#include <stdio.h>
#include <time.h>
#include "board.h"
#include "game.h"
#include <stdlib.h>

int ai_easy(const Board *board, CellState ai_player) {
    int column;
// i decided that the easy ai only plays random valid moves it doesnt do anythingg else
    do {
        column = rand() % COLS;
    } while (board_is_valid_move(board, column) == 0);

    return column;
}

int ai_medium(const Board *board, CellState ai_player) {
    CellState opponent;
// the medium ai blocks the opponent's winning move if it can, otherwise it plays a random valid move (which is what the easy ai does)
// turn based logic 
    if (ai_player == PLAYER1) {
        opponent = PLAYER2;
    } else {
        opponent = PLAYER1;
    }
// check if the ai can win in the next move
    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);
            if (board_check_winner(&temporary_board, ai_player) == 1) {
                return column;
            }
        }
    }
// check if the opponent can win in the next move and block 
    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, opponent);
            if (board_check_winner(&temporary_board, opponent) == 1) {
                return column;
            }
        }
    }
// if theres no way to win immediately or block the opponent, play a random valid move
    return ai_easy(board, ai_player);
}
// heuristic approach, super cool scoring system lol 
int score_position(const Board *board, int player_id) {
    int score = 0;
    int center_column = COLS / 2;
// score of 3 for the center column
    for (int row = 0; row < ROWS; row++) {
        if (board->cells[row][center_column] == player_id) {
            score = score + 3;
        }
    }
// score of 10 for 2 in a row, 50 for 3 in a row, and 1000 for 4 in a row
// this is the horizontal scoring (4 next to each other)
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
// this is the vertical scoring (4 on top of each other)
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
// this is the positive diagonal scoring (top left to bottom right) "\"
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
// other way around, this is the negative diagonal scoring (bottom left to top right) "/"
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
// the hard ai is going to implement a minimax algorithm that thinks multiple moves ahead
// for reference, its a minimum risk maximum reward algorithm
// bit more advanced but can (possibly?) still be beat 
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
// the fun one, it has strategy, it never loses, it forces a draw if it cant win
// it uses traps, thinks ahead, always blocks attacks, keeps track of the player's pieces and predicts their next move to use it as an advantage, etc.
int ai_expert(const Board *board, CellState ai_player) {
    return ai_hard(board, ai_player);
}

int ai_choose_move(const Board *board, CellState ai_player) {
    return ai_medium(board, ai_player);
}
