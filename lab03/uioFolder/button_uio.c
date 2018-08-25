/*
* Button Driver
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "button_uio.h"

/*********************************** macros ***********************************/
#define BUTTON_UIO_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define BUTTON_UIO_GIER_REG_OFFSET 0x11C /* global interrupt register offset */
#define BUTTON_UIO_GIER_MASK (1<<31) /* top register bit (31) is set to one */
#define BUTTON_UIO_IP_IER_REG_OFFSET 0x128 /* IP IER offset value */
#define BUTTON_UIO_IP_ISR_REG_OFFSET 0x120 /* IP ISR register offset */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */


/**************************** functions *****************************/
/* initializes the uio driver */
int32_t button_uio_init(char devDevice[]) {
	/* open the device */
	fd = open(devDevice, O_RDWR);
	/* if there is a problem, return an error */
	if(fd == BUTTON_UIO_ERROR) {
		return BUTTON_UIO_ERROR;
	}

	va = mmap(NULL, BUTTON_UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BUTTON_UIO_MMAP_OFFSET);
	/* if there is a problem, return an error */
	if(va == MAP_FAILED) {
		return BUTTON_UIO_ERROR;
	}

	/* put hardware setup here */
	/* enable channel interrupt in the IP IER */
	button_uio_write(BUTTON_UIO_IP_IER_REG_OFFSET, BUTTON_UIO_CHANNEL_ONE_MASK);
	/* enable global interrupt by setting bit 31 of the GIR */
	button_uio_write(BUTTON_UIO_GIER_REG_OFFSET, BUTTON_UIO_GIER_MASK);

	return BUTTON_UIO_SUCCESS;
}

// write to a register of the UIO device
// offset : the offset of the register that we are writing to
// value : the value that we want to write to the register
void button_uio_write(uint32_t offset, uint32_t value) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + offset)) = value;
}

// Acknowledge interrupt(s) in the interrupt controller
// value: Bitmask of interrupt lines to acknowledge.
void button_uio_acknowledge(uint32_t value) {
	*((volatile uint32_t *)(va + BUTTON_UIO_IP_ISR_REG_OFFSET)) = value;
}

// read from a register of the UIO device
// offset : the offset of the register we wich to read from
// returns : the value contained within the register we are reading from
uint32_t button_uio_read(uint32_t offset) {
	return *((volatile uint32_t *)(va + offset));
}

// Called to exit the driver (unmap and close UIO file)
void button_uio_exit() {
	munmap(va, BUTTON_UIO_MMAP_SIZE);
	close(fd);
}
