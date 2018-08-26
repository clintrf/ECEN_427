#include <stdint.h>
#include "intcFolder/intc.h"
#include <stdio.h>
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

/*********************************** globals ***********************************/
char switch_flag; /* this will track if the switch is flipped on or off */
int32_t seconds; /* this will track the seconds for the clock */
int32_t minutes; /* this will track the minutes for the clock */
int32_t hours; /* this will track the hours for the clock */

/*********************************** functions ***********************************/
// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void isr_fit() {
  intc_ack_interrupt(FIT_MASK);
}

void advanceTimeOneSecond(char incFlag) {
  if(incFlag) {
    seconds++;
  }
  else {
    seconds--;
  }
}

void advanceTimeOneMinute() {

}

void advanceTimeOneHour() {

}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons() {
  uint32_t buttonPressed = button_read(GPIO_DATA_OFFSET);
  switch(buttonPressed) {
    case BTN_0_MASK:
      printf("button 0 pressed\n");
      break;
    case BTN_1_MASK:
      printf("button 1 pressed\n");
      break;
    case BTN_2_MASK:
      printf("button 2 pressed\n");
      break;
    default:
     break;
  }
  // ... do something ...
  button_acknowledge(CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

void isr_switches(){
  uint32_t switchState = switch_read(GPIO_DATA_OFFSET);
  if(switchState & SWTCH_0_MASK) {
    switch_flag = 1;
  }
  else if(~(switchState & SWTCH_0_MASK)) {
    switch_flag = 0;
  }
  switch_acknowledge(CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(SWITCHES_MASK); /* acknowledges an interrupt from the interrupt controller */
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
//    printf("Time: %zu:%zu:%zu\n", hours, minutes, seconds);
  }
}
