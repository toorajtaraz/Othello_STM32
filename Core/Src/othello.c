#include "othello.h"
#include "LiquidCrystal.h"
#include "string.h"
#include "stdio.h"

extern uint8_t status_led_sw;
extern uint8_t selected_sqr[];

uint8_t game_state = GAME_HALTED;
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
uint8_t white_score = 2;
uint8_t black_score = 2;
char turn = 'B';


uint8_t game_total_time;
uint8_t elapsed_game_time;
uint32_t game_round_prev_tick = 0;

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

    white_score = 2;
    black_score = 2;
    turn = 'B';
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
    white_score = 0;
    black_score = 0;
    for(uint8_t i = 0; i < BROWS; i += 2) {
        setCursor(BOARD_POS_COL_START - 1, BOARD_POS_ROW_START + (i >> 1));
        print("|");
        setCursor(BOARD_POS_COL_END + 1, BOARD_POS_ROW_START + (i >> 1));
        print("|");
        for(uint8_t j = 0; j < BCOLS; j++) {
            setCursor(BOARD_POS_COL_START + j, BOARD_POS_ROW_START + (i >> 1));
            if (board[i][j] == BLACK && board[i + 1][j] == BLACK) {
                write(UP_DOWN_BLACK);
                black_score += 2;
            } else if (board[i][j] == BLACK && board[i + 1][j] == EMPTY) {
                write(UP_BLACK);
                black_score++;
            } else if (board[i][j] == BLACK && board[i + 1][j] == WHITE) {
                write(UP_BLACK_DOWN_WHITE);
                black_score++;
                white_score++;
            } else if (board[i][j] == EMPTY && board[i + 1][j] == BLACK) {
                write(DOWN_BLACK);
                black_score++;
            } else if (board[i][j] == EMPTY && board[i + 1][j] == EMPTY) {
                write(' ');
            } else if (board[i][j] == EMPTY && board[i + 1][j] == WHITE) {
                write(DOWN_WHITE);
                white_score++;
            } else if (board[i][j] == WHITE && board[i + 1][j] == BLACK) {
                write(UP_WHITE_DOWN_BLACK);
                white_score++;
                black_score++;
            } else if (board[i][j] == WHITE && board[i + 1][j] == EMPTY) {
                write(UP_WHITE);
                white_score++;
            } else if (board[i][j] == WHITE && board[i + 1][j] == WHITE) {
                write(UP_DOWN_WHITE);
                white_score += 2;
            }
        }
    }
}

void print_game_info() {
    setCursor(LEFT_INFO_POS_COL_START, INFO_POS_ROW_START + 1);
    char info[5] = {'\0'};
    sprintf(&info[0], "%02d", white_score);
    print("WHITE");
    setCursor(LEFT_INFO_POS_COL_START, INFO_POS_ROW_START + 2);
    print(info);
    setCursor(RIGHT_INFO_POS_COL_START, INFO_POS_ROW_START + 1);
    sprintf(&info[0], "%02d", black_score);
    print("BLACK");
    setCursor(RIGHT_INFO_POS_COL_START, INFO_POS_ROW_START + 2);
    print(info);

    setCursor(RIGHT_INFO_POS_COL_START, INFO_POS_ROW_END);
    info[4] = '\0';
    info[2] = ' ';
    info[3] = turn;
    switch (selected_sqr[BROW]) {
        case SQR_1: {
            info[1] = '1';
        };
            break;
        case SQR_2: {
            info[1] = '2';
        };
            break;
        case SQR_3: {
            info[1] = '3';
        };
            break;
        case SQR_4: {
            info[1] = '4';
        };
            break;
        case SQR_5: {
            info[1] = '5';
        };
            break;
        case SQR_6: {
            info[1] = '6';
        };
            break;
        case SQR_7: {
            info[1] = '7';
        };
            break;
        case SQR_8: {
            info[1] = '8';
        };
            break;
    }

    switch (selected_sqr[BCOL]) {
        case A_SQR: {
            info[0] = 'A';
        };
            break;
        case B_SQR: {
            info[0] = 'B';
        };
            break;
        case C_SQR: {
            info[0] = 'C';
        };
            break;
        case D_SQR: {
            info[0] = 'D';
        };
            break;
        case E_SQR: {
            info[0] = 'E';
        };
            break;
        case F_SQR: {
            info[0] = 'F';
        };
            break;
        case G_SQR: {
            info[0] = 'G';
        };
            break;
        case H_SQR: {
            info[0] = 'H';
        };
            break;
    }
    print(info);

    setCursor(LEFT_INFO_POS_COL_START, INFO_POS_ROW_END);
    sprintf(&info[0], (white_score > black_score) ? "%02d W" : "%02d B", (white_score > black_score) ? (white_score - black_score) : (black_score - white_score));
    print(info);
    if (game_state == GAME_ENDED) {
        setCursor(RIGHT_INFO_POS_COL_START, INFO_POS_ROW_START);
        print("OVER");
    } else {
        setCursor(RIGHT_INFO_POS_COL_START, INFO_POS_ROW_START);
        print("    ");
    }
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

void handle_logic() {
    if(selected_sqr[BSW] == B_NEW_GAME) {
        initialize_board();
        game_state = GAME_RUNNING;
        game_round_prev_tick = HAL_GetTick();
        return;
    }
    if(selected_sqr[BSW] == B_END_GAME) {
        initialize_board();
        game_state = GAME_HALTED;
        return;
    }
    if(game_state == GAME_ENDED || game_state == GAME_HALTED || ! (selected_sqr[BSW] == SQR_SELECTED)) return;

    if(has_legal_move((turn == 'W') ? WHITE : BLACK) == FALSE) {
        turn = turn == 'B' ? 'W' : 'B';
        return;
    }

    if (is_valid_move((turn == 'W') ? WHITE : BLACK, selected_sqr[BROW], selected_sqr[BCOL]) == FALSE) {
        selected_sqr[BSW] = SQR_DEFAULT;
        status_led_sw = LED_WRONG_MOVE;
        return;
    }
    if(selected_sqr[BSW] == SQR_SELECTED) {
        apply_move((turn == 'W') ? WHITE : BLACK, selected_sqr[BROW], selected_sqr[BCOL]);
        selected_sqr[BSW] = SQR_DEFAULT;
    }
    game_round_prev_tick = HAL_GetTick();
    elapsed_game_time = game_total_time;
    turn = turn == 'B' ? 'W' : 'B';
    if(has_legal_move(BLACK) == FALSE && has_legal_move(WHITE) == FALSE) {
        game_state = GAME_ENDED;
    }
}
