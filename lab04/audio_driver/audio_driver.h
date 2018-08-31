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
#define AUDIO_DRIVER_READ_OPTIMAL_SUCCESS 1
#define AUDIO_DRIVER_PARTIAL_DATA_TRANSFER 2
#define AUDIO_DRIVER_REACHED_EOF 3
#define AUDIO_DRIVER_READ_ERROR -1

/**************************** function prototypes *****************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t audio_driver_init(char devDevice[]);

// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit();

// Called to write to the audio driver
// len : amount of bytes to write to the driver
// buf : the buffer to be passed into the kernel (contains audio data)
void audio_driver_write(const char *buf, int32_t len);

// Called to read to the audio driver
// len : the amount of bytes to read into the buffer
// returns a value with the type of success pending
int16_t audio_driver_read(int32_t len);
