/*
* PIT Driver
*
*
* ECEn 427
* Clint Frandsen, Dax Eckles, Seth Becerra
* BYU 2019
*/
/*our driver will create a set of attributes, that are represented as files in this virtual filesystem.
Userspace can read and write ASCII text to these files to read/write attributes in your driver. This interface to your device
is nice for users,  as they can interact with your device by simply using cat and echo through the terminal,
without needing to write a program. */
#include <stdint.h>
#include <stdio.h>
#include "pit_driver.h"

// MACROS-- #defines



// Functions

//Function prototypes
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t pit_driver_init(char devDevice[])
{

}

// Called to exit the driver (unmap and close UIO file)
void pit_driver_exit()
{

}

// Get/set timer period
int32_t get_timer_period()
{

}

void set_timer_period()
{

}

// Start/stop timer
void start_stop_timer()
{

}

// Turn timer interrupts on/off
void timer_interrupt_on_off()
{

}
