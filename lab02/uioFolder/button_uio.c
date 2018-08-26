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
#include "button_uio.h"

/*********************************** macros ***********************************/
#define UIO_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define GIER_REG_OFFSET 0x11C /* global interrupt register offset */
#define GIER_MASK (1<<31) /* top register bit (31) is set to one */
#define IP_IER_REG_OFFSET 0x128 /* IP IER offset value */
#define CHANNEL_ONE_MASK 0x1 /* enables Channel 1 interrupts from selected device */
#define IP_ISR_REG_OFFSET 0x120 /* IP ISR register offset */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */


/**************************** functions *****************************/
/* initializes the uio driver */
int32_t button_init(char devDevice[]) {
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
	button_write(IP_IER_REG_OFFSET, CHANNEL_ONE_MASK);
	/* enable global interrupt by setting bit 31 of the GIR */
	button_write(GIER_REG_OFFSET, GIER_MASK);

	return UIO_SUCCESS;
}

// write to a register of the UIO device
void button_write(uint32_t offset, uint32_t value) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + offset)) = value;
}

void button_acknowledge(uint32_t value) {
	*((volatile uint32_t *)(va + IP_ISR_REG_OFFSET)) = value;
}

// read from a register of the UIO device
uint32_t button_read(uint32_t offset) {
	return *((volatile uint32_t *)(va + offset));
}

/* close the UIO device */
void button_exit() {
	munmap(va, UIO_MMAP_SIZE);
	close(fd);
}
