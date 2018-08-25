#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "hdmi.h"

/********************************** macros ***********************************/
#define HDMI_MMAP_SIZE 0x1000 /* size of memory to allocate */

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */

/********************************* functions *********************************/
// initializes the hdmi driver
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, success code otherwise
int32_t hdmi_init(char devDevice[]) {
  /* open the device */
  fd = open(devDevice, O_RDWR);
  /* if there is a problem, return an error */
  if(fd == HDMI_ERROR) {
    return HDMI_ERROR;
  }

  // va = mmap(NULL, HDMI_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, HDMI_MMAP_OFFSET);
  // /* if there is a problem, return an error */
  // if(va == MAP_FAILED) {
  //   return HDMI_ERROR;
  // }

  return HDMI_SUCCESS;
}

// allows you to change the screen and what appears on it
// color : the desired color to change the pixel to
// pixels : the amount of pixels you wish to write to (multiples of 3)
uint32_t hdmi_write(const void *color, uint32_t pixels) {
  return write(fd, color, pixels); // it will begin to write wherever you are on the screen according to the offset of fd)
}

// use to change the offset without reading or writing
// offset : desired offset to change to
// return : returns the current offset, -1 if an error occurs
uint32_t hdmi_set_offset(uint32_t offset) {
  return lseek(fd, offset, SEEK_SET);
}

// read a certain amount of bytes from the screen and puts them into a buffer
// *buf : points to the buffer where we should place our read
// bytes : the amount of bytes to read from the screen
// return : returns the number of bytes that are read
uint32_t hdmi_read(void *buf, uint32_t bytes) {
  return read(fd, buf, bytes);
}

// Called to exit the driver (unmap and close UIO file)
void hdmi_exit() {
  // munmap(va, HDMI_MMAP_SIZE);
  close(fd);
}
