#include <stdio.h>
#include <stdint.h>
#include "globals/globals.h"
#include "image_render.h"
#include "image_render/sprites/sprites.h"
#include "image_render/score_board/score_board.h"
#include "intcFolder/intc.h"
#include "uioFolder/button_uio.h"


/*********************************** macros ***********************************/
#define MOVE_ONE_SPACE 2                                                        // how many pixels to move for every movement
#define BOUNCE_BUTTONS 1000000                                                  // how many ticks to debounce the buttons for
#define BTN_0_MASK 0x1                                                          // mask for the right button
#define BTN_1_MASK 0x2                                                          // mask for the middle button
#define BTN_2_MASK 0x4                                                          // mask for the left button
#define BUFFER_SPACE 10                                                         // number of pixels to leave blank between the aliens
#define LETTER_A 11                                                             // index for letter 'A' in the char_array
#define CHAR_ARRAY_MAX 36                                                       // total number of possible characters to pic from
#define BYTES_PER_PIXEL 3                                                       // how many bytes in one pixel
#define POSITION_ONE 0                                                          // index for the first letter in the name
#define POSITION_TWO 1                                                          // index for the second letter in the name
#define POSITION_THREE 2                                                        // index for the third letter in the name
#define MAX_COUNTER 3                                                           // max umber of letters a user can have in their name
#define MAX_LIVES 5                                                             // the max number of lives a player can have
#define ALIEN_MOVEMENT_DELAY 8                                                  // how many ticks to wait before moving the aliens one space
#define SHOTS_FIRED 1                                                           // 1 indicates that a shot has been fired
#define MAX_ALIEN_SHOTS 1                                                       // the max number of bullets can be on the screen from the 4 different types of bullets
#define SAUCER_SHOT_DELAY_TIME 300                                              // delay on the max time the saucer would have to wait before coming back alive
#define SAUCER_SHOT 0                                                           // indicates a shot has NOT been fired
#define SAUCER_ALIVE 1                                                          // indicates if the saucer is alive
#define BULLET_DELAY_0 100                                                      // delay for shooting the first type of bullet
#define BULLET_DELAY_1 120                                                      // delay for shooting the second type of bullet
#define BULLET_DELAY_2 150                                                      // delay for shooting the third type of bullet
#define BULLET_DELAY_3 180                                                      // delay for shooting the fourth type of bullet
#define COUNTER_DELAY 4                                                         // a tick delay for the bullet counter
#define OVERRUN_DETECTED 1                                                      // indicates if the spites have reached the bunnker line

/********************************** globals **********************************/
int32_t default_color[BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};                    // default color to use on the main screen
uint32_t button_counter = 0;                                                    // counter to be used when debounceing the button_pressed_counter
uint32_t button_pressed_counter = 0;                                            // counter to be used to know if the buttons are being held down
uint32_t bullet_delay_0 = 0;                                                    // delay counter to be used to know when to shoot the next bullet of that type
uint32_t bullet_delay_1 = 0;                                                    // delay counter to be used to know when to shoot the next bullet of that type
uint32_t bullet_delay_2 = 0;                                                    // delay counter to be used to know when to shoot the next bullet of that type
uint32_t bullet_delay_3 = 0;                                                    // delay counter to be used to know when to shoot the next bullet of that type
uint32_t counter_delay = 0;                                                     // delay counter to be used to know when to shoot the next bullet of that type
uint32_t tanks_bullet_delay = 0;                                                // delay for when shooting the tank bullets that way the bullets dont move as fast as the ticks
uint32_t saucer_counter = 0;                                                    // counter to know how long sauser has been dead for
uint32_t letter_index = LETTER_A;                                               // start letter for when selecting the user name
uint32_t interrupts;                                                            // var to indicate which interupt had been called
char letter_1;                                                                  // letter saved in the first spot
char letter_2;                                                                  // letter saved in the second spot
char letter_3;                                                                  // letter saved in the third spot
uint32_t alien_counter = 0;                                                     // counter for how many aliens are Left
uint32_t buttonPressed = 0;                                                     // flag to know when a button has been preed
uint16_t alien_movement_delay = ALIEN_MOVEMENT_DELAY;                           // delay counter so the aliens dont move every tick
uint16_t max_alien_shots = MAX_ALIEN_SHOTS;                                     // max number of bullets the aleins can have on the screen at one time


