#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dax Eckles & Clint Frandsen");
MODULE_DESCRIPTION("ECEn 427 Audio Driver");

#define MODULE_NAME "audio"
#define I2S_DATA_RX_L_REG_OFFSET 0x00 // input for left channel (low 24 bits)
#define I2S_DATA_RX_R_REG_OFFSET 0x04 // input for right channel (low 24 bits)
#define I2S_DATA_TX_R_REG_OFFSET 0x08 // output for right channel (low 24 bits)
#define I2S_DATA_TX_L_REG_OFFSET 0x0C // output for left channel (low 24 bits)
/*
 * Various statuses are held in this register
 * [0] : control interrupt generation (0 disabled, 1 enabled)
 * [10-1] : number of values currently in the audio FIFO right channel
 * [20-11] : number of values currently in the audio FIFO left channel
 * [21] : each time a new audio sample is available, this is 1, must be cleared
*/
#define I2S_STATUS_REG_OFFSET 0x10
#define I2S_STATUS_REG_INTERRUPT_CONTROL 0
#define I2S_STATUS_REG_RIGHT_FIFO_VALUE 1
#define I2S_STATUS_REG_LEFT_FIFO_VALUE 11
#define I2S_STATUS_REG_NEW_AUDIO_SAMPLE 21

// Function declarations
static int audio_init(void);
static void audio_exit(void);

module_init(audio_init);
module_exit(audio_exit);


struct audio_device {
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device * pdev;      // Platform device pointer
    struct device* dev;                 // device (/dev)

    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size
    u32* virt_addr;                     // Virtual address

    // Add any items to this that you need
};

// This is called when Linux loads your driver
static int audio_init(void) {
  pr_info("%s: Initializing Audio Driver!\n", MODULE_NAME);

  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.

  create_audio_device(); // creates the audio device

  // Register the driver as a platform driver -- platform_driver_register

  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.

  return 0; // (Success)
}

audio_device create_audio_device() {
  audio_device dev;
  dev.minor_num = 0;
  // dev.cdev = 0;
  // dev.platform_device = 0;
  // dev.phys_addr = 0;
  // dev.mem_size = 0;
  // dev.virt_addr = 0;
  return dev;
}

// This is called when Linux unloads your driver
static void audio_exit(void) {
  // platform_driver_unregister
  // class_unregister and class_destroy
  // unregister_chrdev_region
  return;
}

// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
static int audio_probe(struct platform_device *pdev) {
  // Initialize the character device structure (cdev_init)
  // Register the character device with Linux (cdev_add)

  // Create a device file in /dev so that the character device can be accessed from user space (device_create).

  // Get the physical device address from the device tree -- platform_get_resource
  // Reserve the memory region -- request_mem_region
  // Get a (virtual memory) pointer to the device -- ioremap

  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq

  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.

  return 0; //(success)
}

static int audio_remove(struct platform_device * pdev) {
  // iounmap
  // release_mem_region
  // device_destroy
  // cdev_del
  return 0;
}

/******************************************************************************
 * This function was copied from @file audio_adau1761.c, available on github at
 * https://github.com/Xilinx/PYNQ/blob/master/pynq/lib/_pynq/_audio/audio_adau1761.cpp
 * Function to configure the audio PLL.
 * @param   iic_index is the i2c index in /dev list.
 * @return  none.
 *****************************************************************************/
// extern void config_audio_pll(int iic_index) {
//     unsigned char u8TxData[8], u8RxData[6];
//     int iic_fd;
//     iic_fd = setI2C(iic_index, IIC_SLAVE_ADDR);
//     if (iic_fd < 0) {
//         printf("Unable to set I2C %d.\n", iic_index);
//     }
//
//     // Disable Core Clock
//     write_audio_reg(R0_CLOCK_CONTROL, 0x0E, iic_fd);
//     /*  MCLK = 10 MHz
//      *  R = 0100 = 4, N = 0x023C = 572, M = 0x0271 = 625
//      *  PLL required output = 1024x48 KHz = 49.152 MHz
//      *  PLLout/MCLK         = 49.152 MHz/10 MHz = 4.9152 MHz
//      *                      = R + (N/M)
//      *                      = 4 + (572/625)
//      */
//     // Register write address [15:8]
//     u8TxData[0] = 0x40;
//     // Register write address [7:0]
//     u8TxData[1] = 0x02;
//     // byte 6 - M[15:8]
//     u8TxData[2] = 0x02;
//     // byte 5 - M[7:0]
//     u8TxData[3] = 0x71;
//     // byte 4 - N[15:8]
//     u8TxData[4] = 0x02;
//     // byte 3 - N[7:0]
//     u8TxData[5] = 0x3C;
//     // byte 2 - bits 6:3 = R[3:0], 2:1 = X[1:0], 0 = PLL operation mode
//     u8TxData[6] = 0x21;
//     // byte 1 - 1 = PLL Lock, 0 = Core clock enable
//     u8TxData[7] = 0x03;
//     // Write bytes to PLL control register R1 at 0x4002
//     if (writeI2C_asFile(iic_fd, u8TxData, 8) < 0){
//         printf("Unable to write I2C %d.\n", iic_index);
//     }
//
//     // Poll PLL Lock bit
//     u8TxData[0] = 0x40;
//     u8TxData[1] = 0x02;
//     do {
//         if (writeI2C_asFile(iic_fd, u8TxData, 2) < 0){
//             printf("Unable to write I2C %d.\n", iic_index);
//         }
//         if (readI2C_asFile(iic_fd, u8RxData, 6) < 0){
//             printf("Unable to read I2C %d.\n", iic_index);
//         }
//     } while((u8RxData[5] & 0x02) == 0);
//
//     /* Clock control register:  bit 3        CLKSRC = PLL Clock input
//      *                          bit 2:1      INFREQ = 1024 x fs
//      *                          bit 0        COREN = Core Clock enabled
//      */
//     write_audio_reg(R0_CLOCK_CONTROL, 0x0F, iic_fd);
//
//     if (unsetI2C(iic_fd) < 0) {
//         printf("Unable to unset I2C %d.\n", iic_index);
//     }
// }

