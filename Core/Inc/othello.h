#ifndef OTHELLO_H_
#define OTHELLO_H_
#include "main.h"
typedef uint8_t bool;

#define WHITE 1
#define BLACK 2
#define EMPTY 0
#define SQR_COUNT 64
#define BCOLS 8
#define BROWS 8
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

#define BOARD_UP(row, col) board[row - 1][col]
#define BOARD_DOWN(row, col) board[row + 1][col]
#define BOARD_RIGHT(row, col) board[row][col + 1]
#define BOARD_LEFT(row, col) board[row][col - 1]
#define BOARD_UP_RIGHT(row, col) board[row - 1][col + 1]
#define BOARD_UP_LEFT(row, col) board[row - 1][col - 1]
#define BOARD_DOWN_RIGHT(row, col) board[row + 1][col + 1]
#define BOARD_DOWN_LEFT(row, col) board[row + 1][col - 1]

#define IS_UP_IN_RANGE(row, col) ((row - 1) < BROWS && col < BCOLS)
#define IS_DOWN_IN_RANGE(row, col) ((row + 1) < BROWS && col < BCOLS)
#define IS_RIGHT_IN_RANGE(row, col) (row < BROWS && (col + 1) < BCOLS)
#define IS_LEFT_IN_RANGE(row, col) (row < BROWS && (col - 1) < BCOLS)
#define IS_UP_RIGHT_IN_RANGE(row, col) ((row - 1) < BROWS && (col + 1) < BCOLS)
#define IS_UP_LEFT_IN_RANGE(row, col) ((row - 1) < BROWS && (col - 1) < BCOLS)
#define IS_DOWN_RIGHT_IN_RANGE(row, col) ((row + 1) < BROWS && (col + 1) < BCOLS)
#define IS_DOWN_LEFT_IN_RANGE(row, col) ((row + 1) < BROWS && (col - 1) < BCOLS)
#define IS_EMPTY(row, col) (board[row][col] == EMPTY)

#define ADD_TO_APPLICABLE_DIRECTIONS(dir) applicable_directions[applicable_directions[0]++] = dir
#define RESET_APPLICABLE_DIRECTIONS() applicable_directions[0] = 1
#define APPLICABLE_DIRECTIONS_COUNT applicable_directions[0]


void initialize_board();
bool is_valid_in_direction(uint8_t player, uint8_t pos_row, uint8_t pos_col, uint8_t direction);
bool is_valid_move(uint8_t player, uint8_t pos_row, uint8_t pos_col);
void apply_move_in_direction(uint8_t player, uint8_t pointer_row, uint8_t pointer_col, uint8_t direction);
void apply_move(uint8_t player, uint8_t pos_row, uint8_t pos_col);
void print_board();
bool has_legal_move(uint8_t player);
#endif // OTHELLO_H_
