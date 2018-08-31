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
#include <stdlib.h>
#include "audio_driver.h"
#include <err.h>

/*********************************** macros **********************************/
#define END_OF_READ_FILE 0
#define SOUND_FILE_HOME "/home/xilinx/ECEN_427/lab04/wavFiles" // location of all our sound folder
#define INVADER_DIE_AUDIO SOUND_FILE_HOME "/invader_die.wav"
#define LASER_AUDIO SOUND_FILE_HOME "/laser.wav"
#define PLAYER_DIE_AUDIO SOUND_FILE_HOME "/player_die.wav"
#define UFO_AUDIO SOUND_FILE_HOME "/ufo.wav"
#define UFO_DIE_AUDIO SOUND_FILE_HOME "/ufo_die.wav"
#define WALK1_AUDIO SOUND_FILE_HOME "/walk1.wav"
#define WALK2_AUDIO SOUND_FILE_HOME "/walk2.wav"
#define WALK3_AUDIO SOUND_FILE_HOME "/walk3.wav"
#define WALK4_AUDIO SOUND_FILE_HOME "/walk4.wav"


/********************************** structs **********************************/
// struct containing the header and data of audio
typedef struct audio_data_file {
  uint32_t size;
  uint32_t * data;
} audio_data;

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static uint16_t off = 0;
audio_data  data_array[9];



/******************************** prototypes *********************************/
void audio_driver_import_audio(char fileName[], uint16_t index);

/********************************* functions *********************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t audio_driver_init(char devDevice[]) {
  printf("Initializing Audio Driver\n");
  /* open the device */
  fd = open(devDevice, O_RDWR);
  /* if there is a problem, return an error */
  if(fd == AUDIO_DRIVER_ERROR) {
    printf("Audio Driver Error, file cannot open.\n");
    return AUDIO_DRIVER_ERROR;
  }

  audio_driver_import_audio(LASER_AUDIO, 0);
  //audio_driver_import_audio(LASER_AUDIO, 1);
  //audio_driver_import_audio(PLAYER_DIE_AUDIO);
  //audio_driver_import_audio(UFO_AUDIO);
  //audio_driver_import_audio(UFO_DIE_AUDIO);
  //audio_driver_import_audio(WALK1_AUDIO);
  //audio_driver_import_audio(WALK2_AUDIO);
  //audio_driver_import_audio(WALK3_AUDIO);
  //audio_driver_import_audio(WALK4_AUDIO);

  return AUDIO_DRIVER_SUCCESS;
}

void audio_driver_import_audio(char fileName[], uint16_t index) {
  printf("Print Audio Function\n");
  size_t sizeOfFile = 0;
  char* rawbuffer = 0;
  unsigned short int* sampleBuf = 0;
  size_t bytesRead = 0;

  if (!fileName) { errx(1, "Filename not specified"); }                         // Error checking for getting file name
  else {                                                                        // prints the file path
    printf("File Name: ");                                                      // Print out the file name
    for(int i = 0; i < 52; i++) {                                               // ^^^^^^^^^^^^^^^^^^^^^^^
      printf("%c",fileName[i]);                                                 // ^^^^^^^^^^^^^^^^^^^^^^^
    }                                                                           // ^^^^^^^^^^^^^^^^^^^^^^^
    printf("\r\n");                                                             // ^^^^^^^^^^^^^^^^^^^^^^^
  }

  FILE* fp = fopen(fileName, "r");                                              // Open the file given
  if (!fp) { errx(1, "Filename not opened"); }                                  // Error check if File was correctly oppenned

  fseek(fp, 0L, SEEK_END);                                                      // Run though the entire file
  sizeOfFile = (size_t)ftell(fp);                                               // Save how many bytes thf fp is and use as size of file
  rewind(fp);                                                                   // return to to top of file

  rawbuffer = (char*)malloc(sizeOfFile);                                        // Alocate memory based on size of file
  if (!rawbuffer){ errx(1, "Memory not allocated"); }                           // Error check if memory is alocated if not dealocate memory

  bytesRead = fread((void*)rawbuffer, 1, sizeOfFile, fp);                       // Write to raw buffer
  if(bytesRead != sizeOfFile){ errx(1, "Did not read entire file"); }           // Error check if buffer was written to
  sampleBuf = (unsigned short int*)rawbuffer;                                   // Save copy of Raw buffer
  audio_data data;
  data.size = bytesRead / sizeof(unsigned short int);
  //uint32_t * data;
  for (size_t i = 0; i < data.size; i++) {  // Loop through data and
      data.data[i] = sampleBuf[i];
      printf("Data at Index %zu: %zu\n",i,data);
  }
  data_array[index] = data;
}
// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit() {
  close(fd);
}

// Called to write to the audio driver
// len : amount of bytes to write to the driver
// buf : the buffer to be passed into the kernel (contains audio data)
 void audio_driver_write(const char *buf, int32_t len) {
   if(buf == NULL) {
     return;
   }
  write(fd,buf,len);
}

// Called to read to the audio driver
// len : the amount of bytes to read into the buffer
// returns a value with the type of success pending
int16_t audio_driver_read(int32_t len) {
  char rdbuf[len];
  int32_t count = read(fd,&rdbuf,len);
  if(count == len) { // optimal case success
    return AUDIO_DRIVER_READ_OPTIMAL_SUCCESS;
  }
  else if(count > END_OF_READ_FILE) { // read parts of the data, but not all
    return AUDIO_DRIVER_PARTIAL_DATA_TRANSFER;
  }
  else if(count == END_OF_READ_FILE) { // reached EOF before any data was read
    return AUDIO_DRIVER_REACHED_EOF;
  }
  else { // some kind of error occured if the number is negative
    return AUDIO_DRIVER_READ_ERROR;
  }
}
