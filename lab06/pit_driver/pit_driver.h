/*
* pit_driver.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles, Seth Becerra
* BYU 2019
*/
/*Instructions from lab06 writeup*/
/*
* Create a basic kernel driver for your PIT, using your audio driver code for reference.
* You don't need to create read()/write() functions for the driver, as userspace will interact using sysfs.
* Load your kernel module and make sure it probes correctly for your PIT.
*/


//Function prototypes
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t pit_driver_init(char devDevice[]);

// Called to exit the driver (unmap and close UIO file)
void pit_driver_exit();

// Get/set timer period
int32_t get_timer_period();

void set_timer_period();

// Start/stop timer
void start_stop_timer();

// Turn timer interrupts on/off
void timer_interrupt_on_off();
