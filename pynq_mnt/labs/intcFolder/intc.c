/*
* Interrupt Driver
* Initializes interrupts
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
#define SIE_REG_OFFSET 0x10 /* sets the register bits in the IER */
#define CIE_REG_OFFSET 0x14 /* clears the resgiter bits in the IER */
#define IAR_REG_OFFSET 0xC /* acknowledges interrupts */
#define GIER_REG_OFFSET 0x11C /* global interrupt register offset */
#define GPIO_BITS 7 /* turns on all GPIO interrupts */
#define GIER_MASK 0x8000 /* top register bit (31) is set to one */

/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the interrupt handler registers */
static int enable = 1; /* enable code for interrupt */
static int32_t int_buffer; /* buffer for the interrupt */

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
	if(va == MAP_FAILED) {
		return INTC_ERROR;
	}

	intc_enable_global_interrupts(); /*enables global interrupts */
	intc_irq_enable(GPIO_BITS); /* enables all the GPIO interrupts */

	return INTC_SUCCESS;
}

// Called to exit the driver (unmap and close UIO file)
void intc_exit() {
	munmap(va, INTC_MMAP_SIZE);
	close(fd);
}

// This function will block until an interrupt occurrs
// Returns: Bitmask of activated interrupts
uint32_t intc_wait_for_interrupt() {
	return read(fd, &int_buffer, FOUR_BYTES);
}

// Acknowledge interrupt(s) in the interrupt controller
// irq_mask: Bitmask of interrupt lines to acknowledge.
void intc_ack_interrupt(uint32_t irq_mask) {
	*((volatile uint32_t *)(va + IAR_REG_OFFSET)) = irq_mask;
}

// Instruct the UIO to enable interrupts for this device in Linux
// (see the UIO documentation for how to do this)
void intc_enable_uio_interrupts() {
	write(fd, &enable, FOUR_BYTES); /* enable linux interrupts from the GPIO */
}

// Enables global interrupts in the GPIO
void intc_enable_global_interrupts(){
	*((volatile uint32_t *)(va + GIER_REG_OFFSET)) | GIER_MASK;
}

// Enable interrupt line(s)
// irq_mask: Bitmask of lines to enable
// This function only enables interrupt lines, ie, a 0 bit in irq_mask
//	will not disable the interrupt line
void intc_irq_enable(uint32_t irq_mask) {
	//the address is cast as a pointer so it can be dereferenced
	*((volatile uint32_t *)(va + SIE_REG_OFFSET)) = irq_mask;
}

// Same as intc_irq_enable, except this disables interrupt lines
void intc_irq_disable(uint32_t irq_mask) {
	*((volatile uint32_t *)(va + CIE_REG_OFFSET)) = irq_mask;
}
