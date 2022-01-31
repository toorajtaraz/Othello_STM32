#ifndef UTILS_H_
#define UTILS_H_

#include "main.h"

#define COMMAND_END_GAME 1
#define COMMAND_MOVE 2
#define COMMAND_NEW_GAME 0
#define COMMAND_UNKNOWN 3
#define CONST_COMMAND_COUNT 2
#define CONST_COMMAND_LEN 9
#define DEBOUNCE_DELAY 250
#define EXTCOUNT 4
#define KEYPAD_DOWN '8'
#define KEYPAD_LEFT '4'
#define KEYPAD_OK '5'
#define KEYPAD_RIGHT '6'
#define KEYPAD_UP '2'
#define NOTPRESSED 128
#define OUTPUTCOUNT 4
#define COLINDEX 0
#define ROWINDEX 1
#define COLORINDEX 3



void clean_cmd();
uint8_t command_type();
void parse_command();
void handle_command();
void add_special_chars();
void update_selected_sqr();
void handle_melody();
void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume);
void write_number(int num);
void off_all();
void to_bcd(uint8_t digit, uint8_t b[4]);
void split_number(int num, uint8_t digits[4]);
void handle_adaptive_volume();
void handle_display();
void handle_time_managment();
void handle_uart();
void handle_led();
#endif // UTILS_H_
