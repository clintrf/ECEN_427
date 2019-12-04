#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irqdomain.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <stdbool.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
//#include <linux/ioctl.h>// milestone 3 also declared in linux/fs.h
/*********************************** macros *********************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seth Becerra, Dax Eckles & Clint Frandsen");
MODULE_DESCRIPTION("ECEn 427 Pit Driver");

#define MODULE_NAME "pit"

/********************************* prototypes ********************************/
static ssize_t pit_read(struct file *f, char *buf, size_t len, loff_t *off);
static ssize_t pit_write(struct file *f, const char *buf, size_t len,loff_t *off);


/*********************************** structs *********************************/
// struct containing the pit_device data
 typedef struct pit_device {
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device * pdev;      // Platform device pointer
    struct device* dev;                 // device (/dev)
    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size
    u32* virt_addr;                     // Virtual address
    // Add any items to this that you need
} pit_dev;

// struct containing the file operations data
static struct file_operations pit_fops = {
  .owner = THIS_MODULE,
  .read = audio_read,
  .write = audio_write,
  .unlocked_ioctl = pit_ioctl
};

// Link between the hardware and its driver
static struct of_device_id audio_of_match[] = {
  { .compatible = "byu,ecen427-pit_codec", },
  {}
};
MODULE_DEVICE_TABLE(of, audio_of_match);

// struct containing the platform driver
static struct platform_driver audio_platform_driver = {
  .probe = pit_probe,
  .remove = pit_remove,
  .driver = {
    .name = MODULE_NAME,
    .owner = THIS_MODULE,
    .of_match_table = pit_of_match,
  }
};

/****************************** global variables ****************************/
static audio_dev dev; // global audio device
static dev_t dev_nums; // contains major and minor numbers
static struct class *pit;
static struct device *device;
static struct cdev cdev; // character device for the driver
static struct resource *res; // Device Resource Structure
static struct resource *res_mem; // Device Resource Structure
static struct resource *res_irq; // Device Resource Structure
static unsigned int irq_num; // contains the irq number

/***************************** kernel definitions ****************************/
static int audio_init(void);
static void audio_exit(void);

module_init(audio_init);
module_exit(audio_exit);

// reads a certain amount of bytes from a buffer
// f : the file to read from
// buf : the buffer to place the read values into
// len : the number of bytes to read
// off : indicates the file position the user is accessing
// return one byte of data (0 or 1) stating if an audio sample is being played
static ssize_t pit_read(struct file *f, char *buf, size_t len, loff_t *off) {
  printk(KERN_INFO "Driver: read()\n");
}

// reads a certain amount of bytes from a buffer
// f : the file to read from
// buf : accepts a signed 32 bit buffer containing an audio clip
// len : the number of bytes to write
// off : indicates the file position the user is accessing
// returns how many byteirq_locs were written
static ssize_t pit_write(struct file *f, const char *buf, size_t len,loff_t *off) {
  printk(KERN_INFO "Driver: Write()\n");
}

// function that handles the irq
// irq : irq number
// dev_id : the device id
// returns a flag stating if the irq was handled properly
static irqreturn_t irq_isr(int irq_loc, void *dev_id) {
  pr_info("IRQ_ISR: Calling the irq_isr!\n");
}

// Extend your kernel driver to add ioctl to the list of file operations supported by your character device
//
//long audio_ioctl(int fd, unsigned int cmd,unsigned long arg)// or ,
/*_IOC(dir,type,nr,size)_IO(type,nr)
_IOR(type,nr,size)
_IOW(type,nr,size)
_IOWR(type,nr,size)
Macros used to create anioctl command.
_IOC_DIR(nr)
 _IOC_TYPE(nr)
 _IOC_NR(nr)
 _IOC_SIZE(nr)
Macrosusedtodecodeacommand.Inparticular,_IOC_TYPE(nr)isanORcom-bination of_IOC_READ and_IOC_WRITE.*/
static long pit_ioctl(struct file *f, unsigned int cmd,unsigned long arg){

}

