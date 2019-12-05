#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h> //possibly #include<linux/uaccess.h>
#include<linux/sysfs.h> 
#include<linux/kobject.h> 

// don't need the rest probably
#include <linux/types.h>
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
#include <stdbool.h>
#include <asm/io.h>
#include <linux/ioctl.h>
/*********************************** macros *********************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seth Becerra, Dax Eckles & Clint Frandsen");
MODULE_DESCRIPTION("ECEn 427 Pit Driver");

#define MODULE_NAME "pit"
#define MYIP_REG_OFFSET 0x43C3

#define I2S_STATUS_REG_OFFSET 4

// i2s register above
#define FIRST_MINOR 0
#define NUM_OF_CONTIGUOUS_DEVS 1
#define FIRST_RESOURCE 0
#define SECOND_RESOURCE 1
#define INIT_SUCCESS 0
#define INIT_ERR -1
#define PROBE_SUCCESS 0
#define PROBE_ERR -1
#define REMOVE_SUCCESS 0
#define INTERRUPTS_OFF 0x0
#define INTERRUPTS_ON 0x1
#define ZERO_BYTES_WRITTEN 0
#define WRITE_ERR -1
#define SOUND_NOT_PLAYING 0
#define SOUND_PLAYING 1
//#defines for IOCtl files
#define IOC_MAGIC  'k'


/********************************* prototypes ********************************/
static int pit_probe(struct platform_device *pdev);
static int pit_remove(struct platform_device * pdev);

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
} pit_dev;

// struct containing the file operations data
static struct file_operations pit_fops = {
  .owner = THIS_MODULE,
};

// Link between the hardware and its driver
static struct of_device_id pit_of_match[] = {
  { .compatible = "byu,ecen427-pit_codec", },    //CHANGE
  {}
};
MODULE_DEVICE_TABLE(of, pit_of_match);

// struct containing the platform driver
static struct platform_driver pit_platform_driver = {
  .probe = pit_probe,
  .remove = pit_remove,
  .driver = {
    .name = MODULE_NAME,
    .owner = THIS_MODULE,
    .of_match_table = pit_of_match,
  }
};

/****************************** global variables ****************************/
static pit_dev dev; // global pit device
static bool audio_probe_called_once = false; // makes audio_probe call once
static dev_t dev_nums; // contains major and minor numbers
static struct class *pit;
static struct device *device;
static struct cdev cdev; // character device for the driver
static struct resource *res; // Device Resource Structure
static struct resource *res_mem; // Device Resource Structure

/***************************** kernel definitions ****************************/
static int pit_init(void);
static void pit_exit(void);

module_init(pit_init);
module_exit(pit_exit);


