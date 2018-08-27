#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "audio_driver.h"

static int fd; /* this is a file descriptor that describes the UIO device */
static int32_t buf = 0;
static uint32_t len = 0;
static uint16_t off = 0;

int main() {
  audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH);
  audio_driver_write();
  audio_driver_read();
  audio_driver_exit();
}