/********************************* functions *********************************/
// moves the cursor around so the player can type in their name after the game is done
// buttonPressed : the button that was pressed on the board
void move_cursor(uint32_t buttonPressed) {
  switch(buttonPressed) {                                                       // reads in which button was pressed
    case BTN_0_MASK:                                                            // far right button (moves cursor to the right and saves the letter)
      switch(button_pressed_counter){                                           // depending on where the cursor is at, it will save or exit
        case POSITION_ONE:                                                      // saving the character in the first slot
          letter_1 = letter_array[letter_index];
          break;
        case POSITION_TWO:                                                      // saving the character in the second slot
          letter_2 = letter_array[letter_index];
          break;
        case POSITION_THREE:                                                    // saving the character in the third slot
          letter_3 = letter_array[letter_index];
          break;
      }
      button_pressed_counter++;                                                 // increments the counter to know to go to the next slot
      letter_index = LETTER_A;                                                  // sets the start letter to 'A'
      break;
    case BTN_1_MASK:
      if(letter_index>=CHAR_ARRAY_MAX){                                         // if letter_index is greater than 36 then letter_index is 0
        letter_index = LETTER_A;                                                // sets the start letter to 'A'
      }
      else{                                                                     // if its not greater than 36, simply parse through
        letter_index++;                                                         // increment the letter index counter to cycle UP through the letter selection
      }
      // write new character to the screen
      sprites_render_image(char_array[letter_index],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,(SPRITES_SCORE_BOARD_START_POS+(button_pressed_counter*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*BUFFER_SPACE)),SPRITES_NORMAL_CHARACTER_SCALING,default_color);
      break;
    case BTN_2_MASK:                                                            // increment the letter index counter to cycle DOWN through the letter selection
    if(letter_index<=LETTER_A){                                                 // if letter_index is less than  then letter_index is 36
      letter_index = CHAR_ARRAY_MAX;
    }
    else{                                                                       // if its not less than 0, simply parse through
      letter_index--;                                                           // decerement counter for parsing through letter selection
    }
    // write new character to screen
    sprites_render_image(char_array[letter_index],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,(SPRITES_SCORE_BOARD_START_POS+(button_pressed_counter*SPRITES_NORMAL_CHARACTER_SCALING*SPRITES_ONE_PIXEL*BUFFER_SPACE)),SPRITES_NORMAL_CHARACTER_SCALING,default_color);
      break;
  }
}

// runs the enter name portion of the game over screen. allows player to enter their name for high scores
void enter_your_name() {
  uint32_t buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET);        // reads data from buttons
  while(button_counter < BOUNCE_BUTTONS){                                       // debounces the buttons
      button_counter++;
  }
  button_counter = 0;                                                           // reset the button counter
  move_cursor(buttonPressed);                                                   // run the more cursor to determine what to do next
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK);                          // acknowledges an interrupt from the GPIO
  intc_ack_interrupt(INTC_BTNS_MASK);                                           // acknowledges an interrupt from the interrupt controller
}

// runs the game over portion of the game. (when all lives are lost, come here)
void run_game_over() {
  /* acknowledges first button press so that we can continue on */
  // Initialize interrupt controller driver
  intc_init(INTC_GPIO_FILE_PATH);
  // Initialize buttons
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);
  // Reads data from buttons
  button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET);
  // acknowledges an interrupt from the GPIO
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK);
  // acknowledges an interrupt from the interrupt controller
  intc_ack_interrupt(INTC_BTNS_MASK);
  // prints the enter name screen
  image_render_print_game_over_screen_enter_name();

  while(button_counter < BOUNCE_BUTTONS) {                                      // debounces the buttons
      button_counter++;
  }
  button_counter = 0;                                                           // reset the button counter after the debounce
  // allows us to enter our name on the game over screen
  while(button_pressed_counter < MAX_COUNTER) {                                 // while the counter is less than 3, run the enter_your_name state machine
    // enables Linux interrupts
    intc_enable_uio_interrupts();
    // waits to accept interrupts
    interrupts = intc_wait_for_interrupt();                                     // saves the interupt into a local variable
    if(interrupts & INTC_BTNS_MASK) {                                           // wait for interrupts from the buttons
      // runs the enter name portion of the game over screen. allows player to enter their name for high scores
      enter_your_name();
    }
  }
  // prints out the high score screen
  image_render_print_game_over_screen_high_scores();
  // updates the stats with the new information
  update_stats(letter_1,letter_2,letter_3,globals_get_current_score());
  // saves the newly acquired score into the score board before writing high scores out
  save_score_board();
  // prints out the high scores
  print_high_scores();
}

