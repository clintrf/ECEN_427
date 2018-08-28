/*
* hdmi.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>


/*********************************** macros ***********************************/
#define HDMI_ERROR      -1      //error return value
#define HDMI_SUCCESS     0      //success return value
#define HDMI_MMAP_OFFSET 0      //mmap offset location for virtual address
#define HDMI_FILE_PATH "/dev/ecen427_hdmi" //pathway to follow to acccess the drivers

/**************************** function prototypes *****************************/
// initializes the hdmi driver
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, success code otherwise
int32_t hdmi_init(char devDevice[]);

// allows you to change the screen and what appears on it
// color : the desired color to change the pixel to
// pixels : the amount of pixels you wish to write to (multiples of 3)
uint32_t hdmi_write(const void *color, uint32_t pixels);

// use to change the offset without reading or writing
// offset : desired offset to change to
// return : returns the current offset, -1 if an error occurs
uint32_t hdmi_set_offset(uint32_t offset);

uint32_t hdmi_cur_offset(uint32_t offset);

// read a certain amount of bytes from the screen and puts them into a buffer
// *buf : points to the buffer where we should place our read
// bytes : the amount of bytes to read from the screen
// return : returns the number of bytes that are read
uint32_t hdmi_read(void *buf, uint32_t bytes);

// Called to exit the driver (unmap and close UIO file)
void hdmi_exit();
