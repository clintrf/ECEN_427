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
#include "audio_driver.h"
#include <err.h>

/*********************************** macros **********************************/
#define AUDIO_DRIVER_MMAP_SIZE 0x1000 /* size of memory to allocate */
#define END_OF_READ_FILE 0
#define SOUND_FILE_HOME
#define INVADER_DIE_AUDIO SOUND_FILE_HOME+"/invader_die.wav"
#define LASER_AUDIO SOUND_FILE_HOME+"/laser.wav"
#define PLAYER_DIE_AUDIO SOUND_FILE_HOME+"/player_die.wav"
#define UFO_AUDIO SOUND_FILE_HOME+"/ufo.wav"
#define UFO_DIE_AUDIO SOUND_FILE_HOME+"/ufo_die.wav"
#define WALK1_AUDIO SOUND_FILE_HOME+"/walk1.wav"
#define WALK2_AUDIO SOUND_FILE_HOME+"/walk2.wav"
#define WALK3_AUDIO SOUND_FILE_HOME+"/walk3.wav"
#define WALK4_AUDIO SOUND_FILE_HOME+"/walk4.wav"

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static char *va; /* virtual address of the audio driver */
static uint16_t off = 0;
uint32_t data_array[9];




/******************************** prototypes *********************************/
void audio_driver_import_audio(char fileName[]);

/********************************* functions *********************************/
// Initializes the driver (opens UIO file and calls mmap)
// devDevice: The file path to the uio dev file
// Returns: A negative error code on error, INTC_SUCCESS otherwise
// This must be called before calling any other intc_* functions
int32_t audio_driver_init(char devDevice[]) {
  /* open the device */
  fd = open(devDevice, O_RDWR);
  /* if there is a problem, return an error */
  if(fd == AUDIO_DRIVER_ERROR) {
    return AUDIO_DRIVER_ERROR;
  }
  /* map the virtual address to the appropriate location on the pynq */
  va = mmap(NULL, AUDIO_DRIVER_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, AUDIO_DRIVER_MMAP_OFFSET);
  /* if there's a problem, return an error */
  if(va == MAP_FAILED) {
    return AUDIO_DRIVER_ERROR;
  }

  audio_driver_import_audio(INVADER_DIE_AUDIO , 0);
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
  size_t sizeOfFile = 0;
  char* rawbuffer = 0;
  unsigned short int* sampleBuf = 0;
  size_t bytesRead = 0;

  if (!fileName)
    errx(1, "Filename not specified");

  FILE* fp = fopen(fileName, "r");
  if (!fp)
    errx(1, "Filename not opened");

  fseek(fp, 0L, SEEK_END);
  sizeOfFile = (size_t)ftell(fp);
  rewind(fp);

  rawbuffer = (char*)malloc(sizeOfFile);
  if (!rawbuffer){
    errx(1, "Memory not allocated");
  }
  bytesRead = fread((void*)rawbuffer, 1, sizeOfFile, fp); /* Need error checking here also. */
  if(bytesRead != sizeOfFile){
    errx(1, "Did not read entire file");
  }
  sampleBuf = (unsigned short int*)rawbuffer;

  for (size_t i = 0, j = bytesRead / sizeof(unsigned short int); i < j; i++) {
      uint32_t data; = sampleBuf[i];
  }
  data_array[index] = data;

}

// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit() {
  munmap(va, AUDIO_DRIVER_MMAP_SIZE);
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
  int32_t count = read(fd,&buf,len);
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