// moves the tank along the bottom of the screen
// buttonPressed : the button that was pressed on the board
void move_tank(uint32_t buttonPressed) {
  switch(buttonPressed) {                                                       // reads in which button was pressed and decides what action to take for the tank
    case BTN_0_MASK:                                                            // Right button pressed
      // draws the tank one space to the right
      image_render_tank(MOVE_ONE_SPACE,IMAGE_RENDER_RIGHT_MOVEMENT);
      break;
    case BTN_1_MASK:                                                            // Middle button pressed
      if(globals_get_tank_bullet_fired() != SHOTS_FIRED) {                      // if there is no current tank bullet on the screen fire bullet
        // draws the tank bullet
        image_render_fire_tank_bullet();
      }
      break;
    case BTN_2_MASK:                                                            // Left button pressed
      // draws the tank one space to the left
      image_render_tank(MOVE_ONE_SPACE,IMAGE_RENDER_LEFT_MOVEMENT);
      break;
  }
}

// handles the FIT interrupts, moves the saucer, alien block, and bullets
void isr_fit() {
  // acknowledges the received FIT interrupt
  intc_ack_interrupt(INTC_FIT_MASK);
  /* checks to see if the aliens have overrun the bunker location, if they have, then end the game */
  if(globals_get_alien_overrun_flag() == OVERRUN_DETECTED) {                    // if the aliens get too low on the screen, game over!
    for(uint16_t life = MAX_LIVES; life>0; life--){                             // if current life is not zero kill life
      // decerement lives
      globals_decrement_current_lives();
    }
  }
  /* bullet handling for the alien's bullets */
  if(bullet_delay_0 > BULLET_DELAY_0){                                          // delay to shoot the 0th bullet
    if(globals_get_alien_bullet_fired_0() != SHOTS_FIRED) {                     // if the 0th bullet has not been fired
      image_render_alien_fire_bullet_0();                                       // render the 0th alien bullet
      image_render_fire_alien_bullet_0();                                       // fire the 0th alien bullet
      bullet_delay_0 = 0;                                                       // reset the delay counter for when to fire 0th bullet
    }
  }
  if(bullet_delay_1 > BULLET_DELAY_1){                                          // delay to shoot the 1st bullet
    if(globals_get_alien_bullet_fired_1() != SHOTS_FIRED) {                     // if the 1st bullet has not been fired
      image_render_alien_fire_bullet_1();                                       // render the 1st alien bullet
      image_render_fire_alien_bullet_1();                                       // fire the 1sth alien bullet
      bullet_delay_1 = 0;                                                       // reset the delay counter for when to fire 1st bullet
    }
  }
  if(bullet_delay_2 > BULLET_DELAY_2){                                          // delay to shoot the 2nd bullet
    if(globals_get_alien_bullet_fired_2() != SHOTS_FIRED) {                     // if the 2nd bullet has not been fired
      image_render_alien_fire_bullet_2();                                       // render the 2nd alien bullet
      image_render_fire_alien_bullet_2();                                       // fire the 2nd alien bullet
      bullet_delay_2 = 0;                                                       // reset the delay counter for when to fire 2nd bullet
    }
  }
  if(bullet_delay_3 > BULLET_DELAY_3){                                          // delay to shoot the 3rd bullet
    if(globals_get_alien_bullet_fired_3() != SHOTS_FIRED) {                     // if the 3rd bullet has not been fired
      image_render_alien_fire_bullet_3();                                       // render the 3rd alien bullet
      image_render_fire_alien_bullet_3();                                       // fire the 3rd alien bullet
      bullet_delay_3 = 0;                                                       // reset the delay counter for when to fire 3rd bullet
    }
  }
  bullet_delay_0++;                                                             // increment the 0th bullet delay counter with each tick
  bullet_delay_1++;                                                             // increment the 1st bullet delay counter with each tick
  bullet_delay_2++;                                                             // increment the 2nd bullet delay counter with each tick
  bullet_delay_3++;                                                             // increment the 3rd bullet delay counter with each tick
  /* Also handles the alien bullets, this section moves them if they have already been fired
  *  We move the bullets 3 times per FIT tick to speed them up, making the game harder
  *  We need to include the if statement over each individual one to check to see if we hit
  *  something after every movement
  */
  if (counter_delay > COUNTER_DELAY) {                                          // COUNTER_DELAY
    if(globals_get_alien_bullet_fired_0() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_0();
    }
    if(globals_get_alien_bullet_fired_0() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_0();
    }
    if(globals_get_alien_bullet_fired_0() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_0();
    }
    if(globals_get_alien_bullet_fired_1() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_1();
    }
    if(globals_get_alien_bullet_fired_1() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_1();
    }
    if(globals_get_alien_bullet_fired_1() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_1();
    }
    if(globals_get_alien_bullet_fired_2() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_2();
    }
    if(globals_get_alien_bullet_fired_2() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_2();
    }
    if(globals_get_alien_bullet_fired_2() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_2();
    }
    if(globals_get_alien_bullet_fired_3() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_3();
    }
    if(globals_get_alien_bullet_fired_3() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_3();
    }
    if(globals_get_alien_bullet_fired_3() == SHOTS_FIRED) {                     // if the shots have been fired
      image_render_move_alien_bullet_3();
    }
    counter_delay = 0;                                                          // reset counter delay
  }
  counter_delay++;                                                              // increment the counter delay
  /* saucer flight handling in the main */
  if(globals_get_saucer_status() == SAUCER_SHOT) {                              // if the saucer is currently dead
    uint32_t saucer_count = globals_get_saucer_shot_count();                    // save the number of time the sauser has been shot
    if(saucer_count > SAUCER_SHOT_DELAY_TIME) {                                 // we wait a certain amount of time before reprinting it
      globals_set_saucer_status(SAUCER_ALIVE);                                  // set the saucer back to being alive
      globals_reset_saucer_shot_count();                                        // reset count for if the sauser has been shot
      image_render_saucer();                                                    // draw the sauser
    }
    else {                                                                      // if the saucer hasn't reached the specified time, then we increment the counter
      globals_inc_saucer_shot_count();
    }
  }
  else {                                                                        // if the saucer is currently alive
    image_render_saucer();
  }
  /* This controls the firing of the bullets and their movement
  *  We will move the tank bullet twice per FIT interrupt, making it move faster
  */
  if(globals_get_tank_bullet_fired() == SHOTS_FIRED) {                          // if the bullet has been fired
    image_render_move_tank_bullet();
  }
  if(globals_get_tank_bullet_fired() == SHOTS_FIRED) {                          // if the bullet has been fired
    image_render_move_tank_bullet();
  }
  /* this part oversees the movement of the alien block */
  alien_counter++;                                                              // increments the alien counter
  if(alien_counter > alien_movement_delay) {                                    // a little bit of a delay for the alien movement
    image_render_move_alien_block();
    alien_counter = 0;
  }
  globals_print_current_score();                                                // print the current score to the screen
  globals_print_current_lives();                                                // print the current number of lives to the screen
}

