#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include<linux/sysfs.h>
#include<linux/kobject.h>
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
#define PIT_CTRL_OFFSET 0x0
#define PIT_CNTR_OFFSET 1
#define FIRST_MINOR 0
#define NUM_OF_CONTIGUOUS_DEVS 1
#define FIRST_RESOURCE 0
#define INIT_SUCCESS 0
#define INIT_ERR -1
#define PROBE_SUCCESS 0
#define PROBE_ERR -1
#define REMOVE_SUCCESS 0
#define BUNDLE_NUMBER 4
#define SHOW_SUCCESS 0
#define BUFFER_SIZE 10
#define PIT_CNTDWN_ENABLE 0x01
#define PIT_INT_ENABLE 0x02

/********************************* prototypes ********************************/
static int pit_probe(struct platform_device *pdev);
static int pit_remove(struct platform_device * pdev);
static ssize_t int_show(struct device *dev, struct device_attribute *attr,
    char *buf);
static ssize_t int_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count);
static ssize_t timer_show(struct device *dev, struct device_attribute *attr,
    char *buf);
static ssize_t timer_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count);
static ssize_t period_show(struct device *dev, struct device_attribute *attr,
    char *buf);
static ssize_t period_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count);

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
  { .compatible = "xlnx,xps-intc-1.00.b", },
  {}
};
MODULE_DEVICE_TABLE(of, pit_of_match);

// struct containing the platform driver
static struct platform_driver pit_platform_driver = {
  .probe = pit_probe,
  .remove = pit_remove,
  .driver = {
    .name = "ecen427_user_pit",
    .owner = THIS_MODULE,
    .of_match_table = pit_of_match,
  }
};

/****************************** global variables ****************************/
static pit_dev dev; // global pit device
static bool pit_probe_called_once = false; // makes pit_probe call once
static dev_t dev_nums; // contains major and minor numbers"byu,ecen427-pit"
static struct class *pit;
static struct device *device;
static struct cdev cdev; // character device for the driver
static struct resource *res; // Device Resource Structure
static struct resource *res_mem; // Device Resource Structure
struct attribute * bundle[BUNDLE_NUMBER];
struct attribute_group my_attr_grp;
static struct device_attribute dattr_int = __ATTR_RW(int);
static struct device_attribute dattr_timer = __ATTR_RW(timer);
static struct device_attribute dattr_period = __ATTR_RW(period);
static struct kobject * ko;

/***************************** kernel definitions ****************************/
static int pit_init(void);
static void pit_exit(void);
static const char pitname[] = "ecen427_user_pit";
static s32* kp;             // kernel pointer
static uint16_t kp_alloc;   // if kp is allocated
static uint32_t control_signal = 0;
module_init(pit_init);
module_exit(pit_exit);

/********************************** functions ********************************/
// This is called when Linux loadrite (buffer);s your driver
// returns : an int signalling a successful initialization or an error
static int pit_init(void) {
  pr_info("%s: Initializing PIT Driver!\n", MODULE_NAME);
  // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
  int err = alloc_chrdev_region(&dev_nums,FIRST_MINOR,NUM_OF_CONTIGUOUS_DEVS,
    pitname);
  if(err < INIT_SUCCESS) { // failure doing region allocation
    pr_info("Failure allocating major/minor numbers!\n");
    return INIT_ERR;
  }
  int minor_num = MINOR(dev_nums); // returns the minor number of a dev_t type
  dev.minor_num = minor_num;
  // Create a device class. -- class_create()
  pit = class_create(THIS_MODULE,pitname);
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

// called when the sysfs file is read
// dev : the device affected
// attr : the attribute affected
// buf : the number of bytes you want to pass back
// returns : the number of bytes placed into the buf
static ssize_t int_show(struct device *dev, struct device_attribute *attr,
    char *buf) {
  pr_info("Value Read: 0x%x\n\r",ioread32(dev.virt_addr+PIT_CTRL_OFFSET));
  return SHOW_SUCCESS;
}

// is called when the sysfs file is written to
// dev : the device affected
// attr : the attribute affected
// buf : a buffer containing the data to write
// count : the number of bytes of data in the buffer
// returns the number of bytes consumed
static ssize_t int_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count) {
  int32_t enable;
  kstrtou32(buf, BUFFER_SIZE, &enable); // converts input from command line
  pr_info("enable: %d\n\r", enable);
  uint32_t writeVal = PIT_INT_ENABLE; // writes an enable value to the int reg
  control_signal = (enable) ? (writeVal | control_signal) :
    (~writeVal & control_signal); // compute control signal
  pr_info("control_signal: 0x%x\n\r", control_signal);
  iowrite32(control_signal, dev.virt_addr+PIT_CTRL_OFFSET); // write sig
  return count;
}

// called when the sysfs file is read
// dev : the device affected
// attr : the attribute affected
// buf : the number of bytes you want to pass back
// returns : the number of bytes placed into the buf
static ssize_t timer_show(struct device *dev, struct device_attribute *attr,
    char *buf) {
  pr_info("Value Read: 0x%x\n\r",ioread32(dev.virt_addr+PIT_CTRL_OFFSET));
  return SHOW_SUCCESS;
}

