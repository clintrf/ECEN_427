/*
* Interrupt Driver
* initializes interrupts
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "intc.h"

#define INTC_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define FOUR_BYTES 4 /* 32 bits to write to fd */
#define IER_REG_OFFSET 0x08h /* IER register (unmasks corresponding ISR bit) */
#define ENABLE 1
#define DISABLE 0

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the interrupt handler registers */
static int enable = 1; /* enable code for interrupt */
static int disable = 0; /* disable code for interrupt */

/**************************** functions *****************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t intc_init(char devDevice[]){
	/* open the device */
	fd = open(devDevice, O_RDWR);
	if(fd == INTC_ERROR) {
		return INTC_ERROR;
	}

	va = mmap(NULL, INTC_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET);
	if(fd == MAP_FAILED) {
		return INTC_ERROR;
	}

	return INTC_SUCCESS;
}

// Called to exit the driver (unmap and close UIO file)
void intc_exit() {
	munmap(ptr, UIO_BTN_MMAP_SIZE);
	close(fd);
}

// This function will block until an interrupt occurrs
// Returns: Bitmask of activated interrupts
uint32_t intc_wait_for_interrupt(){return 0;};

// Acknowledge interrupt(s) in the interrupt controller
// irq_mask: Bitmask of interrupt lines to acknowledge.
void intc_ack_interrupt(uint32_t irq_mask);

// Instruct the UIO to enable interrupts for this device in Linux
// (see the UIO documentation for how to do this)
void intc_enable_uio_interrupts() {
	write(fd, &enable, FOUR_BYTES); /* enable linux interrupts from the GPIO */
	interrupt_register_write(IER_REG_OFFSET, ENABLE); /* enable interrupts from the interrupt handler */
	/* enable interrupts in each individual GPIO */
}

// write to a register of the UIO device 4
void interrupt_register_write(uint32_t offset, uint32_t value) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + offset)) = value;
}

// Enable interrupt line(s)
// irq_mask: Bitmask of lines to enable
// This function only enables interrupt lines, ie, a 0 bit in irq_mask
//	will not disable the interrupt line
void intc_irq_enable(uint32_t irq_mask);

// Same as intc_irq_enable, except this disables interrupt lines
void intc_irq_disable(uint32_t irq_mask);
