/*
* audio_driver.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>

/*********************************** macros ***********************************/
#define AUDIO_DRIVER_MMAP_OFFSET   0
#define AUDIO_DRIVER_SUCCESS  0
#define AUDIO_DRIVER_ERROR   -1      //error return value
#define AUDIO_DRIVER_GPIO_FILE_PATH "/dev/audio"

/**************************** function prototypes *****************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t audio_driver_init(char devDevice[]);

// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit();

// Called to write to the audio driver
void audio_driver_write();

// Called to read to the audio driver
void audio_driver_read();
