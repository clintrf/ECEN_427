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
#define SOUND_FILE_HOME "/home/xilinx/ECEN_427/lab04/wavFiles"
#define INVADER_DIE_AUDIO SOUND_FILE_HOME "/invader_die.wav"
#define LASER_AUDIO SOUND_FILE_HOME "/laser.wav"
#define PLAYER_DIE_AUDIO SOUND_FILE_HOME "/player_die.wav"
#define UFO_AUDIO SOUND_FILE_HOME "/ufo.wav"
#define UFO_DIE_AUDIO SOUND_FILE_HOME "/ufo_die.wav"
#define WALK1_AUDIO SOUND_FILE_HOME "/walk1.wav"
#define WALK2_AUDIO SOUND_FILE_HOME "/walk2.wav"
#define WALK3_AUDIO SOUND_FILE_HOME "/walk3.wav"
#define WALK4_AUDIO SOUND_FILE_HOME "/walk4.wav"
#define BITS_PER_BYTE 8
#define SUCCESSFUL_CLOSE 0
#define ERR 1
#define FILE_NAME_LENGTH 52
#define TWO_CHARACTERS 2
#define FOUR_CHARACTERS 4
#define READ_ONE_INDEX_POS 1
#define INDEX_ZERO 0
#define INDEX_ONE 1
#define INDEX_TWO 2
#define INDEX_THREE 3
#define BYTES_PER_KILOBYTE 1024
#define FORMAT_NAME_LENGTH 10
#define PCM_FORMAT 1
#define A_LAW_FORMAT 6
#define MU_LAW_FORMAT 7

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static uint16_t off = 0;
// audio_data_header sound_data_array[AUDIO_DRIVER_NUM_SAMPLE_FILES];


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
  audio_driver_import_audio(INVADER_DIE_AUDIO,0);
  printf("INVADER_DIE_AUDIO imported\n\n\r");
  audio_driver_import_audio(LASER_AUDIO,1);
  printf("LASER_AUDIO imported\n\n\r");
  audio_driver_import_audio(PLAYER_DIE_AUDIO,2);
  printf("PLAYER_DIE_AUDIO imported\n\n\r");
  audio_driver_import_audio(UFO_AUDIO,3);
  printf("UFO_AUDIO imported\n\n\r");
  audio_driver_import_audio(UFO_DIE_AUDIO,4);
  printf("UFO_DIE_AUDIO imported\n\n\r");
  audio_driver_import_audio(WALK1_AUDIO,5);
  printf("WALK1_AUDIO imported\n\n\r");
  audio_driver_import_audio(WALK2_AUDIO,6);
  printf("WALK2_AUDIO imported\n\n\r");
  audio_driver_import_audio(WALK3_AUDIO,7);
  printf("WALK3_AUDIO imported\n\n\r");
  audio_driver_import_audio(WALK4_AUDIO,8);
  printf("WALK4_AUDIO imported\n\n\r");
  return AUDIO_DRIVER_SUCCESS;
}

