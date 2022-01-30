#include "utils.h"
#include "main.h"
#include "LiquidCrystal.h"
#include "othello.h"
#include "math.h"

//definitions start
#define VOLUME_LEVEL 1000
//definitions end

//typedef start
typedef uint8_t byte;
//typedef end

//extern variables start
extern ADC_HandleTypeDef hadc4;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2;
extern uint8_t cur_pressed[];
extern uint8_t board[BROWS][BCOLS];
extern uint8_t status_led_sw;
extern uint8_t selected_sqr[3];
extern TIM_HandleTypeDef htim2;
extern uint8_t cur_pressed[];
extern uint8_t board[BROWS][BCOLS];
extern uint8_t status_led_sw;
extern uint8_t selected_sqr[3];
extern uint8_t game_state;
//extern variables end

//variables start

uint8_t occupied = FALSE;

uint32_t volume_prev_tick = 0;
uint16_t volume_min_raw = 0;
uint16_t volume_max_raw = 4095;
uint16_t volume_raw = 0;
uint8_t volume_has_changed_min = 0;
uint8_t volume_has_changed_max = 0;

uint16_t freq;
uint8_t song_tempo = 20;
uint16_t play_count = 0;
uint16_t note_to_play = 0;
uint16_t cur_play_count = 0;

const uint16_t melody[] = {
  // The Godfather theme
  // Score available at https://musescore.com/user/35463/scores/55160

  REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //1
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,

  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,

  NOTE_E4, 2, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //13

  //repeats from 2
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8, //2
  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,

  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,

  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,

  NOTE_E4, 2 //13
};

int _num = 1234;
uint8_t _i = 0;
int sevseg_tick_prev = 0;

uint8_t status_led_sw = LED_DEFAULT;

uint8_t selected_sqr[3] = {SQR_1, A_SQR, SQR_DEFAULT};

unsigned char data;
unsigned char cmd[30] = {'\0'};
int cmd_pointer = 0;
const unsigned char applicable_commands[2][9] = {
  "NEW GAME\0",
  "END GAME\0"
};

int tick_prev_keypad = 0;
uint16_t ext_pins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};
uint16_t output_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
uint8_t cur_pressed[] = {NOTPRESSED, NOTPRESSED};
const unsigned char keymap[][4] = {
              {'1', '2', '3', 'A'},
              {'4', '5', '6', 'B'},
              {'7', '8', '9', 'C'},
              {'S', '0', 'D', 'E'}
};

byte down_white[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x04,
  0x0A,
  0x0A,
  0x04
};
byte up_down_white[] = {
  0x04,
  0x0A,
  0x0A,
  0x04,
  0x04,
  0x0A,
  0x0A,
  0x04
};
byte up_white[] = {
  0x04,
  0x0A,
  0x0A,
  0x04,
  0x00,
  0x00,
  0x00,
  0x00
};
byte up_black[] = {
  0x04,
  0x0E,
  0x0E,
  0x04,
  0x00,
  0x00,
  0x00,
  0x00
};
byte up_down_black[] = {
  0x04,
  0x0E,
  0x0E,
  0x04,
  0x04,
  0x0E,
  0x0E,
  0x04
};
byte down_black[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x04,
  0x0E,
  0x0E,
  0x04
};
byte up_white_down_black[] = {
  0x04,
  0x0A,
  0x0A,
  0x04,
  0x04,
  0x0E,
  0x0E,
  0x04
};
byte up_black_down_white[] = {
  0x04,
  0x0E,
  0x0E,
  0x04,
  0x04,
  0x0A,
  0x0A,
  0x04
};
//variables end

//functions start
void clean_cmd() {
    for(uint8_t i = 0; i < 30; i++) {
      cmd[i] = '\0';
    }
}

uint8_t command_type() {
  uint8_t i = 0;
  for(uint8_t j = 0; j < CONST_COMMAND_COUNT; j++) {
    for(i = 0; i < (CONST_COMMAND_LEN - 1); i++) {
      if(cmd[i] != applicable_commands[j][i]) {
        break;
      }
    }
    if(i == (CONST_COMMAND_LEN - 1)) return j;
  }
  return COMMAND_UNKNOWN;
}