/********************************** functions ********************************/
// This is called when Linux loadrite (buffer);s your driver
// returns : an int signalling a successful initialization or an error
static int pit_init(void) {
  pr_info("%s: Initializing PIT Driver!\n", MODULE_NAME);
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
  pit = class_create(THIS_MODULE,MODULE_NAME);
  if(pit == NULL) { // failed to create the class, undo allocation
    pr_info("Failure creating device class!\nRollback changes...\\n");
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  // Register the driver as a platform driver -- platform_driver_register
  err = platform_driver_register(&pit_platform_driver);
  if(err < INIT_SUCCESS) { // failed to register the platform driver,
    pr_info("Failure registering platform driver!\nRollback changes...\\n");
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return INIT_ERR;
  }
  pr_info("%s: PIT Driver initialization success!\n", MODULE_NAME);
  return INIT_SUCCESS;
}

// This is called when Linux unloads your driver
static void pit_exit(void) {
  pr_info("%s: Exiting PIT Driver!\n", MODULE_NAME);
  platform_driver_unregister(&pit_platform_driver); // unregister platform
  class_destroy(pit); // class_destroy
  unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  pr_info("%s: Finish Exit PIT Driver!\n", MODULE_NAME);
  return;
}

// Called by kernel when a platform device is detected that matches the
// 'compatible' name of this driver.
// pdev : platform device which to probe
// returns an int signalling a successful probe or some kind of error
static int pit_probe(struct platform_device *pdev) {
  pr_info("%s: Probing PIT Driver!\n", MODULE_NAME);
  if(pit_probe_called_once == true) { // we want to call this function once
    pr_info("Already called probe() once...\n");
    return PROBE_SUCCESS;
  }
  // Initialize the character device structure (cdev_init)
  cdev_init(&cdev,&pit_fops);
  cdev.owner = THIS_MODULE;
  // Register the character device with Linux (cdev_add)
  int err = cdev_add(&cdev,dev_nums,NUM_OF_CONTIGUOUS_DEVS);
  if(err < PROBE_SUCCESS) { // if err is negative, the device hasn't been added
    pr_info("Failure registering the cdev!\nRollback changes...\\n");
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.cdev = cdev;
//   // Create a device file in /dev so that the character device can be accessed
//   // from user space -- COMMENTED OUT FROM LAST LAB
//   device = device_create(pit,NULL,dev_nums,NULL,MODULE_NAME);
//   if(device == NULL) { // if the device returns null, then we hit an error
//     pr_info("Failure creating device!\nRollback changes...\\n");
//     cdev_del(&cdev);
//     platform_driver_unregister(&pit_platform_driver);
//     class_destroy(pit);
//     unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
//     return PROBE_ERR;
//   }
//   dev.dev = device;
 
 /********** NEW CODE *********/
 device = root_device_register("my_folder");
 if(device == NULL) {
   pr_info("Failure registering root device!\nRollback changes....\n");
   cdev_del(&cdev);
   platform_driver_unregister(&pit_platform_driver);
   class_destroy(pit);
   unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
   return PROBE_ERR;
 }
 dev.dev = device;
 /******** END NEW CODE *******/
 
  // Get the physical device address from the device tree
  res = platform_get_resource(pdev,IORESOURCE_MEM,FIRST_RESOURCE);
  if(res == NULL) { // if the resource returns null, then we hit an error
    pr_info("Failure Getting Resources 01!\nRollback changes...\\n");
//     device_destroy(pit,dev_nums); // device_destroy COMMENTED OUT FROM LAST LAB
    root_device_unregister(dev.dev); // maybe?
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.phys_addr = res->start;
  // Reserve the memory region -- request_mem_region
  dev.mem_size = (res->end)-(res->start)+1;
  res_mem = request_mem_region(dev.phys_addr,dev.mem_size,MODULE_NAME);
  if(res_mem == NULL) { // if the resource returns null, then we hit an error
    pr_info("Failure Requesting Memory Region!\nRollback changes...\n");
    device_destroy(pit,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
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
//     device_destroy(pit,dev_nums); // device_destroy COMMENTED OUT FROM LAST LAB
    root_device_unregister(dev.dev); // maybe?
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.pdev = pdev;
  // Register your interrupt service routine -- request_irq
  int irq_err = request_irq(irq_num,irq_isr,0,MODULE_NAME,NULL);
  if(irq_err < PROBE_SUCCESS) { // failed to register the platform driver
    pr_info("Failure calling the request_irq !\nRollback changes...\\n");
    release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
//     device_destroy(pit,dev_nums); // device_destroy COMMENTED OUT FROM LAST LAB
    root_device_unregister(dev.dev); // maybe?
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
 pit_probe_called_once = true; // makes certain we don't run probe twice
  pr_info("%s: PIT Driver probing success!\n", MODULE_NAME);
  pr_info("%s: Major Number: %zu\n", MODULE_NAME, MAJOR(dev_nums));
  pr_info("%s: Physical Addr: %zu\n", MODULE_NAME, dev.phys_addr);
  pr_info("%s: Virtual Addr: %p\n", MODULE_NAME, dev.virt_addr);
  pr_info("%s: IRQ Number: %zu\n", MODULE_NAME, irq_num);
  return PROBE_SUCCESS;
}

// removes the pit_device by unloaded and unmapping it, destroys device
// returns : an int signalling success or failure
static int pit_remove(struct platform_device * pdev) {
  pr_info("%s: Removing PIT Driver!\n", MODULE_NAME);
  free_irq(irq_num,NULL); // free the irq to allow interrupts to continue
  ioport_unmap(dev.virt_addr); // iounmap
  release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
//     device_destroy(pit,dev_nums); // device_destroy COMMENTED OUT FROM LAST LAB
  root_device_unregister(dev.dev); // maybe?
  cdev_del(&cdev); // cdev_del
  pr_info("%s: Removing PIT Driver success!\n", MODULE_NAME);
  return REMOVE_SUCCESS;
}
