/*
* Interrupt Driver
* Initializes interrupts
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "audio_driver.h"

/*********************************** macros **********************************/
#define AUDIO_DRIVER_MMAP_SIZE 0x1000 /* size of memory to allocate */

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the audio driver */
static int32_t buf = 0;
static uint32_t len = 0;
static uint16_t off = 0;

/********************************* functions *********************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t audio_driver_init(char devDevice[]) {
  /* open the device */
  fd = open(devDevice, O_RDWR);
  /* if there is a problem, return an error */
  if(fd == AUDIO_DRIVER_ERROR) {
    return AUDIO_DRIVER_ERROR;
  }
  /* map the virtual address to the appropriate location on the pynq */
  va = mmap(NULL, AUDIO_DRIVER_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, AUDIO_DRIVER_MMAP_OFFSET);
  /* if there's a problem, return an error */
  if(va == MAP_FAILED) {
    return AUDIO_DRIVER_ERROR;
  }
  return AUDIO_DRIVER_SUCCESS;
}

// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit() {
  munmap(va, AUDIO_DRIVER_MMAP_SIZE);
  close(fd);
}

// Called to write to the audio driver
void audio_driver_write() {
  write(fd,&buf,len);
}

// Called to read to the audio driver
void audio_driver_read() {
  read(fd,&buf,len);
}
