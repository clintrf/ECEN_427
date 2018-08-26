#include <stdio.h>
#include <stdint.h>

#include "globals/globals.h"
#include "image_render.h"
#include "image_render/sprites/sprites.h"
#include "image_render/score_board/score_board.h"

#include "intcFolder/intc.h"
#include "uioFolder/button_uio.h"

/*********************************** macros ***********************************/
#define MOVE_ONE_SPACE 2
#define BOUNCE 1000000 /* value of the ticks to seconds */
#define BTN_0_MASK 0x1 /* mask for button 0 */
#define BTN_1_MASK 0x2 /* mask for button 1 */
#define BTN_2_MASK 0x4
#define BUFFER_SPACE 10
#define LETTER_A 11
#define CHAR_ARRAY_MAX 36
#define BYTES_PER_PIXEL 3
#define POSITION_ONE 0
#define POSITION_TWO 1
#define POSITION_THREE 2
#define MAX_COUNTER 3
#define TANK_STATE 0
#define SAUCER_STATE 1
#define SPRITES_STATE 2
#define ALIEN_MOVEMENT_DELAY 8
#define SHOTS_FIRED 1

/*********************************** globals ***********************************/
int32_t white_t[BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};
uint32_t button_counter = 0;
uint32_t button_pressed_counter = 0;
uint32_t letter_index = LETTER_A;
uint32_t interrupts;
char letter_1;
char letter_2;
char letter_3;
uint32_t alien_counter = 0;
uint32_t buttonPressed = 0;

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
  // Initialize interrupt controller driver
  intc_init(INTC_GPIO_FILE_PATH);
  // Initialize buttons
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);
  // Enable button and FIT interrupt lines on interrupt controller
  button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
  image_render_print_game_over_screen_enter_name(); // prints the enter name screen
  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }
  button_counter = 0;
  // allows us to enter our name on the game over screen
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

// moves the tank along the bottom of the screen
void move_tank(uint32_t buttonPressed) {
  switch(buttonPressed) { // reads in which button was pressed and decides what action to take for the tank
    case BTN_0_MASK:
      image_render_tank(MOVE_ONE_SPACE,IMAGE_RENDER_RIGHT_MOVEMENT);
      break;
    case BTN_1_MASK:
      if(globals_get_tank_bullet_fired() != SHOTS_FIRED) {
        image_render_fire_tank_bullet();
      }
      break;
    case BTN_2_MASK:
      image_render_tank(MOVE_ONE_SPACE,IMAGE_RENDER_LEFT_MOVEMENT);
      break;
  }
}

// handles the FIT interrupts, moves the saucer, alien block, and bullets
void isr_fit() {
  intc_ack_interrupt(INTC_FIT_MASK); // acknowledges the received FIT interrupt
  // This will count up to 100 ticks before updating the time to one second
  image_render_saucer();
  if(globals_get_tank_bullet_fired() == SHOTS_FIRED) {
    image_render_move_tank_bullet();
  }
  alien_counter++;
  if(alien_counter > ALIEN_MOVEMENT_DELAY) {
    image_render_move_alien_block();
    alien_counter = 0;
  }
}

// handles the button interrupts in the main game
void isr_buttons() {
  buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }
  button_counter = 0;

  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// main function that contains the main basic state machine
int main() {
  intc_init(INTC_GPIO_FILE_PATH); // intializes interrupts
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH); // initializes buttons
  image_render_init(); // initializes image making abilities
  image_render_print_start_screen();
  // bulk of state machine programming
  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    // Call interrupt controller function to wait for interrupt
    uint32_t interrupts = intc_wait_for_interrupt();

    // Check which interrupt lines are high and call the appropriate ISR functions
    if(interrupts & INTC_FIT_MASK) { // FIT Interrupt
      isr_fit();
    }
    if(interrupts & INTC_BTNS_MASK) { // buttons interrupt
      isr_buttons();
    }
    if ((buttonPressed == button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET) && buttonPressed != 0) ) { // manages the movement of the tank
      move_tank(buttonPressed);
    }

  }

  image_render_close(); // closes the image file
  button_uio_exit(); // exits from the button driver
  intc_exit(); // exits interrupt driver
}
