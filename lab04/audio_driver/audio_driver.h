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
#define SUBCHUNK2SIZE 40
#define DATA_OFFSET 44
#define FOUR_BITS 4
#define PCM_8_SHIFT 8
#define PCM_16_SHIFT 16
#define PCM_24_SHIFT 24
#define AUDIO_DRIVER_NUM_SAMPLE_FILES 9
#define AUDIO_DRIVER_WRITE_FAILED -1
#define AUDIO_DRIVER_WRITE_SUCCESS 0

//ADAU audio controller parameters
#define _AUDIO_ADAU1761_H_
// Slave address for the ADAU audio controller 8
#define IIC_SLAVE_ADDR          0x3B
// I2C Serial Clock frequency in Hertz
#define IIC_SCLK_RATE           400000
// I2S Register
#define I2S_DATA_RX_L_REG           0x00
#define I2S_DATA_RX_R_REG           0x04
#define I2S_DATA_TX_L_REG           0x08
#define I2S_DATA_TX_R_REG           0x0C
#define I2S_STATUS_REG              0x10
//ADAU internal registers
enum audio_adau1761_regs {
    R0_CLOCK_CONTROL                                = 0x00,
    R1_PLL_CONTROL                                  = 0x02,
    R2_DIGITAL_MIC_JACK_DETECTION_CONTROL           = 0x08,
    R3_RECORD_POWER_MANAGEMENT                      = 0x09,
    R4_RECORD_MIXER_LEFT_CONTROL_0                  = 0x0A,
    R5_RECORD_MIXER_LEFT_CONTROL_1                  = 0x0B,
    R6_RECORD_MIXER_RIGHT_CONTROL_0                 = 0x0C,
    R7_RECORD_MIXER_RIGHT_CONTROL_1                 = 0x0D,
    R8_LEFT_DIFFERENTIAL_INPUT_VOLUME_CONTROL       = 0x0E,
    R9_RIGHT_DIFFERENTIAL_INPUT_VOLUME_CONTROL      = 0x0F,
    R10_RECORD_MICROPHONE_BIAS_CONTROL              = 0x10,
    R11_ALC_CONTROL_0                               = 0x11,
    R12_ALC_CONTROL_1                               = 0x12,
    R13_ALC_CONTROL_2                               = 0x13,
    R14_ALC_CONTROL_3                               = 0x14,
    R15_SERIAL_PORT_CONTROL_0                       = 0x15,
    R16_SERIAL_PORT_CONTROL_1                       = 0x16,
    R17_CONVERTER_CONTROL_0                         = 0x17,
    R18_CONVERTER_CONTROL_1                         = 0x18,
    R19_ADC_CONTROL                                 = 0x19,
    R20_LEFT_INPUT_DIGITAL_VOLUME                   = 0x1A,
    R21_RIGHT_INPUT_DIGITAL_VOLUME                  = 0x1B,
    R22_PLAYBACK_MIXER_LEFT_CONTROL_0               = 0x1C,
    R23_PLAYBACK_MIXER_LEFT_CONTROL_1               = 0x1D,
    R24_PLAYBACK_MIXER_RIGHT_CONTROL_0              = 0x1E,
    R25_PLAYBACK_MIXER_RIGHT_CONTROL_1              = 0x1F,
    R26_PLAYBACK_LR_MIXER_LEFT_LINE_OUTPUT_CONTROL  = 0x20,
    R27_PLAYBACK_LR_MIXER_RIGHT_LINE_OUTPUT_CONTROL = 0x21,
    R28_PLAYBACK_LR_MIXER_MONO_OUTPUT_CONTROL       = 0x22,
    R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL      = 0x23,
    R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL     = 0x24,
    R31_PLAYBACK_LINE_OUTPUT_LEFT_VOLUME_CONTROL    = 0x25,
    R32_PLAYBACK_LINE_OUTPUT_RIGHT_VOLUME_CONTROL   = 0x26,
    R33_PLAYBACK_MONO_OUTPUT_CONTROL                = 0x27,
    R34_PLAYBACK_POP_CLICK_SUPPRESSION              = 0x28,
    R35_PLAYBACK_POWER_MANAGEMENT                   = 0x29,
    R36_DAC_CONTROL_0                               = 0x2A,
    R37_DAC_CONTROL_1                               = 0x2B,
    R38_DAC_CONTROL_2                               = 0x2C,
    R39_SERIAL_PORT_PAD_CONTROL                     = 0x2D,
    R40_CONTROL_PORT_PAD_CONTROL_0                  = 0x2F,
    R41_CONTROL_PORT_PAD_CONTROL_1                  = 0x30,
    R42_JACK_DETECT_PIN_CONTROL                     = 0x31,
    R67_DEJITTER_CONTROL                            = 0x36,
    R58_SERIAL_INPUT_ROUTE_CONTROL                  = 0xF2,
    R59_SERIAL_OUTPUT_ROUTE_CONTROL                 = 0xF3,
    R61_DSP_ENABLE                                  = 0xF5,
    R62_DSP_RUN                                     = 0xF6,
    R63_DSP_SLEW_MODES                              = 0xF7,
    R64_SERIAL_PORT_SAMPLING_RATE                   = 0xF8,
    R65_CLOCK_ENABLE_0                              = 0xF9,
    R66_CLOCK_ENABLE_1                              = 0xFA
};

