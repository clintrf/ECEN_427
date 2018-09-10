/*
* Interrupt Driver
* Initializes interrupts
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "intc.h"

/*********************************** macros ***********************************/
#define INTC_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define FOUR_BYTES_SIZE 4 /* 32 bits to write to fd */
#define SIE_REG_OFFSET 0x10 /* sets the register bits in the IER */
#define CIE_REG_OFFSET 0x14 /* clears the resgiter bits in the IER */
#define IAR_REG_OFFSET 0xC /* acknowledges interrupts */
#define MER_REG_OFFSET 0x1C /* Master Enable Register */
#define ISR_REG_OFFSET 0x0 /* ISR offset */
#define GPIO_BITS 0x7 /* turns on all GPIO interrupts */
#define MER_BITS 0x3 /* need to turn on lower two bits to enable interrupts */


/*********************************** globals ***********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the interrupt handler registers */
static int enable = 1; /* enable code for interrupt */
static int32_t int_buffer = 0; /* buffer for the interrupt */

/**************************** functions *****************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t intc_init(char devDevice[]){
	/* open the device */
	fd = open(devDevice, O_RDWR);
	/* if there is a problem, return an error */
	if(fd == INTC_ERROR) {
		return INTC_ERROR;
	}

	/* map the virtual address to the appropriate location on the pynq */
	va = mmap(NULL, INTC_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, INTC_MMAP_OFFSET);
	/* if there's a problem, return an error */
	if(va == MAP_FAILED) {
		return INTC_ERROR;
	}

	intc_irq_enable(GPIO_BITS); /* enables all the GPIO interrupts */
	/* turns on Master IRQ enable & Hardware interrupt enable */
	*((volatile uint32_t *)(va + MER_REG_OFFSET)) = MER_BITS;

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
	read(fd, &int_buffer, FOUR_BYTES_SIZE); /* blocks interrupts */
	return *((volatile uint32_t *)(va + ISR_REG_OFFSET));
}

// Acknowledge interrupt(s) in the interrupt controller
// irq_mask: Bitmask of interrupt lines to acknowledge.
void intc_ack_interrupt(uint32_t irq_mask) {
	*((volatile uint32_t *)(va + IAR_REG_OFFSET)) = irq_mask;
}

// Instruct the UIO to enable interrupts for this device in Linux
// (see the UIO documentation for how to do this)
void intc_enable_uio_interrupts() {
	write(fd, &enable, FOUR_BYTES_SIZE); /* enable linux interrupts from the GPIO */
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
