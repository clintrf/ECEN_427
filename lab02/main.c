    #include <stdint.h>
    #include "intcFolder/intc.h"
    #include <stdio.h>

    #define btns_mask 0x01
    #define switches_mask 0x02
    #define fit_mask 0x03

    // This is invoked in response to a timer interrupt.
    // It does 2 things: 1) debounce switches, and 2) advances the time.
    void isr_fit() {
      printf("fit interrupt\n");
    }

    // This is invoked each time there is a change in the button state (result of a push or a bounce).
    void isr_buttons() {
      printf("button pressed\n");
        // Read the state of the buttons
        // ... do something ...
        // Acknowledge the button interrupt
    }

    int main() {
        // Initialize interrupt controller driver
        intc_init("/dev/uio4");
        // Initialize buttons

        // Enable interrupt output from buttons
        // Enable button and FIT interrupt lines on interrupt controller

        while(1) {
            // Call interrupt controller function to wait for interrupt
            uint32_t interrupts = intc_wait_for_interrupt();
            
            printf("%zu  \r\n", interrupts);
            printf("%zu \r\n", switches_mask);
            // Check which interrupt lines are high and call the appropriate ISR functions
            if(interrupts & fit_mask) {
              isr_fit();
            }

            if(interrupts & btns_mask) {
              isr_buttons();
            }

            if(interrupts & switches_mask) {
            printf("switches\r\n");
        }
        }
    }
