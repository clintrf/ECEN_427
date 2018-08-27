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
#include <string.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <err.h>
#include "audio_driver.h"
#include "../i2cps/i2cps.h"

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
      printf("Data at Index %zu: %zu\n",i,sampleBuf[i]);
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

/******************************************************************************
 * Function to write 8 bits to one of the registers from the audio
 * controller.
 * @param   u8RegAddr is the register address.
 * @param   u8Data is the data byte to write.
 * @param   iic_fd is the file descriptor for /dev/i2c-x
 * @return  none.
 *****************************************************************************/
void write_audio_reg(unsigned char u8RegAddr,
                     unsigned char u8Data, int iic_fd) {
    unsigned char u8TxData[3];
    u8TxData[0] = 0x40;
    u8TxData[1] = u8RegAddr;
    u8TxData[2] = u8Data;
    if (writeI2C_asFile(iic_fd, u8TxData, 3) < 0){
        printf("Unable to write audio register.\n");
    }
}

/******************************************************************************
 * Function to configure the audio PLL.
 * @param   iic_index is the i2c index in /dev list.
 * @return  none.
 *****************************************************************************/
void config_audio_pll(int iic_index) {
    printf("Configure Audio PLL...\n");
    unsigned char u8TxData[8], u8RxData[6];
    int iic_fd;
    iic_fd = setI2C(iic_index, IIC_SLAVE_ADDR);
    if (iic_fd < 0) {
        printf("Unable to set I2C %d.\n", iic_index);
    }

    // Disable Core Clock
    write_audio_reg(R0_CLOCK_CONTROL, 0x0E, iic_fd);
    /*  MCLK = 10 MHz
     *  R = 0100 = 4, N = 0x023C = 572, M = 0x0271 = 625
     *  PLL required output = 1024x48 KHz = 49.152 MHz
     *  PLLout/MCLK         = 49.152 MHz/10 MHz = 4.9152 MHz
     *                      = R + (N/M)
     *                      = 4 + (572/625)
     */
    // Register write address [15:8]
    u8TxData[0] = 0x40;
    // Register write address [7:0]
    u8TxData[1] = 0x02;
    // byte 6 - M[15:8]
    u8TxData[2] = 0x02;
    // byte 5 - M[7:0]
    u8TxData[3] = 0x71;
    // byte 4 - N[15:8]
    u8TxData[4] = 0x02;
    // byte 3 - N[7:0]
    u8TxData[5] = 0x3C;
    // byte 2 - bits 6:3 = R[3:0], 2:1 = X[1:0], 0 = PLL operation mode
    u8TxData[6] = 0x21;
    // byte 1 - 1 = PLL Lock, 0 = Core clock enable
    u8TxData[7] = 0x03;
    // Write bytes to PLL control register R1 at 0x4002
    if (writeI2C_asFile(iic_fd, u8TxData, 8) < 0){
        printf("Unable to write I2C %d.\n", iic_index);
    }

    // Poll PLL Lock bit
    u8TxData[0] = 0x40;
    u8TxData[1] = 0x02;
    do {
        if (writeI2C_asFile(iic_fd, u8TxData, 2) < 0){
            printf("Unable to write I2C %d.\n", iic_index);
        }
        if (readI2C_asFile(iic_fd, u8RxData, 6) < 0){
            printf("Unable to read I2C %d.\n", iic_index);
        }
    } while((u8RxData[5] & 0x02) == 0);

    /* Clock control register:  bit 3        CLKSRC = PLL Clock input
     *                          bit 2:1      INFREQ = 1024 x fs
     *                          bit 0        COREN = Core Clock enabled
     */
    write_audio_reg(R0_CLOCK_CONTROL, 0x0F, iic_fd);

    if (unsetI2C(iic_fd) < 0) {
        printf("Unable to unset I2C %d.\n", iic_index);
    }
}

