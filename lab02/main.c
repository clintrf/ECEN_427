    #include <stdint.h>
    #include "intcFolder/intc.h"
    #include <stdio.h>
    #include "uioFolder/generic_uio.h"

    #define BTNS_MASK 0x2
    #define SWITCHES_MASK 0x4
    #define FIT_MASK 0x1

    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit() {
      printf("fit interrupt\n");
      intc_ack_interrupt(FIT_MASK);
    }

    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons() {
      printf("button pressed\n");
      intc_ack_interrupt(BTNS_MASK);
        // Read the state of the buttons
        // ... do something ...
        // Acknowledge the button interrupt
    }

    int main() {
        // Initialize interrupt controller driver
        intc_init("/dev/uio4");

        // Initialize buttons
        int32_t stuff = generic_init("/dev/uio1");

        // Enable interrupt output from button
        
        // Enable button and FIT interrupt lines on interrupt controller
        
        
        while(1) {
            // Call interrupt controller function to wait for interrupt
            uint32_t interrupts = intc_wait_for_interrupt();

            printf("%zu  \r\n", interrupts);
            
            // Check which interrupt lines are high and call the appropriate ISR functions
            if(interrupts & FIT_MASK) {
              isr_fit();
            }
		
            if(interrupts & BTNS_MASK) {
              isr_buttons();
            }

            if(interrupts & SWITCHES_MASK) {
              printf("switches\r\n");
            }
        }
    }
