#include "utest.h"
#include "../include/board.h"

// Test board initialization clears all cells
UTEST(board, init) {
	Board b;
	board_init(&b);
	ASSERT_EQ(board_is_full(&b), 0);
	ASSERT_EQ(b.cells[0][0], EMPTY);
	ASSERT_EQ(b.cells[ROWS - 1][COLS - 1], EMPTY);
}

// Test dropping a piece returns the bottom row and sets the cell
UTEST(board, drop_piece_basic) {
	Board b;
	board_init(&b);
	int row = board_drop_piece(&b, 3, PLAYER1);
	ASSERT_EQ(row, ROWS - 1);
	ASSERT_EQ(b.cells[row][3], PLAYER1);
}

// Test invalid column indices are rejected
UTEST(board, drop_piece_invalid_column) {
	Board b;
	board_init(&b);
	ASSERT_EQ(board_drop_piece(&b, -1, PLAYER1), -1);
	ASSERT_EQ(board_drop_piece(&b, COLS, PLAYER1), -1);
}

// Test filling a column and related validations
UTEST(board, column_full_and_is_valid_move) {
	Board b;
	board_init(&b);
	for (int i = 0; i < ROWS; ++i) {
		int r = board_drop_piece(&b, 2, PLAYER2);
		ASSERT_NE(r, -1);
	}
	ASSERT_EQ(board_is_valid_move(&b, 2), 0);
	ASSERT_EQ(board_drop_piece(&b, 2, PLAYER1), -1);
}

// Horizontal win detection
UTEST(board, horizontal_win) {
	Board b;
	board_init(&b);
	// place PLAYER1 across bottom row in cols 0..3
	for (int c = 0; c <= 3; ++c) {
		board_drop_piece(&b, c, PLAYER1);
	}
	ASSERT_EQ(board_check_winner(&b, PLAYER1), 1);
}

// Vertical win detection
UTEST(board, vertical_win) {
	Board b;
	board_init(&b);
	// stack PLAYER2 four high in column 0
	for (int i = 0; i < 4; ++i) {
		board_drop_piece(&b, 0, PLAYER2);
	}
	ASSERT_EQ(board_check_winner(&b, PLAYER2), 1);
}

// Diagonal (down-right) win detection via direct cell setup
UTEST(board, diagonal_down_right_win) {
	Board b;
	board_init(&b);
	// create diagonal starting at (2,0) -> (5,3)
	b.cells[2][0] = PLAYER1;
	b.cells[3][1] = PLAYER1;
	b.cells[4][2] = PLAYER1;
	b.cells[5][3] = PLAYER1;
	ASSERT_EQ(board_check_winner(&b, PLAYER1), 1);
}

// Diagonal (down-left) win detection via direct cell setup
UTEST(board, diagonal_down_left_win) {
	Board b;
	board_init(&b);
	// create diagonal starting at (2,3) -> (5,0)
	b.cells[2][3] = PLAYER2;
	b.cells[3][2] = PLAYER2;
	b.cells[4][1] = PLAYER2;
	b.cells[5][0] = PLAYER2;
	ASSERT_EQ(board_check_winner(&b, PLAYER2), 1);
}


