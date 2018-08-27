#include <stdint.h>
#include "intcFolder/intc.h"
#include <stdio.h>
#include <stdlib.h>
#include "uioFolder/button_uio.h"
#include "uioFolder/switch_uio.h"

/*********************************** macros ***********************************/
#define FIT_MASK 0x1 /* interrupt handler's first bit: corresponds to fit */
#define BTNS_MASK 0x2 /* interrupt handler's second bit: corresponds to buttons */
#define SWITCHES_MASK 0x4 /* interrupt handler's third bit: corresponds to switches */
#define CHANNEL_ONE_MASK 0x1 /* the IP ISR's first bit. Needed to acknowledge interrupts in the GPIO */
#define GPIO_DATA_OFFSET 0x0 /* the data from channel one received from the inerrupts */
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
#define BOUNCE 100000 /* value of the ticks to seconds*/
#define FIVE_SECONDS 1000000 /* value of the ticks to seconds*/
#define DELAY_TIME 250000 /* value of the ticks to seconds for delay*/


/*********************************** globals ***********************************/
char switch_flag = SWITCH_FLAG_UP; /* this will track if the switch is flipped on or off */
int32_t seconds = TIME_MIN; /* this will track the seconds for the clock */
int32_t minutes = TIME_MIN; /* this will track the minutes for the clock */
int32_t hours = TIME_MIN; /* this will track the hours for the clock */

int32_t second_old = SECONDS_MAX; /* this will track the seconds for the clock */
int32_t minute_old = MINUTES_MAX; /* this will track the minutes for the clock */
int32_t hour_old = HOURS_MAX; /* this will track the hours for the clock */

int32_t counter = TIME_MIN; /*counter for the interrupts to increment seconds */
int32_t button_counter = TIME_MIN; /*counter for debounceing the buttons*/


int32_t multiple_buttons = 0;



/*********************************** functions ***********************************/

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

// updateds the time based on the switch possition.
void update_time(int32_t time_max, int32_t *unit, char switch_control) {
  int32_t temp = *unit;
  // if switch is up increase time
  if(switch_control){
    if(temp >= time_max){
      temp = TIME_MIN;
    }
    else{
      temp++;
    }
  }
  // if switch is down decrease time
  else{
    if(temp <= TIME_MIN){
      temp = time_max;
    }
    else{
      temp--;
    }
  }
  *unit = temp;
  print_time();
}

void increment_time(uint32_t buttonPressed){
  switch(buttonPressed) {
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
      //printf("%s \r\n", "ERROR");
      multiple_buttons = 1;
      break;
  }
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void isr_fit() {
  intc_ack_interrupt(FIT_MASK);
  if(counter >= 100){
    int32_t temp_seconds = seconds;
    int32_t temp_minutes = minutes;

    update_time(SECONDS_MAX, &seconds, SWITCH_FLAG_UP);
    if(temp_seconds>=SECONDS_MAX){
      update_time(MINUTES_MAX, &minutes, SWITCH_FLAG_UP);
    }
    if(temp_seconds>=SECONDS_MAX && temp_minutes >= MINUTES_MAX){
      update_time(HOURS_MAX, &hours, SWITCH_FLAG_UP);
    }

    counter = TIME_MIN;
  }
  else{
    counter++;
  }
}

void isr_switches(){
  uint32_t switchState = switch_read(GPIO_DATA_OFFSET);
  if(switchState & SWTCH_0_MASK) {
    switch_flag = SWITCH_FLAG_UP;
  }
  else if(~(switchState & SWTCH_0_MASK)) {
    switch_flag = SWITCH_FLAG_DOWN;
  }
  switch_acknowledge(CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(SWITCHES_MASK); /* acknowledges an interrupt from the interrupt controller */
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons() {
  uint32_t buttonPressed = button_read(GPIO_DATA_OFFSET);
  while(button_counter < BOUNCE) {
      button_counter++;
  }

  if(buttonPressed == button_read(GPIO_DATA_OFFSET)){
    isr_switches();
    increment_time(buttonPressed);
  }
  uint32_t auto_counter = 0;
  uint32_t delay_counter = 0;
  while (buttonPressed == button_read(GPIO_DATA_OFFSET) && buttonPressed != 0){
      auto_counter++;
      if(auto_counter > FIVE_SECONDS) {
       delay_counter++;
       if(delay_counter > DELAY_TIME) {
         isr_switches();
         increment_time(buttonPressed);
         delay_counter = 0;
       }
      }
      if(multiple_buttons){
        break;
      }
  }
  auto_counter = 0;
  multiple_buttons = 0;

  button_counter = TIME_MIN;
  // ... do something ...
  button_acknowledge(CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

int main() {
  // Initialize interrupt controller driver
  intc_init("/dev/uio4");
  // Initialize buttons
  button_init("/dev/uio1");
  // Initialize switches
  switch_init("/dev/uio2");
  // Enable button and FIT interrupt lines on interrupt controller


  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    // Call interrupt controller function to wait for interrupt
    uint32_t interrupts = intc_wait_for_interrupt();

    // printf("%zu  \r\n", interrupts);

    // Check which interrupt lines are high and call the appropriate ISR functions
    if(interrupts & FIT_MASK) {
      isr_fit();
    }
    if(interrupts & BTNS_MASK) {
      isr_buttons();
    }
    if(interrupts & SWITCHES_MASK) {
      isr_switches();
    }
    print_time();
  }
}
