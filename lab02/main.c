#include <stdint.h>
#include "intcFolder/intc.h"
#include <stdio.h>
#include <stdlib.h>
#include "uioFolder/button_uio.h"
#include "uioFolder/switch_uio.h"

/*********************************** macros ***********************************/
#define BTN_0_MASK 0x1 /* mask for button 0 */
#define BTN_1_MASK 0x2 /* mask for button 1 */
#define BTN_2_MASK 0x4 /* mask for button 2 */
#define SWTCH_0_MASK 0x1 /* mask for switch 0 */
#define SWTCH_0_MASK 0x1 /* first switch mask */
#define SECONDS_MAX 59 /* max number the seconds value can be */
#define MINUTES_MAX 59 /* max number the minutes value can be */
#define HOURS_MAX 23 /* max number the hours value can be */
#define TIME_MIN 0 /* smallest number the time values can be*/
#define SWITCH_FLAG_UP 1 /* value of the flag when it is up*/
#define SWITCH_FLAG_DOWN 0 /* value of the flag when it is down*/
#define TICKS_PER_SECOND 100 /* number of FIT interrupts taken to reach one second */
#define BOUNCE 100000 /* value of the ticks to seconds */
#define FIVE_SECONDS 1000000 /* value of the ticks to seconds */
#define DELAY_TIME 250000 /* value of the ticks to seconds for delay */

/*********************************** globals ***********************************/
int16_t switch_flag = SWITCH_FLAG_UP; /* this will track if the switch is flipped on or off */
int32_t seconds = TIME_MIN; /* this will track the seconds for the clock */
int32_t minutes = TIME_MIN; /* this will track the minutes for the clock */
int32_t hours = TIME_MIN; /* this will track the hours for the clock */

int32_t second_old = SECONDS_MAX; /* this will track the seconds for the clock */
int32_t minute_old = MINUTES_MAX; /* this will track the minutes for the clock */
int32_t hour_old = HOURS_MAX; /* this will track the hours for the clock */

int32_t counter = TIME_MIN; /*counter for the interrupts to increment seconds */
int32_t button_counter = TIME_MIN; /*counter for debounceing the buttons*/

int32_t multiple_buttons = 0; /* flag that states whether or not multiple buttons have been pressed */

/*********************************** functions ***********************************/

/* This function prints the time to the screen with every time that the time
* needs to be updated. This includes every second as the timer goes up and
* whenever a button is pressed to adjust the clock
*/
void print_time() {
  // clear screen and print new values if the time has be updated
  if(hour_old!=hours || minute_old!=minutes || second_old!=seconds){
    // Clear the screen from the previous time
    system("clear");
    printf("Time: %02zu:%02zu:%02zu\n", hours, minutes, seconds);
    // save new values into old time values
    hour_old = hours;
    minute_old = minutes;
    second_old = seconds;
  }
}

/* updates the time based on the switch possition.
* int32_t time_max : maximum amount of time (i.e. 59 secs, 59 minutes, 23 hours)
* int32_t *unit : pointer that says which unit of time must be updated
* int16_t switch_control : says whether to increment or decrement timer
*/
void update_time(int32_t time_max, int32_t *unit, int16_t switch_control) {
  int32_t temp = *unit;
  // if switch is up; increase time
  if(switch_control){
    if(temp >= time_max){ // if we have reached max time, set time back to 0
      temp = TIME_MIN;
    }
    else{ // if max time is not reached, go up one
      temp++;
    }
  }
  // if switch is down; decrease time
  else{
    if(temp <= TIME_MIN){ // if time is at 0, then reset it to the maximum time
      temp = time_max;
    }
    else{ // if we have not arrived at o, decrement time
      temp--;
    }
  }
  *unit = temp;
  print_time(); // print the time at the end to update it completely on screen
}

