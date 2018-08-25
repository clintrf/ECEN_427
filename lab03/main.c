#include <stdio.h>
#include <stdint.h>
#include "image_render.h"
#include "intcFolder/intc.h"
#include "uioFolder/button_uio.h"

void run_game_over() {
  button_uio_acknowledge(BUTTON_UIO_CHANNEL_ONE_MASK); /* acknowledges an interrupt from the GPIO */
  intc_ack_interrupt(INTC_BTNS_MASK); /* acknowledges an interrupt from the interrupt controller */
  image_render_print_game_over_screen_enter_name();
}

int main() {
  intc_init(INTC_GPIO_FILE_PATH);
  button_uio_init(BUTTON_UIO_GPIO_FILE_PATH);
  image_render_init();
  image_render_test_image();
  while(1) {
    /* need to run this each time that we block, because this function will unblock */
    intc_enable_uio_interrupts(); /* enables Linux interrupts */
    uint32_t interrupts = intc_wait_for_interrupt();

    if(interrupts & INTC_BTNS_MASK) {
      run_game_over();
    }
  }

  image_render_print_game_over_screen_high_scores();
  image_render_close();
}