// handles the button interrupts in the main game
void isr_buttons() {
  buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET);                 // reads data from buttons
  while(button_counter < BOUNCE_BUTTONS) {                                      // debounces the buttons
      button_counter++;
  }
  button_counter = 0;                                                           // after debounce reset the button counte delay
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK);                          // acknowledges an interrupt from the GPIO
  intc_ack_interrupt(INTC_BTNS_MASK);                                           // acknowledges an interrupt from the interrupt controller
}


/*********************************** main ***********************************/
int main() {
  intc_init(INTC_GPIO_FILE_PATH);                                               // intializes interrupts
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);                                   // initializes buttons
  image_render_init();                                                          // initializes image making abilities
  image_render_print_start_screen();
  // bulk of running program. keep running the program until we run out of lives
  while(globals_get_current_lives() > 0) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    uint32_t interrupts = intc_wait_for_interrupt();                            // Call interrupt controller function to wait for interrupt
    // Check which interrupt lines are high and call the appropriate ISR functions
    if(interrupts & INTC_FIT_MASK) {                                            // FIT Interrupt
      isr_fit();
    }
    if(interrupts & INTC_BTNS_MASK) {                                           // buttons interrupt
      isr_buttons();
    }
    if ((buttonPressed == button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET) && buttonPressed != 0) ) { // manages the movement of the tank
      move_tank(buttonPressed);
    }
  }
  run_game_over();                                                              // runs the game over screen
  image_render_close();                                                         // closes the image file, which shuts connection to the HDMI
  button_uio_exit();                                                            // exits from the button driver
  intc_exit();                                                                  // exits interrupt driver
}