/* a switch statement that is used to tell how the program how the time
* should be updated.
* uint32_t buttonPressed : tells us which of the 3 buttons were pressed
*/
void increment_time(uint32_t buttonPressed){
  switch(buttonPressed) { // takes care of updating time depending on the btn pressed
    case BTN_0_MASK:
      // button 0 pressed
      update_time(SECONDS_MAX, &seconds, switch_flag);
      break;
    case BTN_1_MASK:
      // button 1 pressed
      update_time(MINUTES_MAX, &minutes, switch_flag);
      break;
    case BTN_2_MASK:
      // button 2 pressed
      update_time(HOURS_MAX, &hours, switch_flag);
      break;
    default:
      // multiple buttons or button 4 was pressed
      multiple_buttons = 1;
      break;
  }
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void isr_fit() {
  intc_ack_interrupt(INTC_FIT_MASK); // acknowledges the received FIT interrupt
  // This will count up to 100 ticks before updating the time to one second
  if(counter >= TICKS_PER_SECOND){
    int32_t temp_seconds = seconds;
    int32_t temp_minutes = minutes;

    update_time(SECONDS_MAX, &seconds, SWITCH_FLAG_UP);
    // If the maximum amount of seconds has been reached, then we count up 1 min
    if(temp_seconds>=SECONDS_MAX){
      update_time(MINUTES_MAX, &minutes, SWITCH_FLAG_UP);
    }
    // If the max number of mins is reached, we count up 1 hr
    if(temp_seconds>=SECONDS_MAX && temp_minutes >= MINUTES_MAX){
      update_time(HOURS_MAX, &hours, SWITCH_FLAG_UP);
    }
    // reset the tick counter so we can increment back to the ticks per second
    counter = TIME_MIN;
  }
  else{
    // if we haven't yet reached the ticks per sec parameter, increment the
    // counter to keep counting until the number is reached
    counter++;
  }
}

// This function is used to read, react, and acknowledge interrupts received from the switches
void isr_switches(){
  uint32_t switchState = switch_uio_read(SWITCH_UIO_GPIO_DATA_OFFSET); // read data from switches
  // if the switch is in the on position, set the switch flag to on (increment)
  if(switchState & SWTCH_0_MASK) {
    switch_flag = SWITCH_FLAG_UP;
  }
  else if(~(switchState & SWTCH_0_MASK)) {
    // if the switch is off, set the flag to off (decrement)
    switch_flag = SWITCH_FLAG_DOWN;
  }
  switch_uio_acknowledge(SWITCH_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_SWITCHES_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons() {
  uint32_t buttonPressed = button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET); // reads data from buttons
  // debounces the buttons
  while(button_counter < BOUNCE) {
      button_counter++;
  }

  // read the button again to check to see if the button is still pressed
  if(buttonPressed == button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET)){
    isr_switches(); // check state of the switches
    increment_time(buttonPressed); // update time accordingly
  }
  uint32_t auto_counter = 0; // auto_counter counts how long we should delay before auto increment begins
  uint32_t delay_counter = 0; // delay_counter allows for time until we need to count seconds
  // while we are holding down the button, do auto increment
  while (buttonPressed == button_uio_read(BUTTON_UIO_GPIO_DATA_OFFSET) && buttonPressed != 0){
      auto_counter++; // increment auto_counter
      if(auto_counter > FIVE_SECONDS) { // auto_counter must reach five seconds before we begin incrementing
       delay_counter++; // increment delay_counter
       if(delay_counter > DELAY_TIME) { // delay_counter must reach threshold before changing time
         isr_switches(); // check status of switches
         increment_time(buttonPressed); // update time accordingly
         delay_counter = 0; // reset the delay_counter
       }
      }
      // if we are pressing multiple buttons at the same time, do nothing
      if(multiple_buttons){
        break;
      }
  }
  // reset all counters
  auto_counter = 0;
  multiple_buttons = 0;
  button_counter = 0;
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// intitializes the drivers, runs the clock, and passes interrupts to their
// appropriate handlers
int main() {
  // Initialize interrupt controller driver
  intc_init(INTC_GPIO_FILE_PATH);
  // Initialize buttons
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);
  // Initialize switches
  switch_uio_init(SWITCH_UIO_GPIO_FILE_PATH);
  // Enable button and FIT interrupt lines on interrupt controller

  // Main body of the code, runs in a loop forever to keep time
  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    // Call interrupt controller function to wait for interrupt
    uint32_t interrupts = intc_wait_for_interrupt();

    // printf("%zu  \r\n", interrupts);

    // Check which interrupt lines are high and call the appropriate ISR functions
    if(interrupts & INTC_FIT_MASK) {
      isr_fit();
    }
    if(interrupts & INTC_BTNS_MASK) {
      isr_buttons();
    }
    if(interrupts & INTC_SWITCHES_MASK) {
      isr_switches();
    }
    print_time(); // prints the time to the screen
  }
}
