#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "audio_driver/audio_driver.h"
#include "i2cps/i2cps.h"

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int32_t len = 0;
const char * soundbuf;

int main() {
  // initialize codec
  config_audio_pll(0);
  config_audio_codec(0);
  // initialize audio_driver
  audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH);
  printf("\t\t***done initing all audio files***\n\r");

  struct audio_data copy = audio_driver_get_data_array(4);

  FILE *fpp;
  fpp = fopen("/home/xilinx/ECEN_427/lab04/t.txt", "w");
  fprintf(fpp, "This is testing for fprintf...\n");
  for(uint32_t i = 0; i < copy.num_samples; i++) {
    fprintf(fpp,"Data @ Index %zu: %x\n",i,copy.sound_data[i]);
  }
  fclose(fpp);

  int i = 0;
  while(1) {
    int stuff = audio_driver_read(audio_driver_get_data_array(i).sound_data,audio_driver_get_data_array(i).num_samples);
    if(!stuff) {
      sleep(3);
      audio_driver_write(audio_driver_get_data_array(i).sound_data,audio_driver_get_data_array(i).num_samples);
      i++;
      if(i > 8) {
        i = 0;
      }
    }
    printf("Current Index: %zu\n",i);
  }

  printf("\t invader_die audio sound played\n\r");
  // exit from the audio driver and free up allocated memory
  audio_driver_exit();
}
