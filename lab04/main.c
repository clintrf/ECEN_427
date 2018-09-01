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
//  audio_driver_write((audio_driver_get_data_array(0)).data_size, audio_driver_get_data_array(0).data_size);
  for(int i=0;i<9;i++)
  {
    printf("number of samples for for array[%d]: %d\n",i,sound_data_array[i].num_samples);
  }
  audio_driver_read(len);
  audio_driver_exit();
}