/******************************************************************************
 * Function to configure the audio codec.
 * @param   iic_index is the i2c index in /dev list.
 * @return  none.
 *****************************************************************************/
void config_audio_codec(int iic_index) {
    printf("Configure Audio Codec...\n");
    int iic_fd;
    iic_fd = setI2C(iic_index, IIC_SLAVE_ADDR);
    if (iic_fd < 0) {
        printf("Unable to set I2C %d.\n", iic_index);
    }

    /*
     * Input path control registers are configured
     * in select_mic and select_line_in
     */

    // Mute Mixer1 and Mixer2 here, enable when MIC and Line In used
    write_audio_reg(R4_RECORD_MIXER_LEFT_CONTROL_0, 0x00, iic_fd);
    write_audio_reg(R6_RECORD_MIXER_RIGHT_CONTROL_0, 0x00, iic_fd);
    // Set LDVOL and RDVOL to 21 dB and Enable left and right differential
    write_audio_reg(R8_LEFT_DIFFERENTIAL_INPUT_VOLUME_CONTROL, 0xB3, iic_fd);
    write_audio_reg(R9_RIGHT_DIFFERENTIAL_INPUT_VOLUME_CONTROL, 0xB3, iic_fd);
    // Enable MIC bias
    write_audio_reg(R10_RECORD_MICROPHONE_BIAS_CONTROL, 0x01, iic_fd);
    // Enable ALC control and noise gate
    write_audio_reg(R14_ALC_CONTROL_3, 0x20, iic_fd);
    // Put CODEC in Master mode
    write_audio_reg(R15_SERIAL_PORT_CONTROL_0, 0x01, iic_fd);
    // Enable ADC on both channels, normal polarity and ADC high-pass filter
    write_audio_reg(R19_ADC_CONTROL, 0x33, iic_fd);
    // Mute play back Mixer3 and Mixer4 and enable when output is required
    write_audio_reg(R22_PLAYBACK_MIXER_LEFT_CONTROL_0, 0x00, iic_fd);
    write_audio_reg(R24_PLAYBACK_MIXER_RIGHT_CONTROL_0, 0x00, iic_fd);
    // Mute left input to mixer3 (R23) and right input to mixer4 (R25)
    write_audio_reg(R23_PLAYBACK_MIXER_LEFT_CONTROL_1, 0x00, iic_fd);
    write_audio_reg(R25_PLAYBACK_MIXER_RIGHT_CONTROL_1, 0x00, iic_fd);
    // Mute left and right channels output; enable them when output is needed
    write_audio_reg(R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, 0xE5, iic_fd);
    write_audio_reg(R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, 0xE5, iic_fd);
    // Enable play back right and left channels
    write_audio_reg(R35_PLAYBACK_POWER_MANAGEMENT, 0x03, iic_fd);
    // Enable DAC for both channels
    write_audio_reg(R36_DAC_CONTROL_0, 0x03, iic_fd);
    // Set SDATA_In to DAC
    write_audio_reg(R58_SERIAL_INPUT_ROUTE_CONTROL, 0x01, iic_fd);
    // Set SDATA_Out to ADC
    write_audio_reg(R59_SERIAL_OUTPUT_ROUTE_CONTROL, 0x01, iic_fd);
    // Enable DSP and DSP Run
    write_audio_reg(R61_DSP_ENABLE, 0x01, iic_fd);
    write_audio_reg(R62_DSP_RUN, 0x01, iic_fd);
    /*
     * Enable Digital Clock Generator 0 and 1.
     * Generator 0 generates sample rates for the ADCs, DACs, and DSP.
     * Generator 1 generates BCLK and LRCLK for the serial port.
     */
    write_audio_reg(R65_CLOCK_ENABLE_0, 0x7F, iic_fd);
    write_audio_reg(R66_CLOCK_ENABLE_1, 0x03, iic_fd);

    if (unsetI2C(iic_fd) < 0) {
        printf("Unable to unset I2C %d.\n", iic_index);
    }
}