/********************************** functions ********************************/
// This is called when Linux loadrite (buffer);s your driver
// returns : an int signalling a successful initialization or an error
static int pit_init(void) {
  pr_info("%s: Initializing Audio Driver!\n", MODULE_NAME);
  // sound_playing = false; // flag to indicate that a sound it playing
  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  int err = alloc_chrdev_region(&dev_nums,FIRST_MINOR,NUM_OF_CONTIGUOUS_DEVS,
    MODULE_NAME);
  if(err < INIT_SUCCESS) { // failure doing region allocation
    pr_info("Failure allocating major/minor numbers!\n");
    return INIT_ERR;
  }
  int minor_num = MINOR(dev_nums); // returns the minor number of a dev_t type
  dev.minor_num = minor_num;
  // Create a device class. -- class_create()
  audio = class_create(THIS_MODULE,MODULE_NAME);
  if(audio == NULL) { // failed to create the class, undo allocation
    pr_info("Failure creating device class!\nRollback changes...\\n");
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // Register the driver as a platform driver -- platform_driver_register
  err = platform_driver_register(&audio_platform_driver);
  if(err < INIT_SUCCESS) { // failed to register the platform driver,
    pr_info("Failure registering platform driver!\nRollback changes...\\n");
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  pr_info("%s: Audio Driver initialization success!\n", MODULE_NAME);
  return INIT_SUCCESS;
}

// This is called when Linux unloads your driver
static void audio_exit(void) {
  pr_info("%s: Exiting Audio Driver!\n", MODULE_NAME);
  platform_driver_unregister(&audio_platform_driver); // unregister platform
  class_destroy(audio); // class_destroy
  unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  pr_info("%s: Finish Exit Audio Driver!\n", MODULE_NAME);
  return;
}

// Called by kernel when a platform device is detected that matches the
// 'compatible' name of this driver.
// pdev : platform device which to probe
// returns an int signalling a successful probe or some kind of error
static int audio_probe(struct platform_device *pdev) {
  pr_info("%s: Probing Audio Driver!\n", MODULE_NAME);
  if(audio_probe_called_once == true) { // we want to call this function once
    pr_info("Already called probe() once...\n");
    return PROBE_SUCCESS;
  }
  // Initialize the character device structure (cdev_init)
  cdev_init(&cdev,&audio_fops);
  cdev.owner = THIS_MODULE;
  // Register the character device with Linux (cdev_add)
  int err = cdev_add(&cdev,dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  if(err < PROBE_SUCCESS) { // if err is negative, the device hasn't been added
    pr_info("Failure registering the cdev!\nRollback changes...\\n");
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.cdev = cdev;
  // Create a device file in /dev so that the character device can be accessed
  // from user space
  device = device_create(audio,NULL,dev_nums,NULL,MODULE_NAME);
  if(device == NULL) { // if the device returns null, then we hit an error
    pr_info("Failure creating device!\nRollback changes...\\n");
    cdev_del(&cdev);
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.dev = device;
  // Get the physical device address from the device tree
  res = platform_get_resource(pdev,IORESOURCE_MEM,FIRST_RESOURCE);
  if(res == NULL) { // if the resource returns null, then we hit an error
    pr_info("Failure Getting Resources 01!\nRollback changes...\\n");
    device_destroy(audio,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.phys_addr = res->start;
  // Reserve the memory region -- request_mem_region
  dev.mem_size = (res->end)-(res->start)+1;
  res_mem = request_mem_region(dev.phys_addr,dev.mem_size,MODULE_NAME);
  if(res_mem == NULL) { // if the resource returns null, then we hit an error
    pr_info("Failure Requesting Memory Region!\nRollback changes...\n");
    device_destroy(audio,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  }
  // Get a (virtual memory) pointer to the device -- ioremap
  dev.virt_addr = ioremap(res->start,dev.mem_size);
  // Get the IRQ number from the device tree -- platform_get_resource
  res_irq = platform_get_resource(pdev,IORESOURCE_IRQ,FIRST_RESOURCE);
  irq_num = res_irq->start;
  if(res_irq == NULL){ // if the resource returns null, then we hit an error
    pr_info("Failure Getting Resources 02!\nRollback changes...\\n");
    release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
    device_destroy(audio,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.pdev = pdev;
  // Register your interrupt service routine -- request_irq
  int irq_err = request_irq(irq_num,irq_isr,0,MODULE_NAME,NULL);
  if(irq_err < PROBE_SUCCESS) { // failed to register the platform driver
    pr_info("Failure calling the request_irq !\nRollback changes...\\n");
    release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
    device_destroy(audio,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&audio_platform_driver);
    class_destroy(audio);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  audio_probe_called_once = true; // makes certain we don't run probe twice
  pr_info("%s: Audio Driver probing success!\n", MODULE_NAME);
  pr_info("%s: Major Number: %zu\n", MODULE_NAME, MAJOR(dev_nums));
  pr_info("%s: Physical Addr: %zu\n", MODULE_NAME, dev.phys_addr);
  pr_info("%s: Virtual Addr: %p\n", MODULE_NAME, dev.virt_addr);
  pr_info("%s: IRQ Number: %zu\n", MODULE_NAME, irq_num);
  return PROBE_SUCCESS;
}

// removes the audio_device by unloaded and unmapping it, destroys device
// returns : an int signalling success or failure
static int audio_remove(struct platform_device * pdev) {
  pr_info("%s: Removing Audio Driver!\n", MODULE_NAME);
  free_irq(irq_num,NULL); // free the irq to allow interrupts to continue
  ioport_unmap(dev.virt_addr); // iounmap
  release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
  device_destroy(audio,dev_nums); // device_destroy
  cdev_del(&cdev); // cdev_del
  pr_info("%s: Removing Audio Driver success!\n", MODULE_NAME);
  return REMOVE_SUCCESS;
}
