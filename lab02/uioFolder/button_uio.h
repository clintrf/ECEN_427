/*
* button_uio.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>


/*********************************** macros ***********************************/
#define BUTTON_UIO_ERROR      -1      //error return value
#define BUTTON_UIO_SUCCESS     0      //success return value
#define BUTTON_UIO_MMAP_OFFSET     0     // offset of the mmap
#define BUTTON_UIO_CHANNEL_ONE_MASK 0x1 /* enables Channel 1 interrupts from selected device */
#define BUTTON_UIO_GPIO_DATA_OFFSET 0x0 /* the data from channel one received from the inerrupts */
#define BUTTON_UIO_GPIO_FILE_PATH "/dev/uio1"

/**************************** function prototypes *****************************/
// initializes the uio driver
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, success code otherwise
int32_t button_uio_init(char devDevice[]);

// write to a register of the UIO device
// offset : the offset of the register that we are writing to
// value : the value that we want to write to the register
void button_uio_write(uint32_t offset, uint32_t value);

// Acknowledge interrupt(s) in the interrupt controller
// value: Bitmask of interrupt lines to acknowledge.
void button_uio_acknowledge(uint32_t value);

// read from a register of the UIO device
// offset : the offset of the register we wich to read from
// returns : the value contained within the register we are reading from
uint32_t button_uio_read(uint32_t offset);

// Called to exit the driver (unmap and close UIO file)
void button_uio_exit();
