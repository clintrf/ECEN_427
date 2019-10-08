#include <stdio.h>
#include <stdint.h>

#ifndef GLOBALS_C
#include "globals/globals.c"
#endif

#include "image_render.h"
#include "image_render/sprites/sprites.h"
#include "image_render/score_board/score_board.h"

#include "intcFolder/intc.h"
#include "uioFolder/button_uio.h"

/*********************************** macros ***********************************/


/*********************************** globals ***********************************/
char white_t[BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};
uint32_t button_counter = 0;
uint32_t button_pressed_counter = 0;
uint32_t letter_index = LETTER_A;
uint32_t interrupts;
char letter_1;
char letter_2;
char letter_3;

/*********************************** functions ***********************************/
// moves the cursor around so the player can type in their name after the game is done
// buttonPressed : the button that was pressed on the board
void move_cursor(uint32_t buttonPressed) {
  switch(buttonPressed) { // reads in which button was pressed
    case BTN_0_MASK: // far right button (moves cursor to the right and saves the letter)
      switch(button_pressed_counter){ // depending on where the cursor is at, it will save or exit
        case POSITION_ONE:
          letter_1 = letter_array[letter_index];
          break;
        case POSITION_TWO:
          letter_2 = letter_array[letter_index];
          break;
        case POSITION_THREE:
          letter_3 = letter_array[letter_index];
          break;
      }
      button_pressed_counter++;
      letter_index = LETTER_A;
      break;
    case BTN_1_MASK:
      // if letter_index is greater than 36 then letter_index is 0
      if(letter_index>=CHAR_ARRAY_MAX){
        letter_index = LETTER_A;
      }
      else{ // if its not greater than 36, simply parse through
        letter_index++;
      }
      // write new character
      sprites_render_image(char_array[letter_index],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,(SPRITES_SCORE_BOARD_START_POS+(button_pressed_counter*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*BUFFER_SPACE)),SPRITES_NORMAL_CHARACTER_SCALING,white_t);
      break;
    case BTN_2_MASK:
    // if letter_index is less than  then letter_index is 36
    if(letter_index<=LETTER_A){
      letter_index = CHAR_ARRAY_MAX;
    }
    else{ // if its not less than 0, simply parse through
      letter_index--;
    }
    // write new character
    sprites_render_image(char_array[letter_index],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,(SPRITES_SCORE_BOARD_START_POS+(button_pressed_counter*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*BUFFER_SPACE)),SPRITES_NORMAL_CHARACTER_SCALING,white_t);
      break;
  }
}

// runs the enter name portion of the game over screen. allows player to enter their name for high scores
void enter_your_name() {
  uint32_t buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  while(button_counter < BOUNCE) { // debounces the buttons
      button_counter++;
  }
  button_counter = 0;
  move_cursor(buttonPressed); // run the more cursor to determine what to do next
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// runs the game over portion of the game. (when all lives are lost, come here)
void run_game_over() {
  /* acknowledges first button press so that we can continue on */
  button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
  image_render_print_game_over_screen_enter_name(); // prints the enter name screen
  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }
  button_counter = 0;

  while(button_pressed_counter < MAX_COUNTER) { // while the counter is less than 3, run the enter_your_name state machine
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    interrupts = intc_wait_for_interrupt(); // waits to accept interrupts
    if(interrupts & INTC_BTNS_MASK) { // wait for interrupts from the buttons
      enter_your_name();
    }
  }
  image_render_print_game_over_screen_high_scores(); // prints out the high score screen
  update_stats(letter_1,letter_2,letter_3,99999); // updates the stats with the new information
  save_score_board(); // saves the newly acquired score into the score board before writing high scores out
  print_high_scores(); // prints out the high scores
}

void move_tank( uint32_t buttonPressed){
  switch(buttonPressed) { // reads in which button was pressed
    case BTN_0_MASK:
      printf("%s moving right\r\n", "tank");
      image_render_tank(1, 1);
      break;
    case BTN_1_MASK:
      printf("%s shooting\r\n", "tank");
      break;
    case BTN_2_MASK:
      printf("%s moving left\r\n", "tank");
      image_render_tank(1, 0);
      break;
  }
}

void isr_buttons() {
  uint32_t buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }

  // while we are holding down the button, do auto increment
  while (buttonPressed == button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET) && buttonPressed != 0){
    move_tank(buttonPressed);
  }
  printf("%s moving or shooting\r\n", "not");
  // reset all counters
  //multiple_buttons = 0;
  button_counter = 0;
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// main function that contains the main basic state machine
int main() {
  intc_init(INTC_GPIO_FILE_PATH); // intializes interrupts
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH); // initializes buttons
  image_render_init(); // initializes image making abilities
  printf("Checkpoint1\n");
  image_render_print_start_screen();
  printf("Checkpoint3\n");
  // bulk of state machine programming
  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    uint32_t interrupts = intc_wait_for_interrupt();
    // Check which interrupt lines are high and call the appropriate ISR functions
    if(interrupts & INTC_FIT_MASK) {
      //isr_fit();
    }
    if(interrupts & INTC_BTNS_MASK) {
      isr_buttons();
    }
  }
  image_render_close(); // closes the image file
  button_uio_exit(); // exits from the button driver
  intc_exit(); // exits interrupt driver
}
