#ifndef OTHELLO_H_
#define OTHELLO_H_
#include "main.h"

typedef uint8_t bool;

#define BOARD_POS_COL_START 6
#define BOARD_POS_COL_END 13
#define BOARD_POS_ROW_START 0
#define BOARD_POS_ROW_END 3

#define LEFT_INFO_POS_COL_START 0
#define LEFT_INFO_POS_COL_END 4
#define RIGHT_INFO_POS_COL_START 15
#define RIGHT_INFO_POS_COL_END 19
#define INFO_POS_ROW_START 0
#define INFO_POS_ROW_END 3

#define GAME_RUNNING 0
#define GAME_ENDED 1
#define GAME_HALTED 2

#define WHITE 1
#define BLACK 2
#define EMPTY 0
#define SQR_COUNT 64
#define BCOLS 8
#define BROWS 8
#define BROW 0
#define BCOL 1
#define BSW 2
#define TRUE 1
#define FALSE 0

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define UP_RIGHT 4
#define UP_LEFT 5
#define DOWN_RIGHT 6
#define DOWN_LEFT 7
#define DIR_COUNT 8

#define A_SQR 0
#define B_SQR 1
#define C_SQR 2
#define D_SQR 3
#define E_SQR 4
#define F_SQR 5
#define G_SQR 6
#define H_SQR 7
#define SQR_1 0
#define SQR_2 1
#define SQR_3 2
#define SQR_4 3
#define SQR_5 4
#define SQR_6 5
#define SQR_7 6
#define SQR_8 7
#define SQR_SELECTED 0
#define SQR_DEFAULT 1
#define B_NEW_GAME 2
#define B_END_GAME 3
#define WIN 0
#define TIMEOUT 1

#define BOARD_UP(row, col) board[row - 1][col]
#define BOARD_DOWN(row, col) board[row + 1][col]
#define BOARD_RIGHT(row, col) board[row][col + 1]
#define BOARD_LEFT(row, col) board[row][col - 1]
#define BOARD_UP_RIGHT(row, col) board[row - 1][col + 1]
#define BOARD_UP_LEFT(row, col) board[row - 1][col - 1]
#define BOARD_DOWN_RIGHT(row, col) board[row + 1][col + 1]
#define BOARD_DOWN_LEFT(row, col) board[row + 1][col - 1]

#define IS_UP_IN_RANGE(row, col) ((row - 1) < BROWS && col < BCOLS && row >= 1 && col >= 0)
#define IS_DOWN_IN_RANGE(row, col) ((row + 1) < BROWS && col < BCOLS && (row + 1) >= 0 && col >= 0)
#define IS_RIGHT_IN_RANGE(row, col) (row < BROWS && (col + 1) < BCOLS && row >= 0 && (col + 1) >= 0)
#define IS_LEFT_IN_RANGE(row, col) (row < BROWS && (col - 1) < BCOLS && row >= 0 && (col) >= 1)
#define IS_UP_RIGHT_IN_RANGE(row, col) ((row - 1) < BROWS && (col + 1) < BCOLS && (row) >= 1 && (col + 1) >= 0)
#define IS_UP_LEFT_IN_RANGE(row, col) ((row - 1) < BROWS && (col - 1) < BCOLS && (row) >= 1 && (col) >= 1)
#define IS_DOWN_RIGHT_IN_RANGE(row, col) ((row + 1) < BROWS && (col + 1) < BCOLS && (row + 1) >= 0 && (col + 1) >= 0)
#define IS_DOWN_LEFT_IN_RANGE(row, col) ((row + 1) < BROWS && (col - 1) < BCOLS && (row + 1) >= 0 && (col) >= 1)
#define IS_EMPTY(row, col) (board[row][col] == EMPTY)

#define ADD_TO_APPLICABLE_DIRECTIONS(dir) applicable_directions[applicable_directions[0]++] = dir
#define RESET_APPLICABLE_DIRECTIONS() applicable_directions[0] = 1
#define APPLICABLE_DIRECTIONS_COUNT applicable_directions[0]


bool has_legal_move(uint8_t player);
bool is_valid_in_direction(uint8_t player, uint8_t pos_row, uint8_t pos_col, uint8_t direction);
bool is_valid_move(uint8_t player, uint8_t pos_row, uint8_t pos_col);
void apply_move(uint8_t player, uint8_t pos_row, uint8_t pos_col);
void apply_move_in_direction(uint8_t player, uint8_t pointer_row, uint8_t pointer_col, uint8_t direction);
void handle_logic();
void initialize_board();
void print_board();
void print_game_info();
#endif // OTHELLO_H_
