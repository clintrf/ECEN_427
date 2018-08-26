#include <stdint.h>
#include "intcFolder/intc.h"
#include <stdio.h>
#include "uioFolder/button_uio.h"
#include "uioFolder/switch_uio.h"

#define BTNS_MASK 0x2
#define SWITCHES_MASK 0x4
#define FIT_MASK 0x1
#define CHANNEL_ONE_MASK 0x1

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void isr_fit() {
  intc_ack_interrupt(FIT_MASK);
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void isr_buttons() {
  printf("button pressed\n");
  // Read the state of the buttons
  // ... do something ...
  button_acknowledge(CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
}

void isr_switches(){
  printf("switch interrupt\n");
    // Read the state of the switches
    // ... do somethin ...
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
  }
}