void parse_command() {
  if(
    !(
      cmd[ROWINDEX] <= '9' &&
      cmd[ROWINDEX] >= '1' &&
      cmd[COLINDEX] <= 'H' &&
      cmd[COLINDEX] >= 'A' &&
      (cmd[COLORINDEX] == 'W' || cmd[COLORINDEX] == 'B')
    )
  ) {
    return;
  }
  switch (cmd[ROWINDEX]) {
    case '1': {
      selected_sqr[BROW] = SQR_1;
    };
      break;
    case '2': {
      selected_sqr[BROW] = SQR_2;
    };
      break;
    case '3': {
      selected_sqr[BROW] = SQR_3;
    };
      break;
    case '4': {
      selected_sqr[BROW] = SQR_4;
    };
      break;
    case '5': {
      selected_sqr[BROW] = SQR_5;
    };
      break;
    case '6': {
      selected_sqr[BROW] = SQR_6;
    };
      break;
    case '7': {
      selected_sqr[BROW] = SQR_7;
    };
      break;
    case '8': {
      selected_sqr[BROW] = SQR_8;
    };
      break;
  }

  switch (cmd[COLINDEX]) {
    case 'A': {
      selected_sqr[BCOL] = A_SQR;
    };
      break;
    case 'B': {
      selected_sqr[BCOL] = B_SQR;
    };
      break;
    case 'C': {
      selected_sqr[BCOL] = C_SQR;
    };
      break;
    case 'D': {
      selected_sqr[BCOL] = D_SQR;
    };
      break;
    case 'E': {
      selected_sqr[BCOL] = E_SQR;
    };
      break;
    case 'F': {
      selected_sqr[BCOL] = F_SQR;
    };
      break;
    case 'G': {
      selected_sqr[BCOL] = G_SQR;
    };
      break;
    case 'H': {
      selected_sqr[BCOL] = H_SQR;
    };
      break;
  }
  selected_sqr[BSW] = SQR_SELECTED;
}

void handle_command() {
  uint8_t cmd_type = command_type();

  if(cmd_type == COMMAND_NEW_GAME) {
    selected_sqr[BSW] = B_NEW_GAME;
  } else if(cmd_type == COMMAND_END_GAME) {
    selected_sqr[BSW] = B_END_GAME;
  } else {
    parse_command();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart->Instance == USART3){
    cmd[cmd_pointer] = data == 0x0D ? '\0' : data;
    if(++cmd_pointer == 30 && data != 0x0D) {
      clean_cmd();
      cmd_pointer = 0;
    } else if (data == 0x0D) {
      handle_command();
      clean_cmd();
      cmd_pointer = 0;
    }
  }
  HAL_UART_Receive_IT(&huart3,&data,sizeof(data));
}

void add_special_chars() {
  createChar(UP_WHITE, up_white);
  createChar(UP_BLACK, up_black);
  createChar(UP_WHITE_DOWN_BLACK, up_white_down_black);
  createChar(UP_BLACK_DOWN_WHITE, up_black_down_white);
  createChar(UP_DOWN_BLACK, up_down_black);
  createChar(UP_DOWN_WHITE, up_down_white);
  createChar(DOWN_BLACK, down_black);
  createChar(DOWN_WHITE, down_white);
}

