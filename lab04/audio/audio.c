#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <stdbool.h>

#include <linux/err.h>
#include <linux/string.h>

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

//#define platform_register_drivers(drivers, count) //cf (change)

/**************************** function definitions ***************************/
static int audio_init(void);
static void audio_exit(void);

module_init(audio_init);
module_exit(audio_exit);

static int audio_open(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: open()\n");
  return INIT_SUCCESS;
}

static int audio_release(struct inode *i, struct file *f) {
  printk(KERN_INFO "Driver: close()\n");
  return INIT_SUCCESS;
}

static ssize_t audio_read(struct file *f, char *buf, size_t len, loff_t *off) {
  printk(KERN_INFO "Driver: read()\n");
  return INIT_SUCCESS;
}

static ssize_t audio_write(struct file *f, const char *buf, size_t len,
    loff_t *off) {
  printk(KERN_INFO "Driver: write()\n");
  return len;
}

/*********************************** structs *********************************/
// struct containing the audio_device data
typedef struct audio_device audio_device;
static struct audio_device {
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device * pdev;      // Platform device pointer
    struct device* dev;                 // device (/dev)

    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size
    u32* virt_addr;                     // Virtual address
    // Add any items to this that you need
};

// struct containing the file operations data
static struct file_operations audio_fops = {
  .owner = THIS_MODULE,
  .open = audio_open,
  .release = audio_release,
  .read = audio_read,
  .write = audio_write
};

// Link between the hardware and its driver
static struct of_device_id audio_of_match[] __devinitdata = {
  { .compatible = "ecen427_audio_codec", },
  {}
};

// struct containing the platform driver
static struct platform_driver audio_platform_driver = {
  .probe = audio_probe,
  .remove = audio_remove,
  .driver = {
    .name = MODULE_NAME,
    .owner = THIS_MODULE,
    .of_match_table = audio_of_match,
  },
};

/****************************** global variables ****************************/
static audio_device dev; // global audio device
static platform_device pdev; // global platform device
static bool audio_probe_called_once = false; // makes audio_probe call once
static dev_t dev_nums; // contains major and minor numbers
static struct class *audio;
static device *device;
static struct cdev cdev; // character device for the driver

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
  dev.minor_num = minor_num;
  
  // Create a device class. -- class_create()
  audio = class_create(THIS_MODULE,MODULE_NAME);
  if(audio == NULL) { // failed to create the class, undo allocation
    pr_info("Failure creating device class!\nUnregistering...\n");
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // Register the driver as a platform driver -- platform_driver_register
  pdev = platform_driver_register(&audio_platform_driver);
  if(pdev == NULL) { // failed to create the class, undo allocation
    pr_info("Failure registering platform driver!\nUnregistering...\n");
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  dev.pdev = &pdev;

  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.

  return INIT_SUCCESS;
}

// This is called when Linux unloads your driver
static void audio_exit(void) {
  // platform_driver_unregister
  platform_unregister_drivers(&audio_platform_driver); //cf
  // class_unregister and class_destroy
  //class_unregister(&audio);
  class_destroy(audio);
  // unregister_chrdev_region
  unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  return;
}

// Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
// returns :
static int audio_probe(struct platform_device *pdev) {
  // Initialize the character device structure (cdev_init)
  cdev_init(&cdev,&audio_fops);
  dev.cdev = cdev;
  // Register the character device with Linux (cdev_add)
  int err = cdev_add(&cdev,dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  if(err < INIT_SUCCESS) { // if err is negative, the device has NOT been added
    pr_info("Failure registering the cdev!\nDestroying...\n");
    // platform_driver_unregister
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // Create a device file in /dev so that the character device can be accessed from user space (device_create).
  device = device_create(audio,NULL,dev_nums,NULL,MODULE_NAME)
  if(device == NULL) {
    pr_info("Failure creating device!\nUnregistering and destroying...\n");
    // platform_driver_unregister
    // do something with cdev??
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  dev.dev = device;
  // Get the physical device address from the device tree -- platform_get_resource
  // Reserve the memory region -- request_mem_region
  // Get a (virtual memory) pointer to the device -- ioremap

  // Get the IRQ number from the device tree -- platform_get_resource
  // Register your interrupt service routine -- request_irq

  // If any of the above functions fail, return an appropriate linux error code, and make sure
  // you reverse any function calls that were successful.
  audio_probe_called_once = true;
  return INIT_SUCCESS;
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
