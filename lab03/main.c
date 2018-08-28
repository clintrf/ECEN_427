#include <stdio.h>
#include <stdint.h>
#include "image_render.h"
#include "image_render/sprites/sprites.h"
#include "image_render/score_board/score_board.h"

#include "intcFolder/intc.h"
#include "uioFolder/button_uio.h"

#define BOUNCE 10000000 /* value of the ticks to seconds */
#define BTN_0_MASK 0x1 /* mask for button 0 */
#define BTN_1_MASK 0x2 /* mask for button 1 */
#define BTN_2_MASK 0x4 /* mask for button 2 */

char white_t[3] = {0xFF, 0xFF, 0xFF};


uint32_t button_counter = 0;
uint32_t button_pressed_counter = 0;
uint32_t letter_index = 11;
uint32_t interrupts;

char letter_1;
char letter_2;
char letter_3;



void move_cursor(uint32_t buttonPressed) {
  switch(buttonPressed) {
    case BTN_0_MASK:
      // save character
      switch(button_pressed_counter){
        case 0:
          letter_1 = letter_array[letter_index];
          break;
        case 1:
          letter_2 = letter_array[letter_index];
          break;
        case 2:
          letter_3 = letter_array[letter_index];
          break;
      }
      button_pressed_counter++;
      letter_index = 11;
      break;
    case BTN_1_MASK:
      // if letter_index is greater than 36 then letter_index is 0
      if(letter_index>=36){
        letter_index = 11;
      }
      else{
        letter_index++;
      }
      // write new character
      sprites_render_image(char_array[letter_index],5,5,(SCORE_BOARD_START_POS+(button_pressed_counter*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_t);
      break;
    case BTN_2_MASK:
    // if letter_index is less than  then letter_index is 36
    if(letter_index<=11){
      letter_index = 36;
    }
    else{
      letter_index--;
    }
    // write new character
    sprites_render_image(char_array[letter_index],5,5,(SCORE_BOARD_START_POS+(button_pressed_counter*DEFAULT_CHAR_SCALE*ONE_PIXEL*10)),DEFAULT_CHAR_SCALE,white_t);
      break;
  }
}

void enter_your_name() {
  uint32_t buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }
  button_counter = 0;
  move_cursor(buttonPressed);
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

void run_game_over() {
  /* acknowledges first button press so that we can continue on */
  button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
  image_render_print_game_over_screen_enter_name();

  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }
  button_counter = 0;

  while(button_pressed_counter < 3) {
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    interrupts = intc_wait_for_interrupt();
    if(interrupts & INTC_BTNS_MASK) {
      enter_your_name();
    }
  }
  printf("Milestone 1\n");
  image_render_print_game_over_screen_high_scores();


  printf("Milestone 2\n");

  printf("%c, %c, %c \r\n",letter_1,letter_2,letter_3);

  save_score_board();
  update_stats(letter_1,letter_2,letter_3,99999);
  update_stats('A','A','A',0);
  printf("Milestone 3\n");
  print_high_scores();
}



int main() {
  intc_init(INTC_GPIO_FILE_PATH);
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);
  image_render_init();
  image_render_test_image();
  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    interrupts = intc_wait_for_interrupt();

    if(interrupts & INTC_BTNS_MASK) {
      run_game_over();
      break;
    }
  }
  image_render_close();
}
