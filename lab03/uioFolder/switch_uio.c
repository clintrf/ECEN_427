/*
* Switch Driver
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "switch_uio.h"

/*********************************** macros ***********************************/
#define SWITCH_UIO_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define SWITCH_UIO_GIER_REG_OFFSET 0x11C /* global interrupt register offset */
#define SWITCH_UIO_GIER_MASK (1<<31) /* top register bit (31) is set to one */
#define SWITCH_UIO_IP_IER_REG_OFFSET 0x128 /* IP IER offset value */
#define SWITCH_UIO_IP_ISR_REG_OFFSET 0x120 /* IP ISR register offset */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */


/**************************** functions *****************************/
// initializes the uio driver
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, success code otherwise
int32_t switch_uio_init(char devDevice[]) {
	/* open the device */
	fd = open(devDevice, O_RDWR);
	/* if there is a problem, return an error */
	if(fd == SWITCH_UIO_ERROR) {
		return SWITCH_UIO_ERROR;
	}

	va = mmap(NULL, SWITCH_UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, SWITCH_UIO_MMAP_OFFSET);
	/* if there is a problem, return an error */
	if(va == MAP_FAILED) {
		return SWITCH_UIO_ERROR;
	}

	/* put hardware setup here */
	/* enable channel interrupt in the IP IER */
	switch_uio_write(SWITCH_UIO_IP_IER_REG_OFFSET, SWITCH_UIO_CHANNEL_ONE_MASK);
	/* enable global interrupt by setting bit 31 of the GIR */
	switch_uio_write(SWITCH_UIO_GIER_REG_OFFSET, SWITCH_UIO_GIER_MASK);

	return SWITCH_UIO_SUCCESS;
}

// write to a register of the UIO device
// offset : the offset of the register that we are writing to
// value : the value that we want to write to the register
void switch_uio_write(uint32_t offset, uint32_t value) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + offset)) = value;
}

// Acknowledge interrupt(s) in the interrupt controller
// value: Bitmask of interrupt lines to acknowledge.
void switch_uio_acknowledge(uint32_t value) {
	*((volatile uint32_t *)(va + SWITCH_UIO_IP_ISR_REG_OFFSET)) = value;
}

// read from a register of the UIO device
// offset : the offset of the register we wich to read from
// returns : the value contained within the register we are reading from
uint32_t switch_uio_read(uint32_t offset) {
	return *((volatile uint32_t *)(va + offset));
}

// Called to exit the driver (unmap and close UIO file)
void switch_uio_exit() {
	munmap(va, SWITCH_UIO_MMAP_SIZE);
	close(fd);
}
