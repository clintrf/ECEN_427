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
#include <sys/mman.h>
#include "btn_uio.h"

#define UIO_BTN_MMAP_SIZE 0x1000 /* size of memory to allocate */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the button registers */


/**************************** functions *****************************/
/* initializes the uio driver */
int32_t btn_init(char devDevice[]) {
	/* open the device */
	fd = open(devDevice, O_RDWR);
	if(f == UIO_BTN_ERROR) {
		return UIO_BTN_ERROR;
	}

	va = mmap(NULL, UIO_BTN_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
	if(fd == MAP_FAILED) {
		return UIO_EXAMPLE_ERROR;
	}

	/* put hardware setup here */

	return UIO_BTN_SUCCESS;
}

// write to a register of the UIO device
void btn_write(uint32_t offset, uint32_t value);

// read from a register of the UIO device
uint32_t btn_read(uint32_t offset) {
	return *((volatile uint32_t *)(ptr + offset));
}

/* close the UIO device */
void btn_exit() {
	munmap(ptr, UIO_BTN_MMAP_SIZE);
	close(fd);
}