// is called when the sysfs file is written to
// dev : the device affected
// attr : the attribute affected
// buf : a buffer containing the data to write
// count : the number of bytes of data in the buffer
// returns the number of bytes consumed
static ssize_t timer_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count) {
  pr_info("\tPIT COUNTDOWN WRITE HAPPENED\n\r");
  int32_t enable;
  kstrtou32(buf, BUFFER_SIZE, &enable); // converts input from command line
  pr_info("enable: %d\n\r", enable);
  uint32_t writeVal = PIT_CNTDWN_ENABLE; // writes an enable value to the reg
  control_signal = (enable) ? (writeVal | control_signal) :
    (~writeVal & control_signal); // compute control signal
  pr_info("\tcontrol_signal: 0x%x\n\r", control_signal);
  iowrite32(control_signal, dev.virt_addr+PIT_CTRL_OFFSET); // write sig
  return count;
}

// called when the sysfs file is read
// dev : the device affected
// attr : the attribute affected
// buf : the number of bytes you want to pass back
// returns : the number of bytes placed into the buf
static ssize_t period_show(struct device *dev, struct device_attribute *attr,
    char *buf) {
  pr_info("Value Read: 0x%x\n\r",ioread32(dev.virt_addr+PIT_CNTR_OFFSET));
  return SHOW_SUCCESS;
}

// is called when the sysfs file is written to
// dev : the device affected
// attr : the attribute affected
// buf : a buffer containing the data to write
// count : the number of bytes of data in the buffer
// returns the number of bytes consumed
static ssize_t period_store(struct device *dev, struct device_attribute *attr,
    const char *buf, size_t count) {
  pr_info("\tPIT COUNTER WRITE HAPPENED\n\r");
  int32_t writeVal; // the value to be written to the reg
  kstrtou32(buf, BUFFER_SIZE, &writeVal); // converts the value from str to int
  pr_info("Value to be written: %d\n\r", writeVal);
  iowrite32(writeVal, dev.virt_addr+PIT_CNTR_OFFSET); // writes the val
  pr_info("Wrote %d to the counter\n\r",
    ioread32(dev.virt_addr+PIT_CNTR_OFFSET));
  return count;
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

  // Create a device file in /dev so that the character device can be accessed
  // from user space -- COMMENTED OUT FROM LAST LAB
  device = device_create(pit,NULL,dev_nums,NULL,MODULE_NAME);
  if(device == NULL) { // if the device returns null, then we hit an error
    pr_info("Failure creating device!\nRollback changes...\\n");
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  dev.dev = device;


  // Get the physical device address from the device tree
  res = platform_get_resource(pdev,IORESOURCE_MEM,FIRST_RESOURCE);
  if(res == NULL) { // if the resource returns null, then we hit an error
    pr_info("Failure Getting Resources 01!\nRollback changes...\\n");
    device_destroy(pit,dev_nums); // device_destroy
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
  dev.pdev = pdev; // sets the platform driver

  ko = &(dev.dev->kobj);
  struct attribute* bundle[BUNDLE_NUMBER]; // need 3 attributes and NULL

  // Take attributes and bundle them together
  bundle[0] = &dattr_period.attr;
  bundle[1] = &dattr_int.attr;
  bundle[2] = &dattr_timer.attr;
  bundle[3] = NULL;
  // create a group from those attributes
  my_attr_grp.attrs = bundle;
  // create the groups files that will be edited
  if(sysfs_create_group(ko,&(my_attr_grp))) {
    pr_info("Failure creating group!\nRollback changes...\\n");
    device_destroy(pit,dev_nums); // device_destroy
    cdev_del(&cdev);
    platform_driver_unregister(&pit_platform_driver);
    class_destroy(pit);
    unregister_chrdev_region(dev_nums,NUM_OF_CONTIGUOUS_DEVS);
    return PROBE_ERR;
  }
  // check and print off last few items to ensure that everything is working
  pit_probe_called_once = true; // makes certain we don't run probe twice
  pr_info("%s: PIT Driver probing success!\n", MODULE_NAME);
  pr_info("%s: Major Number: %zu\n", MODULE_NAME, MAJOR(dev_nums));
  pr_info("%s: Physical Addr: %zu\n", MODULE_NAME, dev.phys_addr);
  pr_info("%s: Virtual Addr: %p\n", MODULE_NAME, dev.virt_addr);
  return PROBE_SUCCESS;
}

// removes the pit_device by unloaded and unmapping it, destroys device
// returns : an int signalling success or failure
static int pit_remove(struct platform_device * pdev) {
  pr_info("%s: Removing PIT Driver!\n", MODULE_NAME);
  ioport_unmap(dev.virt_addr); // iounmap
  sysfs_remove_group(root,&(my_attr_grp)); // remove the group
  release_mem_region(dev.phys_addr,dev.mem_size); // release_mem_region
  device_destroy(pit,dev_nums); // device_destroy
  cdev_del(&cdev); // cdev_del
  pr_info("%s: Removing PIT Driver success!\n", MODULE_NAME);
  return REMOVE_SUCCESS;
}
