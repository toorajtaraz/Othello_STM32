#include "othello.h"
#include "LiquidCrystal.h"

uint8_t board[BROWS][BCOLS] = {0};
uint8_t directions[DIR_COUNT] = {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT
};
uint8_t applicable_directions[DIR_COUNT + 1]= {1};

void initialize_board() {
    // Every sqr is empty,
    for(uint8_t i = 0; i < BROWS; i++) {
        for(uint8_t j = 0; j < BCOLS; j++) {
            board[i][j] = EMPTY;
        }
    }
    // except these four:
    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

bool is_valid_in_direction(uint8_t player, uint8_t pos_row, uint8_t pos_col, uint8_t direction) {
    uint8_t opponent;
    if(player == WHITE) {
        opponent = BLACK;
    } else {
        opponent = WHITE;
    }

    uint8_t pointer_row = pos_row;
    uint8_t pointer_col = pos_col;

    switch(direction) {
        case UP: {
            while(IS_UP_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP(pointer_row, pointer_col) == opponent) {
                    pointer_row--;
                } else if(BOARD_UP(pointer_row, pointer_col) == player && pointer_row != pos_row) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(UP);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case DOWN: {
            while(IS_DOWN_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN(pointer_row, pointer_col) == opponent) {
                    pointer_row++;
                } else if(BOARD_DOWN(pointer_row, pointer_col) == player && pointer_row != pos_row) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(DOWN);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case RIGHT: {
            while(IS_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_RIGHT(pointer_row, pointer_col) == opponent) {
                    pointer_col++;
                } else if(BOARD_RIGHT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(RIGHT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case LEFT: {
            while(IS_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_LEFT(pointer_row, pointer_col) == opponent) {
                    pointer_col--;
                } else if(BOARD_LEFT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(LEFT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case UP_RIGHT: {
            while(IS_UP_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP_RIGHT(pointer_row, pointer_col) == opponent) {
                    pointer_col++;
                    pointer_row--;
                } else if(BOARD_UP_RIGHT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(UP_RIGHT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case UP_LEFT: {
            while(IS_UP_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP_LEFT(pointer_row, pointer_col) == opponent) {
                    pointer_col--;
                    pointer_row--;
                } else if(BOARD_UP_LEFT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(UP_LEFT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case DOWN_RIGHT: {
            while(IS_DOWN_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN_RIGHT(pointer_row, pointer_col) == opponent) {
                    pointer_col++;
                    pointer_row++;
                } else if(BOARD_DOWN_RIGHT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(DOWN_RIGHT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
        case DOWN_LEFT: {
            while(IS_DOWN_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN_LEFT(pointer_row, pointer_col) == opponent) {
                    pointer_col--;
                    pointer_row++;
                } else if(BOARD_DOWN_LEFT(pointer_row, pointer_col) == player && pointer_col != pos_col) { // Condition for being sandwiched => * .. o .. *
                    ADD_TO_APPLICABLE_DIRECTIONS(DOWN_LEFT);
                    return TRUE;
                } else {
                    return FALSE;
                }
            }
        };
            break;
    }

    return FALSE;
}

bool is_valid_move(uint8_t player, uint8_t pos_row, uint8_t pos_col) {
    // That sqr must be empty
    if(! IS_EMPTY(pos_row, pos_col)) {
        return FALSE;
    }

    bool ret = FALSE;
    for(uint8_t i = 0; i < DIR_COUNT; i++) {
        if(is_valid_in_direction(player, pos_row, pos_col, directions[i])) {
            ret = TRUE;
        }
    }

    return ret;
}

void apply_move_in_direction(uint8_t player, uint8_t pointer_row, uint8_t pointer_col, uint8_t direction) {
    switch(direction) {
        case UP: {
            while(IS_UP_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_UP(pointer_row, pointer_col) = player;
                    pointer_row--;
                }
            }
        };
            break;
        case DOWN: {
            while(IS_DOWN_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_DOWN(pointer_row, pointer_col) = player;
                    pointer_row++;
                }
            }
        };
            break;
        case RIGHT: {
            while(IS_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_RIGHT(pointer_row, pointer_col) == player) {
                   break;
                } else {
                    BOARD_RIGHT(pointer_row, pointer_col) = player;
                    pointer_col++;
                }
            }
        };
            break;
        case LEFT: {
            while(IS_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_LEFT(pointer_row, pointer_col) == player) {
                   break;
                } else {
                    BOARD_LEFT(pointer_row, pointer_col) = player;
                    pointer_col--;
                }

            }
        };
            break;
        case UP_RIGHT: {
            while(IS_UP_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP_RIGHT(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_UP_RIGHT(pointer_row, pointer_col) = player;
                    pointer_col++;
                    pointer_row--;
                }
            }
        };
            break;
        case UP_LEFT: {
            while(IS_UP_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_UP_LEFT(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_UP_LEFT(pointer_row, pointer_col) = player;
                    pointer_col--;
                    pointer_row--;
                }
            }
        };
            break;
        case DOWN_RIGHT: {
            while(IS_DOWN_RIGHT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN_RIGHT(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_DOWN_RIGHT(pointer_row, pointer_col) = player;
                    pointer_col++;
                    pointer_row++;
                }
            }
        };
            break;
        case DOWN_LEFT: {
            while(IS_DOWN_LEFT_IN_RANGE(pointer_row, pointer_col)) {
                if(BOARD_DOWN_LEFT(pointer_row, pointer_col) == player) {
                    break;
                } else {
                    BOARD_DOWN_LEFT(pointer_row, pointer_col) = player;
                    pointer_col--;
                    pointer_row++;
                }
            }
        };
            break;
    }
}

void apply_move(uint8_t player, uint8_t pos_row, uint8_t pos_col) {
    board[pos_row][pos_col] = player;

    for(uint8_t i = 1; i < (APPLICABLE_DIRECTIONS_COUNT); i++) {
        apply_move_in_direction(player, pos_row, pos_col, applicable_directions[i]);
    }
    RESET_APPLICABLE_DIRECTIONS();
}

void print_board() {
    /* printf("\n "); */
    /* for(uint8_t j = 0; j < 15; j++) { */
    /*     printf("_"); */
    /* } */
    /* printf(" \n"); */
    /* for(uint8_t i = 0; i < BROWS; i++) { */
    /*     for(uint8_t j = 0; j < BCOLS; j++) { */
    /*         printf("|"); */
    /*         if(board[i][j] == EMPTY) */
    /*             printf(" "); */
    /*         else if (board[i][j] == WHITE) */
    /*             printf("O"); */
    /*         else */
    /*             printf("*"); */
    /*     } */
    /*     printf("|\n|"); */
    /*     for(uint8_t j = 0; j < 15; j++) { */
    /*         printf("_"); */
    /*     } */
    /*     printf("|\n"); */
    /* } */
}

bool has_legal_move(uint8_t player) {
    for(uint8_t i = 0; i < BROWS; i++) {
        for(uint8_t j = 0; j < BCOLS; j++) {
            if(board[i][j] != EMPTY) {
                continue;
            }
            if(is_valid_move(player, i, j)) {
                RESET_APPLICABLE_DIRECTIONS();
                return TRUE;
            }

        }
    }
    RESET_APPLICABLE_DIRECTIONS();
    return FALSE;
}
