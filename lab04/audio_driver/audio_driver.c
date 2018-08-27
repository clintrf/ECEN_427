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
#define END_OF_READ_FILE 0

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the audio driver */
static int32_t buf = 0;
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
// len : amount of bytes to write to the driver
 void audio_driver_write(int32_t len) {
  write(fd,&buf,len);
}

// Called to read to the audio driver
// len : the amount of bytes to read into the buffer
// returns a value with the type of success pending
int16_t audio_driver_read(int32_t len) {
  int32_t count = read(fd,&buf,len);
  if(count == len) { // optimal case success
    return AUDIO_DRIVER_READ_OPTIMAL_SUCCESS;
  }
  else if(count > END_OF_READ_FILE) { // read parts of the data, but not all
    return AUDIO_DRIVER_PARTIAL_DATA_TRANSFER;
  }
  else if(count == END_OF_READ_FILE) { // reached EOF before any data was read
    return AUDIO_DRIVER_REACHED_EOF;
  }
  else { // some kind of error occured if the number is negative
    return AUDIO_DRIVER_READ_ERROR;
  }
}