/********************************** structs **********************************/
// struct containing the header and data of audio
typedef struct audio_data{ // 48 bytes for the struct itself
   char riff[FOUR_BITS]; // RIFF string
   uint32_t overall_size; // overall size of file in bytes
   char wave[FOUR_BITS]; // WAVE string
   char fmt_chunk_marker[FOUR_BITS]; // fmt string with trailing null char
   uint16_t length_of_fmt; // length of the format data
   uint16_t format_type; // format type.
   uint16_t channels; // no.of channels
   uint16_t sample_rate;  // sampling rate (blocks per second)
   uint32_t byte_rate; // SampleRate * NumChannels * BitsPerSample/8
   uint16_t block_align; // NumChannels * BitsPerSample/8
   uint16_t bits_per_sample; // bits per sample, 8- 8bits, 16- 16 bits etc
   char data_chunk_header[FOUR_BITS]; // DATA string or FLLR string
   uint32_t data_size; // NumSamples*NumChannels*(BitsPerSample/8)-ChunkSize2
   uint32_t num_samples; // the number of samples inside the file
   uint32_t * sound_data; // a pointer to the start of the data
 }audio_data_header;
 struct audio_data audio_data_info;

// array of sound data
audio_data_header sound_data_array[AUDIO_DRIVER_NUM_SAMPLE_FILES];
/**************************** function prototypes ****************************/
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
// returns an int indicating a success or failure
 int16_t audio_driver_write(uint32_t *buf, int32_t len);

// Called to read to the audio driver
// len : the amount of bytes to read into the buffer
// returns a value with the type of success pending
int16_t audio_driver_read(int32_t len);

// Call to get the audio header and data out of the data data_array
// index : the audio sound index (each one contains a different sound)
// return : audio_data struct that contains the data buffer
audio_data_header audio_driver_get_data_array(uint32_t index);

/******************************************************************************
 * Function to write 8 bits to one of the registers from the audio
 * controller.
 * @param   u8RegAddr is the register address.
 * @param   u8Data is the data byte to write.
 * @param   iic_fd is the file descriptor for /dev/i2c-x
 * @return  none.
 *****************************************************************************/
void write_audio_reg(unsigned char u8RegAddr,
                     unsigned char u8Data, int iic_fd);

/******************************************************************************
* Function to configure the audio PLL.
* @param   iic_index is the i2c index in /dev list.
* @return  none.
*****************************************************************************/
void config_audio_pll(int iic_index);

/******************************************************************************
 * Function to configure the audio codec.
 * @param   iic_index is the i2c index in /dev list.
 * @return  none.
 *****************************************************************************/
void config_audio_codec(int iic_index);
