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
#define BITSPERBYTE 8

/********************************** globals **********************************/
static int fd; /* this is a file descriptor that describes the UIO device */
static uint16_t off = 0;
audio_data_header  data_array[9];


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
  audio_driver_import_audio(INVADER_DIE_AUDIO, 0);
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

// This goes through one audio file, converts it, and puts it in the data_array
// fileName : the name of the file to import
// index : the index within data_array to place the imported and converted file
void audio_driver_import_audio(char fileName[], uint16_t index) {
  printf("Print Audio Function\n");
  size_t sizeOfFile = 0;
  char* rawbuffer = 0;
  unsigned short int* sampleBuf = 0;
  size_t bytesRead = 0;
  /* Error checking for getting file name */
  if (!fileName) { errx(1, "Filename not specified"); }
  else { /* If we are getting a fileName, we want to print the name */
    printf("File Name: ");
    for(int i = 0; i < 52; i++) {
      printf("%c",fileName[i]);
    }
    printf("\r\n");
  }
  /* Open the file given */
  FILE* fp = fopen(fileName, "r");
  /* Error check if File was correctly opened */
  if (!fp) { errx(1, "Filename not opened"); }
  int read=0;
  char buffer4[4];
  char buffer2[2];
  //read the header parts of .wav fileName
  read=fread(audio_data_info.riff,sizeof(audio_data_info.riff),1,fp);
  printf("(1-4): %s \n", audio_data_info.riff);
  read =fread(buffer4,sizeof(buffer4),1,fp);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
  //convert little endian to big endian 4 byte int
  audio_data_info.overall_size=buffer4[0]|(buffer4[1]<<PCM_8_SHIFT)|
                               (buffer4[2]<<PCM_16_SHIFT)|(buffer4[3]<<PCM_24_SHIFT);
  printf("(5-8) Overall size: bytes:%u, Kb:%u \n",
      audio_data_info.overall_size, audio_data_info.overall_size/1024);
  read=fread(audio_data_info.wave,sizeof(audio_data_info.wave),1,fp);
  printf("(9-12) Wave marker: %s\n", audio_data_info.wave);
  read=fread(audio_data_info.fmt_chunk_marker,sizeof(audio_data_info.fmt_chunk_marker),1,fp);
  printf("(13-16) Fmt marker: %s\n", audio_data_info.fmt_chunk_marker);
  read = fread(buffer4, sizeof(buffer4), 1, fp);
	printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
  // convert little endian to big endian 4 byte integer
  audio_data_info.length_of_fmt = buffer4[0] |
                            (buffer4[1] << PCM_8_SHIFT) |
                            (buffer4[2] << PCM_16_SHIFT) |
                            (buffer4[3] << PCM_24_SHIFT);
  printf("(17-20) Length of Fmt header: %u \n", audio_data_info.length_of_fmt);
  read = fread(buffer2, sizeof(buffer2), 1, fp);
  printf("%u %u \n", buffer2[0], buffer2[1]);
  audio_data_info.format_type = buffer2[0] | (buffer2[1] << 8);
  char format_name[10] = "";
  if (audio_data_info.format_type == 1)
   strcpy(format_name,"PCM");
  else if (audio_data_info.format_type == 6)
    strcpy(format_name, "A-law");
  else if (audio_data_info.format_type == 7)
    strcpy(format_name, "Mu-law");
  printf("(21-22) Format type: %u %s \n", audio_data_info.format_type, format_name);
  read = fread(buffer2, sizeof(buffer2), 1, fp);
  printf("%u %u \n", buffer2[0], buffer2[1]);
  audio_data_info.channels = buffer2[0] | (buffer2[1] << 8);
  printf("(23-24) Channels: %u \n", audio_data_info.channels);
  read = fread(buffer4, sizeof(buffer4), 1, fp);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
  audio_data_info.sample_rate = buffer4[0] |
                      (buffer4[1] << 8) |
                      (buffer4[2] << 16) |
                      (buffer4[3] << 24);
  printf("(25-28) Sample rate: %u\n", audio_data_info.sample_rate);
  read = fread(buffer4, sizeof(buffer4), 1, fp);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
  audio_data_info.byterate  = buffer4[0] |
                    (buffer4[1] << 8) |
                    (buffer4[2] << 16) |
                    (buffer4[3] << 24);
  printf("(29-32) Byte Rate: %u , Bit Rate:%u\n",
  audio_data_info.byterate, audio_data_info.byterate*8);
  read = fread(buffer2, sizeof(buffer2), 1, fp);
  printf("%u %u \n", buffer2[0], buffer2[1]);
  audio_data_info.block_align = buffer2[0] |
                    (buffer2[1] << 8);
  printf("(33-34) Block Alignment: %u \n", audio_data_info.block_align);
  read = fread(buffer2, sizeof(buffer2), 1, fp);
  printf("%u %u \n", buffer2[0], buffer2[1]);
  audio_data_info.bits_per_sample = buffer2[0] |
                    (buffer2[1] << 8);
  printf("(35-36) Bits per sample: %u \n", audio_data_info.bits_per_sample);
  read = fread(audio_data_info.data_chunk_header, sizeof(audio_data_info.data_chunk_header), 1, fp);
  printf("(37-40) Data Marker: %s \n", audio_data_info.data_chunk_header);
  read = fread(buffer4, sizeof(buffer4), 1, fp);
  printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
  audio_data_info.data_size = buffer4[0] |
                (buffer4[1] << 8) |
                (buffer4[2] << 16) |
                (buffer4[3] << 24 );
  printf("(41-44) Size of data chunk: %u \n", audio_data_info.data_size);
  // calculate no.of samples
  uint32_t num_samples = (8 * audio_data_info.data_size) /
                         (audio_data_info.channels * audio_data_info.bits_per_sample);
  printf("Number of samples:%lu \n", num_samples);
  uint32_t size_of_each_sample = (audio_data_info.channels * audio_data_info.bits_per_sample) / 8;
  printf("Size of each sample:%ld bytes\n", size_of_each_sample);
  // read each sample from data chunk if PCM//
  if (audio_data_info.format_type == 1) { // PCM
    /*Seth Code, */
    long i =0;
    char data_buffer[size_of_each_sample];
    long bytes_in_each_channel = (size_of_each_sample / audio_data_info.channels);
    int data_in_channel = 0;
    for (i =1; i <= num_samples; i++) {
      if (bytes_in_each_channel == 4) {
          data_in_channel =   data_buffer[0] |
                              (data_buffer[1]<<8) |
                              (data_buffer[2]<<16) |
                              (data_buffer[3]<<24);
      }
      else if (bytes_in_each_channel == 2) {
          data_in_channel = data_buffer[0] |
                            (data_buffer[1] << 8);
      }
      else if (bytes_in_each_channel == 1) {
          data_in_channel = data_buffer[0];
      }
      printf("%d ", data_in_channel);
    }
    /*End of seth code*/
    
    printf("Dump sample data? Y/N?");
    char c = 'n';
    scanf("%c", &c);
    if (c == 'Y' || c == 'y') {
        // long i =0;
        // char data_buffer[size_of_each_sample];
        // int  size_is_correct = 1;
        // // make sure that the bytes-per-sample is completely divisible by num.of channels
        // long bytes_in_each_channel = (size_of_each_sample / audio_data_info.channels);
        // if ((bytes_in_each_channel  * audio_data_info.channels) != size_of_each_sample) {
        //     printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, audio_data_info.channels, size_of_each_sample);
        //     size_is_correct = 0;
        // }
        // if (size_is_correct) {
        //             // the valid amplitude range for values based on the bits per sample
        //     long low_limit = 0l;
        //     long high_limit = 0l;
        //     switch (audio_data_info.bits_per_sample) {
        //         case 8:
        //             low_limit = -128;
        //             high_limit = 127;
        //             break;
        //         case 16:
        //             low_limit = -32768;
        //             high_limit = 32767;
        //             break;
        //         case 32:
        //             low_limit = -2147483648;
        //             high_limit = 2147483647;
        //             break;
        //     }
        //     printf("\n\n.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
        //     for (i =1; i <= num_samples; i++) {
        //         printf("==========Sample %ld / %ld=============\n", i, num_samples);
        //         read = fread(data_buffer, sizeof(data_buffer), 1, fp);
        //         if (read == 1) {
        //             // dump the data read
        //             unsigned int  xchannels = 0;
        //             int data_in_channel = 0;
        //     for (xchannels = 0; xchannels < audio_data_info.channels; xchannels ++ ) {
        //                 printf("Channel#%d : ", (xchannels+1));
        //                 // convert data from little endian to big endian based on bytes in each channel sample
        //                 if (bytes_in_each_channel == 4) {
        //                     data_in_channel =   data_buffer[0] |
        //                                         (data_buffer[1]<<8) |
        //                                         (data_buffer[2]<<16) |
        //                                         (data_buffer[3]<<24);
        //                 }
        //                 else if (bytes_in_each_channel == 2) {
        //                     data_in_channel = data_buffer[0] |
        //                                       (data_buffer[1] << 8);
        //                 }
        //                 else if (bytes_in_each_channel == 1) {
        //                     data_in_channel = data_buffer[0];
        //                 }
        //                 printf("%d ", data_in_channel);
        //                 // check if value was in range
        //                 if (data_in_channel < low_limit || data_in_channel > high_limit)
        //                     printf("**value out of range\n");
        //
        //                 printf(" | ");
        //             }
        //             printf("\n");
        //         }
        //         else {
        //             printf("Error reading file. %d bytes\n", read);
        //             break;
        //         }
        //     } //    for (i =1; i <= num_samples; i++) {
        // } //    if (size_is_correct) {
     } // if (c == 'Y' || c == 'y') {

  } //  if (header.format_type == 1) {
  // fseek(fp, 0L, SEEK_END); // Run though the entire file
  // sizeOfFile = (size_t)ftell(fp); // grab the size of the file
  // rewind(fp); // return to to top of file
  // /* Alocate memory based on size of file */
  // rawbuffer = (char*)malloc(sizeOfFile);
  // /* Error check if memory is alocated if not dealocate memory */
  // if (!rawbuffer){ errx(1, "Memory not allocated"); }
  // /* The actual reading part. Used to grab the data from the file */
  // bytesRead = fread((void*)rawbuffer, 1, sizeOfFile, fp);
  // /* Error check if buffer was written to */
  // if(bytesRead != sizeOfFile){ errx(1, "Did not read entire file"); }
  // /* Set up the audio_data structure and sets it to an index in data_array */
  // audio_data data;
  // data.total_size = bytesRead / sizeof(unsigned short int);
  // printf("Total Size: %zu\n", data.total_size);
  // data.head = rawbuffer;
  // data.data_size = data.total_size-(DATA_OFFSET/2);
  // printf("Total Size: %zu\n", data.data_size);
  // data.data = rawbuffer+DATA_OFFSET;
  //
  // /* Save copy of Raw buffer by converting from char to int */
  // sampleBuf = (unsigned short int*) data.head;
  // uint32_t * dataCheck = (uint32_t *) data.data;
  // size_t j = 44;
  // size_t k = 0;
  // data.sample = data.data;
  // uint32_t * base = data.sample;
  //
  // /* Used to print out the data in ASCII (checks for correct transfer) */
  // for (size_t i = 0; i < (data.total_size - data.data_size) ; i++) {
  //     printf("Data at Index %zu: %c\n",j++,(sampleBuf[i]&0x00ff));
  //     printf("Data at Index %zu: %c\n",j++, (sampleBuf[i]&0xff00)>>8);
  // }
  // printf("\r\n\n\n");
  // // j = 44;
  // // for(size_t i = 0; i < (44/*data.data_size*/)/2; i++) {
  // //   printf("Data at Index %zu: %x, %x\n",j++,(dataCheck[i]&0x00ff0000),dataCheck[i]);
  // //   printf("Data at Index %zu: %x, %x\n",j++, (dataCheck[i]&0xff000000)>>8,dataCheck[i]);
  // //
  // // }
  //
  // for(size_t i = 0; i < 44/*(data.data_size)/2*/; i++) {
  //   printf("Data at Index %zu: %x, %x\n",j,(dataCheck[i]&0x0000ffff)<<16,dataCheck[i]);
  //   printf("Data at Index %zu: %x, %x\n",j++,(dataCheck[i]&0xffff0000),dataCheck[i]);
  // }
  // data_array[index] = data;
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

// Call to get the audio header and data out of the data data_array
// index : the audio sound numbersd
// return : audio_data struct that contains the data buffer and the size of the index
audio_data_header audio_driver_get_data_array(uint32_t index){
  return data_array[index];
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