/******************************************************************************
 * This function was copied from @file audio_adau1761.c, available on github at
 * https://github.com/Xilinx/PYNQ/blob/master/pynq/lib/_pynq/_audio/audio_adau1761.cpp
 * Function to configure the audio codec.
 * @param   iic_index is the i2c index in /dev list.
 * @return  none.
 *****************************************************************************/
// extern void config_audio_codec(int iic_index) {
//     int iic_fd;
//     iic_fd = setI2C(iic_index, IIC_SLAVE_ADDR);
//     if (iic_fd < 0) {
//         printf("Unable to set I2C %d.\n", iic_index);
//     }
//
//     /*
//      * Input path control registers are configured
//      * in select_mic and select_line_in
//      */
//
//     // Mute Mixer1 and Mixer2 here, enable when MIC and Line In used
//     write_audio_reg(R4_RECORD_MIXER_LEFT_CONTROL_0, 0x00, iic_fd);
//     write_audio_reg(R6_RECORD_MIXER_RIGHT_CONTROL_0, 0x00, iic_fd);
//     // Set LDVOL and RDVOL to 21 dB and Enable left and right differential
//     write_audio_reg(R8_LEFT_DIFFERENTIAL_INPUT_VOLUME_CONTROL, 0xB3, iic_fd);
//     write_audio_reg(R9_RIGHT_DIFFERENTIAL_INPUT_VOLUME_CONTROL, 0xB3, iic_fd);
//     // Enable MIC bias
//     write_audio_reg(R10_RECORD_MICROPHONE_BIAS_CONTROL, 0x01, iic_fd);
//     // Enable ALC control and noise gate
//     write_audio_reg(R14_ALC_CONTROL_3, 0x20, iic_fd);
//     // Put CODEC in Master mode
//     write_audio_reg(R15_SERIAL_PORT_CONTROL_0, 0x01, iic_fd);
//     // Enable ADC on both channels, normal polarity and ADC high-pass filter
//     write_audio_reg(R19_ADC_CONTROL, 0x33, iic_fd);
//     // Mute play back Mixer3 and Mixer4 and enable when output is required
//     write_audio_reg(R22_PLAYBACK_MIXER_LEFT_CONTROL_0, 0x00, iic_fd);
//     write_audio_reg(R24_PLAYBACK_MIXER_RIGHT_CONTROL_0, 0x00, iic_fd);
//     // Mute left input to mixer3 (R23) and right input to mixer4 (R25)
//     write_audio_reg(R23_PLAYBACK_MIXER_LEFT_CONTROL_1, 0x00, iic_fd);
//     write_audio_reg(R25_PLAYBACK_MIXER_RIGHT_CONTROL_1, 0x00, iic_fd);
//     // Mute left and right channels output; enable them when output is needed
//     write_audio_reg(R29_PLAYBACK_HEADPHONE_LEFT_VOLUME_CONTROL, 0xE5, iic_fd);
//     write_audio_reg(R30_PLAYBACK_HEADPHONE_RIGHT_VOLUME_CONTROL, 0xE5, iic_fd);
//     // Enable play back right and left channels
//     write_audio_reg(R35_PLAYBACK_POWER_MANAGEMENT, 0x03, iic_fd);
//     // Enable DAC for both channels
//     write_audio_reg(R36_DAC_CONTROL_0, 0x03, iic_fd);
//     // Set SDATA_In to DAC
//     write_audio_reg(R58_SERIAL_INPUT_ROUTE_CONTROL, 0x01, iic_fd);
//     // Set SDATA_Out to ADC
//     write_audio_reg(R59_SERIAL_OUTPUT_ROUTE_CONTROL, 0x01, iic_fd);
//     // Enable DSP and DSP Run
//     write_audio_reg(R61_DSP_ENABLE, 0x01, iic_fd);
//     write_audio_reg(R62_DSP_RUN, 0x01, iic_fd);
//     /*
//      * Enable Digital Clock Generator 0 and 1.
//      * Generator 0 generates sample rates for the ADCs, DACs, and DSP.
//      * Generator 1 generates BCLK and LRCLK for the serial port.
//      */
//     write_audio_reg(R65_CLOCK_ENABLE_0, 0x7F, iic_fd);
//     write_audio_reg(R66_CLOCK_ENABLE_1, 0x03, iic_fd);
//
//     if (unsetI2C(iic_fd) < 0) {
//         printf("Unable to unset I2C %d.\n", iic_index);
//     }
// }