// This goes through one audio file, converts it, and puts it in the data_array
// fileName : the name of the file to import
// index : the index within data_array to place the imported and converted file
void audio_driver_import_audio(char fileName[], uint16_t index) {
  printf("Print Audio Function\n");
  /* Error checking for getting file name */
  if (!fileName) { errx(ERR, "Filename not specified"); }
  /* Open the file given */
  FILE* fp = fopen(fileName, "r");
  /* Error check if File was correctly opened */
  if (!fp) { errx(ERR, "Filename not opened"); }
  /* values used to grab information from the WAV files */
  int read = 0; // use of int because fread returns int type
  char buffer4[FOUR_CHARACTERS]; // used to grab information of 4 bytes length
  char buffer2[TWO_CHARACTERS]; // used to grab information of 2 bytes length

  /*** read the header parts of .wav fileName ***/
  // grab the character array "riff"
  read = fread(sound_data_array[index].riff,
    sizeof(sound_data_array[index].riff),READ_ONE_INDEX_POS,fp);
  printf("(1-4) RIFF marker: %s \n", sound_data_array[index].riff);
  // grab the Chunk Size (grabs in little Endian)
  read = fread(buffer4,sizeof(buffer4),READ_ONE_INDEX_POS,fp);
  // shift to big endian
  sound_data_array[index].overall_size = buffer4[INDEX_ZERO] |
                                        (buffer4[INDEX_ONE]<<PCM_8_SHIFT) |
                                        (buffer4[INDEX_TWO]<<PCM_16_SHIFT) |
                                        (buffer4[INDEX_THREE]<<PCM_24_SHIFT);
  // print chunk size
  printf("(5-8) Overall size: bytes:%u, Kb:%u \n",
    sound_data_array[index].overall_size,
    sound_data_array[index].overall_size/BYTES_PER_KILOBYTE);
  // grab wave formatting marker (should read WAVE)
  read=fread(sound_data_array[index].wave,sizeof(sound_data_array[index].wave),
    READ_ONE_INDEX_POS,fp);
  printf("(9-12) Wave marker: %s\n", sound_data_array[index].wave);
  // grab sub chunk id marker (should read fmt)
  read=fread(sound_data_array[index].fmt_chunk_marker,
    sizeof(sound_data_array[index].fmt_chunk_marker),READ_ONE_INDEX_POS,fp);
  printf("(13-16) Fmt marker: %s\n", sound_data_array[index].fmt_chunk_marker);
  // grab sub chunk 1 size (should be 16)
  read = fread(buffer4,sizeof(buffer4),READ_ONE_INDEX_POS,fp);
  // convert little endian to big endian 4 byte integer
  sound_data_array[index].length_of_fmt = buffer4[INDEX_ZERO] |
                                        (buffer4[INDEX_ONE]<<PCM_8_SHIFT) |
                                        (buffer4[INDEX_TWO]<<PCM_16_SHIFT) |
                                        (buffer4[INDEX_THREE]<<PCM_24_SHIFT);
  // print subchunk1size
  printf("(17-20) Length of Fmt header: %u \n",
    sound_data_array[index].length_of_fmt);
  // read in audio format, should be 1 for PCM
  read = fread(buffer2,sizeof(buffer2),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].format_type = buffer2[INDEX_ZERO] |
                                        (buffer2[INDEX_ONE]<<PCM_8_SHIFT);
  // print out the type of format
  char format_name[FORMAT_NAME_LENGTH] = "";
  if (sound_data_array[index].format_type == PCM_FORMAT)
   strcpy(format_name,"PCM");
  else if (sound_data_array[index].format_type == A_LAW_FORMAT)
    strcpy(format_name, "A-law");
  else if (sound_data_array[index].format_type == MU_LAW_FORMAT)
    strcpy(format_name, "Mu-law");
  // print the format type
  printf("(21-22) Format type: %u %s \n",
    sound_data_array[index].format_type,format_name);
  // grab number of channel (1 for mono & 2 for stereo)
  read = fread(buffer2,sizeof(buffer2),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].channels = buffer2[INDEX_ZERO] |
                                    (buffer2[INDEX_ONE]<<PCM_8_SHIFT);
  // print the channels used (1 or 2)
  printf("(23-24) Channels: %u \n", sound_data_array[index].channels);
  // grab the sample rate
  read = fread(buffer4,sizeof(buffer4),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].sample_rate = buffer4[INDEX_ZERO] |
                                        (buffer4[INDEX_ONE]<<PCM_8_SHIFT) |
                                        (buffer4[INDEX_TWO]<<PCM_16_SHIFT) |
                                        (buffer4[INDEX_THREE]<<PCM_24_SHIFT);
  // print the sample rate of the file
  printf("(25-28) Sample rate: %u\n", sound_data_array[index].sample_rate);
  // grab the byte rate
  read = fread(buffer4,sizeof(buffer4),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].byte_rate  = buffer4[INDEX_ZERO] |
                                      (buffer4[INDEX_ONE]<<PCM_8_SHIFT) |
                                      (buffer4[INDEX_TWO]<<PCM_16_SHIFT) |
                                      (buffer4[INDEX_THREE]<<PCM_24_SHIFT);
  // print the byte rate
  printf("(29-32) Byte Rate: %u\n", sound_data_array[index].byte_rate);
  // grab the block alignment, should be 2
  read = fread(buffer2,sizeof(buffer2),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].block_align = buffer2[INDEX_ZERO] |
                                        (buffer2[INDEX_ONE]<<PCM_8_SHIFT);
  // print the block alignment
  printf("(33-34) Block Alignment: %u\n", sound_data_array[index].block_align);
  // grab the number of bits per sample, should be 16
  read = fread(buffer2, sizeof(buffer2), READ_ONE_INDEX_POS, fp);
  sound_data_array[index].bits_per_sample = buffer2[INDEX_ZERO] |
                                            (buffer2[INDEX_ONE]<<PCM_8_SHIFT);
  // print the bits per sample
  printf("(35-36) Bits per sample: %u\n",
    sound_data_array[index].bits_per_sample);
  // grab the data marker, should read "data"
  read = fread(sound_data_array[index].data_chunk_header,
    sizeof(sound_data_array[index].data_chunk_header),READ_ONE_INDEX_POS,fp);
  printf("(37-40) Data Marker: %s\n",sound_data_array[index].data_chunk_header);
  // read the size of the data chunk
  read = fread(buffer4,sizeof(buffer4),READ_ONE_INDEX_POS,fp);
  sound_data_array[index].data_size = buffer4[INDEX_ZERO] |
                                      (buffer4[INDEX_ONE]<<PCM_8_SHIFT) |
                                      (buffer4[INDEX_TWO]<<PCM_16_SHIFT) |
                                      (buffer4[INDEX_THREE]<<PCM_24_SHIFT);
  printf("(41-44) Size of data chunk: %u\n",sound_data_array[index].data_size);
  // calculate number of samples & the size of each sample
  sound_data_array[index].num_samples =
    (BITS_PER_BYTE*sound_data_array[index].data_size) /
    (sound_data_array[index].channels*sound_data_array[index].bits_per_sample);
  printf("Number of samples: %zu\n", sound_data_array[index].num_samples);
  uint32_t size_of_each_sample = (sound_data_array[index].channels*
    sound_data_array[index].bits_per_sample)/BITS_PER_BYTE;
  printf("Size of each sample: %zu bytes\n", size_of_each_sample);
  /*** read the data parts of .wav fileName ***/
  if (sound_data_array[index].format_type == PCM_FORMAT) {
    // printf("Checkpoint 1, Index is %zu\n",index);
    uint32_t data_buffer; // 32 bit storage point
    uint16_t tmp_union; // 16 bit storage point
    char sample_extract[TWO_CHARACTERS]; // extracts 16 bits of data at a time
    // dynamically allocate space for the data in the file
    /****************we're hitting a seg fault between checkpoints 2 & 3, but on the third iteration****************/
     sound_data_array[index].sound_data =
      (uint32_t*)malloc(sizeof(uint32_t)*(sound_data_array[index].num_samples+1));
    // check and make sure malloc returned a valid value (address)
    if(sound_data_array[index].sound_data != NULL) {
      printf("******malloc success***\n\r");
    }
    // parse through each bit of data and load it into the data array
    for (uint32_t i = 0; i <= sound_data_array[index].num_samples; i++) {
      // reads 16 bits (one sample) from the data
      read = fread(sample_extract,sizeof(sample_extract),READ_ONE_INDEX_POS,fp);
      // puts the extracted samples into tmp_union
      /*little endian version*/
        //tmp_union = sample_extract[INDEX_ZERO]<<PCM_8_SHIFT; // may be the correct version
        //tmp_union = tmp_union|sample_extract[INDEX_ONE];
      /*big endian version*/
      tmp_union=sample_extract[INDEX_ONE]<<PCM_8_SHIFT;
      tmp_union = tmp_union|sample_extract[INDEX_ZERO];
      // sign extend tmp_union out to 32 bits and put it in data_buffer
      data_buffer = (uint32_t)tmp_union;//<<PCM_16_SHIFT;
      // move the information from data buffer into the sound_data array
      sound_data_array[index].sound_data[i] = data_buffer;
    }
  }
  uint16_t close_err = fclose(fp);
  if(close_err != SUCCESSFUL_CLOSE) {
    printf("Unable to close FILE* at index %zu",index);
  }
}

// Called to exit the driver (unmap and close UIO file)
void audio_driver_exit() {
  for(uint16_t i = 0; i < AUDIO_DRIVER_NUM_SAMPLE_FILES; i++) {
    free(sound_data_array[i].sound_data);
    sound_data_array[i].sound_data = 0;
  }
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

// Call to get the audio header and data out of the data data_array
// index : the audio sound index (each one contains a different sound)
// return : audio_data struct that contains the data buffer
audio_data_header audio_driver_get_data_array(uint32_t index){
  return sound_data_array[index];
}

/*****************************************************************************
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

    /* ADDED CODE TO THIS FUNCTION (Clint's fault entirely)*/
    // Re enable muted audio.
    write_audio_reg(R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, 0xE7, iic_fd);
    write_audio_reg(R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, 0xE7, iic_fd);
    /* END ADDED CODE TO THIS FUNCTION */

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
