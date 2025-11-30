#include "ai.h"
#include "board.h"
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
// this is whats going to be used for the minimax algorithm
// first evalutate the board's current state and give it a score
static int evaluate_board(const Board *board, CellState ai_player) {
    CellState opponent;
    int ai_score;
    int opponent_score;

    if (ai_player == PLAYER1) {
        opponent = PLAYER2;
    } else {
        opponent = PLAYER1;
    }

    ai_score = score_position(board, ai_player);
    opponent_score = score_position(board, opponent);

    return ai_score - opponent_score;
}
// counts how many immediate winning moves are available for a player, this will be used for the expert ai
static int count_immediate_wins(const Board *board, CellState player) {
    int wins = 0;

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, player);
            if (board_check_winner(&temporary_board, player) == 1) {
                wins = wins + 1;
            }
        }
    }
    return wins;
}
// the hard ai is going to implement the minimax algorithm that thinks multiple moves ahead
// for reference, its a minimum risk maximum reward algorithm
// bit more advanced but can (possibly?) still be beat 
int ai_hard(const Board *board, CellState ai_player) {
    int best_column = -1;
    int best_score = 0;
    int best_score_set = 0;

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);
            int current_score = evaluate_board(&temporary_board, ai_player);

            if (best_score_set == 0 || current_score > best_score) {
                best_score = current_score;
                best_score_set = 1;
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
    CellState opponent;
    int best_column = -1;
    int best_score = 0;
    int best_score_set = 0;

    if (ai_player == PLAYER1) {
        opponent = PLAYER2;
    } else {
        opponent = PLAYER1;
    }
// added the same code that was in medium, if the ai can win immediately, win. if the opponent can win immediately, block.
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
    // minimax! evaluating all the potential worst case scenarios and choosing the best worst case
    // also it checks for immediate winning traps using the function made earlier 
    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);
            int ai_future_wins = count_immediate_wins(&temporary_board, ai_player);
            if (ai_future_wins >= 2) {
                return column;
            }
        }
    }

    for (int column = 0; column < COLS; column++) {
        if (board_is_valid_move(board, column) == 1) {
            Board temporary_board = *board;
            board_drop_piece(&temporary_board, column, ai_player);

            int worst_score_for_ai = 0;
            int worst_score_set = 0;
            
            for (int opponent_column = 0; opponent_column < COLS; opponent_column++) {
                if (board_is_valid_move(&temporary_board, opponent_column) == 1) {
                    Board opponent_board = temporary_board;
                    board_drop_piece(&opponent_board, opponent_column, opponent);
                    int position_score = evaluate_board(&opponent_board, ai_player);
// check for opponent traps
                    int opponent_future_wins = count_immediate_wins(&opponent_board, opponent);
                    if (opponent_future_wins >= 2) {
                        position_score = position_score - 100000; // dramatic penalty for allowing the opponent to trap the ai
                    }

                    if (worst_score_set == 0 || position_score < worst_score_for_ai) {
                        worst_score_for_ai = position_score;
                        worst_score_set = 1;
                    }
                }
            }

            if (worst_score_set == 0) {
                worst_score_for_ai = evaluate_board(&temporary_board, ai_player);
            }

            if (best_score_set == 0 || worst_score_for_ai > best_score) {
                best_score = worst_score_for_ai;
                best_score_set = 1;
                best_column = column;
            }
        }
    }

    if (best_column == -1) {
        return ai_medium(board, ai_player);
    }

    return best_column;
}

void *ai_thread_function(void *arg) {
    AIThread *task = (AIThread *)arg;

    if (task->ai_level == AI_HARD) {
        task->result = ai_hard(&task->board_copy, task->ai_player);
    } else {
        task->result = ai_expert(&task->board_copy, task->ai_player);
    }

    return NULL;
}

int ai_choose_move(const Board *board, CellState ai_player) {
    return ai_medium(board, ai_player);
}
