#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <stdbool.h>

/*********************************** macros *********************************/
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
#define FIRST_MINOR 0
#define NUM_OF_CONTIGUOUS_DEVS 1
#define INIT_SUCCESS 0
#define INIT_ERR -1

/***************************** function prototypes ***************************/
static int audio_init(void);
static void audio_exit(void);
static void create_audio_device(void);

module_init(audio_init);
module_exit(audio_exit);

/*********************************** structs *********************************/
// struct containing the audio_device data
typedef struct audio_device audio_device;
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

/****************************** global variables ****************************/
static audio_device dev; // since we only use one audio device, we can use a global
static bool audio_probe_called_once = false; // makes sure audio_probe is called once
static dev_t dev_nums;
static struct class *audio;

/********************************** functions ********************************/
// This is called when Linux loads your driver
// returns : an int signalling a successful initialization or some kind of error
static int audio_init(void) {
  pr_info("%s: Initializing Audio Driver!\n", MODULE_NAME);
  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  int err = alloc_chrdev_region(&dev_nums,FIRST_MINOR,NUM_OF_CONTIGUOUS_DEVS,MODULE_NAME);
  if(err != 0) { // failure doing region allocation
    pr_info("Failure allocating major/minor numbers!\n");
    return INIT_ERR;
  }
  int major_num = MAJOR(dev_nums); // returns the major number of a dev_t type
  int minor_num = MINOR(dev_nums); // returns the minor number of a dev_t type
  // Create a device class. -- class_create()
  audio = class_create(THIS_MODULE,MODULE_NAME);
  if(audio == NULL) { // failed to create the class, undo allocation
    pr_info("Failure creating device class!\nUnregistering...\n");
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // create device, if there's a failure, then destroy the class and unregister
  if(device_create(audio,NULL,dev_nums,NULL,MODULE_NAME) == NULL) {
    pr_info("Failure creating device!\nUnregistering and destroying...\n");
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // Register the driver as a platform driver -- platform_driver_register

  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.

  return INIT_SUCCESS;
}

// This is called when Linux unloads your driver
static void audio_exit(void) {
  // platform_driver_unregister
  // class_unregister and class_destroy
  // unregister_chrdev_region
  unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  return;
}

// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
// returns :
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
  audio_probe_called_once = true;
  return 0; //(success)
}

// removes the audio_device by unloaded and unmapping it, destroys device
// returns : an int signalling success or failure
static int audio_remove(struct platform_device * pdev) {
  // iounmap
  // release_mem_region
  // device_destroy
  // cdev_del
  return 0;
}
