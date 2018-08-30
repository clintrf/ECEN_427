#include <stdio.h>
#include <stdint.h>
#include "audio/audio.c"

static int fd;

int main() {
  fd = open("dev/audio", O_RDWR);

  //buffer = 0;
  //read (buffer);
  //write (buffer);

  return 0;
}
