#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "audio_driver/audio_driver.h"
#include "i2cps/i2cps.h"

int32_t len = 0;
const char * soundbuf;

int main() {
  config_audio_pll(0);
  config_audio_codec(0);
  audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH);
  audio_driver_write(soundbuf, len);
  audio_driver_read(len);
  audio_driver_exit();
}