void update_selected_sqr() {
  switch (keymap[cur_pressed[0]][cur_pressed[1]]) {
    case KEYPAD_UP: {
      if(IS_UP_IN_RANGE(selected_sqr[BROW], selected_sqr[BCOL])) {
        selected_sqr[BROW]--;
      }
    };
      break;
    case KEYPAD_DOWN: {
      if(IS_DOWN_IN_RANGE(selected_sqr[BROW], selected_sqr[BCOL])) {
        selected_sqr[BROW]++;
      }
    };
      break;
    case KEYPAD_LEFT: {
      if(IS_LEFT_IN_RANGE(selected_sqr[BROW], selected_sqr[BCOL])) {
        selected_sqr[BCOL]--;
      }
    };
      break;
    case KEYPAD_RIGHT: {
      if(IS_RIGHT_IN_RANGE(selected_sqr[BROW], selected_sqr[BCOL])) {
        selected_sqr[BCOL]++;
      }
    };
      break;
    case KEYPAD_OK: {
      selected_sqr[BSW] = SQR_SELECTED;
    };
      break;
  }
  cur_pressed[0] = NOTPRESSED;
  cur_pressed[1] = NOTPRESSED;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  int now = HAL_GetTick();
  if((now - tick_prev_keypad) < DEBOUNCE_DELAY) {
    return;
  }
  tick_prev_keypad = now;
  uint8_t j = 0;
  for(uint8_t i = 0; i < OUTPUTCOUNT; i++) {
    for(j = 0; j < OUTPUTCOUNT; j++) {
      HAL_GPIO_WritePin(GPIOD, output_pins[j], GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(GPIOD, output_pins[i], GPIO_PIN_SET);
    for(j = 0; j < EXTCOUNT; j++) {
      if(HAL_GPIO_ReadPin(GPIOD, ext_pins[j])) {
        cur_pressed[0] = i;
        cur_pressed[1] = j;
      }
    }
  }

  for(j = 0; j < OUTPUTCOUNT; j++) {
    HAL_GPIO_WritePin(GPIOD, output_pins[j], GPIO_PIN_SET);
  }
  update_selected_sqr();
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) {
  if (pwm_freq == 0 || pwm_freq > 20000) {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  }
  else {
    const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
    const uint16_t prescaler = 1;
    const uint32_t timer_clock = internal_clock_freq;
    const uint32_t period_cycles = (uint32_t)timer_clock / (uint32_t)pwm_freq;
    const uint32_t pulse_width = (uint16_t)((volume * period_cycles) / 1000) / 2;
    htim2.Instance->PSC = prescaler - 1;
    htim2.Instance->ARR = period_cycles - 1;
    htim2.Instance->EGR = TIM_EGR_UG;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width);
  }
}

void split_number(int num, uint8_t digits[4]) {
    if(num > 9999) {
        return;
    }
    for(int i = 0; num > 0 && i < 4; i++) {
        uint8_t digit = num % 10;
        digits[3 - i] = digit;
        num /= 10;
    }
}

void to_bcd(uint8_t digit, uint8_t b[4]) {
   uint8_t bcd = 0;
   if (digit > 0) {
      bcd |= (digit % 10) << (0 << 2);
   }
   b[3] = (bcd & 8) > 0 ? 1 : 0;
   b[2] = (bcd & 4) > 0 ? 1 : 0;
   b[1] = (bcd & 2) > 0 ? 1 : 0;
   b[0] = (bcd & 1) > 0 ? 1 : 0;
}

void off_all() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 , 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOA,  GPIO_PIN_3, 1);
	HAL_GPIO_WritePin(GPIOA,  GPIO_PIN_5, 1);
}

void write_number(int num) {
	if (num > 9999 || num < 0) return;
	uint8_t digits[4] = {0};
	uint8_t b[4] = {0};
	uint16_t controllers[4] = {
                GPIO_PIN_5, GPIO_PIN_2,
                GPIO_PIN_3, GPIO_PIN_1
  };
	uint16_t ic_pins[4] = {
                GPIO_PIN_6, GPIO_PIN_2,
                GPIO_PIN_1, GPIO_PIN_7
  };
	GPIO_TypeDef *ic_gpios[4] = {
                GPIOA, GPIOB,
                GPIOB, GPIOA
  };

	split_number(num, digits);
	off_all();
	to_bcd(digits[_i], b);
	for(uint8_t j = 0; j < 4; j++) {
		HAL_GPIO_WritePin(ic_gpios[j],  ic_pins[j], b[j]);
	}
	HAL_GPIO_WritePin(GPIOA, controllers[_i], 0);
	if(++_i == 4) {
		_i = 0;
	}
}

void handle_melody() {
  return;
  if(cur_play_count < (melody[note_to_play + 1] * song_tempo)) {
    if(cur_play_count == 0)
      PWM_Change_Tone(melody[note_to_play], VOLUME_LEVEL);
    cur_play_count++;
  } else {
    cur_play_count = 0;
    note_to_play += 2;
    PWM_Change_Tone(0, 0);
    if(note_to_play == 258) {
      note_to_play = 0;
    }
  }
}


void handle_adaptive_volume() {
  if(HAL_GetTick() - 100 >= volume_prev_tick){
    volume_raw = HAL_ADC_GetValue(&hadc4);
    if((volume_min_raw == 0 && volume_has_changed_min == 0) || volume_raw < volume_min_raw) {
      volume_min_raw = volume_raw;
      volume_has_changed_min = 1;
    }
    if((volume_min_raw != volume_raw && volume_max_raw < volume_raw) || (volume_max_raw == 4095 && volume_min_raw != volume_raw && volume_has_changed_max == 0)) {
      volume_max_raw = volume_raw;
      volume_has_changed_max = 1;
    }
    volume_prev_tick = HAL_GetTick();
  }
}

void handle_display() {
  //some messy lock :)))
  while(occupied);
  occupied = TRUE;
  print_board();
  occupied = FALSE;
}

void handle_time_managment() {
  //Runs every 0.1 seconds
  _num = (int) ceil((volume_raw / (volume_max_raw - volume_min_raw)));
}
//functions end
