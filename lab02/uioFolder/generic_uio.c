/*
* Button Driver
*
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "generic_uio.h"

#define UIO_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define GIER_REG_OFFSET 0x11C /* global interrupt register offset */
#define GIER_MASK 0x8000 /* top register bit (31) is set to one */
#define IP_IER_REG_OFFSET 0x128 /* IP IER offset value */
#define IP_IER_MASK 0x3 /* enables Channel 1 interrupts from selected device */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */


/**************************** functions *****************************/
/* initializes the uio driver */
int32_t generic_init(char devDevice[]) {
	/* open the device */
	fd = open(devDevice, O_RDWR);
	if(fd == UIO_ERROR) {
		return UIO_ERROR;
	}

	va = mmap(NULL, UIO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
	if(va == MAP_FAILED) {
		return UIO_ERROR;
	}

	/* put hardware setup here */
	/* enable channel interrupt in the IP IER */
	generic_write(IP_IER_REG_OFFSET, IP_IER_MASK);
	/* enable global interrupt by setting bit 31 of the GIR */
	generic_write(GIER_REG_OFFSET, GIER_MASK);

	return UIO_SUCCESS;
}

// write to a register of the UIO device
void generic_write(uint32_t offset, uint32_t value) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + offset)) = value;
}

// read from a register of the UIO device
uint32_t generic_read(uint32_t offset) {
	return *((volatile uint32_t *)(va + offset));
}

/* close the UIO device */
void generic_exit() {
	munmap(va, UIO_MMAP_SIZE);
	close(fd);
}
