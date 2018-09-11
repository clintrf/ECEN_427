/**
 * Created by:
 *      Jeremy Lyman
 *      Chad Greene
 *      Makani Price
 *
 * Brigham Young University
 * ECEN 427
 * 2019
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/version.h>
#include <asm/io.h>
#include <asm/signal.h>
#include <asm/uaccess.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("C. Greene & M. Price & J. Lyman");
MODULE_DESCRIPTION("ECEn 427 PIT Driver");

#define MODULE_NAME "pit"
#define BYTE_TO_WORD_OFFSET 0x04

#define DEVICE_NUMBER 1
#define PIT_MINOR_NUMBER 0
#define PIT_DRIVER_NAME "ecen427_user_pit"
#define FIFO_DEPTH 1024
#define BASE10 10

#define MEMORY_START 0
#define KP_ALLOCATED 1
#define IOCTL_REPEAT_COMMAND 1
#define ENABLE_INTERRUPTS 1
#define DISABLE_INTERRUPTS 0
#define DONE_READING_ADUIO 0
#define SUCCESS 0
#define UNKNOWN_COMMAND_FAILURE -1

#define PIT_CTRL_OFFSET 0x4
#define PIT_PERIOD 0xC
#define PIT_BASE_ADDR 0x43C30000

#define PLATFORM_GET_RESOURCE_INDEX 0

#define CLASS_CREATE_ERR_MESSAGE "class_create error\n\r"
#define CHARDEV_ALLOC_ERR_MESSAGE "alloc_chrdev_region error\n\r"
#define PLAT_DRV_ERR_MESSAGE "platform_driver_register error\n\r"
#define PIT_PROBE_CDEV_FAILURE_MESSAGE "\tcdev failed to add\n\r"
#define PIT_PROBE_DEVICE_CREATE_FAILURE_MESSAGE "\tdevice failed to create\n\r"
#define PIT_PROBE_PLATFORM_GET_RESOURCE_FAILURE_MESSAGE "\tplatform did not get resource\n\r"
#define PIT_PROBE_REQUEST_MEM_REGION_FAILURE_MESSAGE "\tfailed request mem region\n\r"
#define PIT_PROBE_IOREMAP_FAILURE_MESSAGE "\tfailed ioremap\n\r"
#define PIT_PROBE_CREATE_GROUP_FAILURE_MESSAGE "\tfailed create group\n\r"
// #define PIT_PROBE_IRQ_RESOURCE_FAILURE_MESSAGE "\tfailed irq get resource\n\r"
#define PIT_PROBE_ERROR_MESSAGE "\tProbe Failed\n\r"

// Function declarations
static int pit_init(void);
static void pit_exit(void);

static const char pitname[] = "ecen427_user_pit";

static s32* kp;             // kernel pointer
static uint16_t kp_alloc;   // if kp is allocated
static uint32_t control_signal = 0;

//function pointers for init and exit
module_init(pit_init);
module_exit(pit_exit);

/**
 * structure for the pit device
 */
struct pit_device{
    int minor_num;                 // Device minor number
    int major;                     // Device major number
    struct cdev cdev;              // Character device structure
    struct platform_device *pdev;  // Platform device pointer
    struct device * dev;           // device (/dev)

    phys_addr_t phys_addr;         // Physical address
    u32 mem_size;                  // Allocated mem space size
    u32 *virt_addr;                // Virtual address
};

static dev_t devt;            // device major and minor numbers
static struct class* pitClass; // Global variable for the device class

static struct pit_device pitDev; //pitDevice that contains relevant info from init

static const struct file_operations fops = {
  .owner =    THIS_MODULE,
};

static struct of_device_id pit_of_match[] = {
  { .compatible = "xlnx,xps-intc-1.00.b", },
  {}
};

static int pit_probe(struct platform_device *pdev);
static int pit_remove(struct platform_device * pdev);

MODULE_DEVICE_TABLE(of, pit_of_match);

static struct platform_driver pit_platform_driver = {
  .probe = pit_probe,
  .remove = pit_remove,
  .driver = {
    .name = "ecen427_user_pit",
    .owner = THIS_MODULE,
    .of_match_table = pit_of_match,
  },
};

#define PIT_CONTROL_OFFSET 0
#define PIT_COUNTER_OFFSET 1
#define PIT_CNTDWN_ENABLE 0x01
#define PIT_CNTDWN_DISABLE 0x00
#define PIT_INT_ENABLE 0x02
#define PIT_INT_DISABLE 0X00


ssize_t pit_counter_read(struct device * dev, struct device_attribute * attr, char * buf){
  pr_info("\tPIT COUNTER READ HAPPENED\n\r");
  pr_info("value: 0x%x\n\r",ioread32(pitDev.virt_addr+PIT_COUNTER_OFFSET));
  return 0;
}
ssize_t pit_countdown_enable_read(struct device * dev, struct device_attribute * attr, char * buf){
  pr_info("\tPIT CONTROL (coundown) READ HAPPENED\n\r");
  pr_info("value: 0x%x\n\r",ioread32(pitDev.virt_addr+PIT_CONTROL_OFFSET));
  return 0;
}
ssize_t pit_interrupt_enable_read(struct device * dev, struct device_attribute * attr, char * buf){
  pr_info("\tPIT CONTROL (interrupt) READ HAPPENED\n\r");
  pr_info("value: 0x%x\n\r",ioread32(pitDev.virt_addr+PIT_CONTROL_OFFSET));
  return 0;
}

#define WRITE_SUCCESS 32
#define WRITE_FAILURE 0
ssize_t pit_counter_write(struct device * dev, struct device_attribute * attr,
                        const char * buf, size_t count){
  pr_info("\tPIT COUNTER WRITE HAPPENED\n\r");
  int32_t writeVal;
  kstrtou32(buf, 10, &writeVal);
  pr_info("\t\twriteVal: %d\n\r", writeVal);

  pr_info("\t\tabout to write\n\r");
  iowrite32(writeVal, pitDev.virt_addr+PIT_COUNTER_OFFSET);
  pr_info("\t\tWrote %d to the counter\n\r", ioread32(pitDev.virt_addr+PIT_COUNTER_OFFSET));
  return count;
}

ssize_t pit_interrupt_enable_write(struct device * dev, struct device_attribute * attr,
                        const char * buf, size_t count){
  pr_info("\tPIT INT WRITE HAPPENED\n\r");
  int32_t enable;
  kstrtou32(buf, 10, &enable);
  pr_info("\t\tenable: %d\n\r", enable);

  //second bit, bit 1
  uint32_t writeVal = PIT_INT_ENABLE;
  control_signal = (enable) ? (writeVal | control_signal) : (~writeVal & control_signal);
  pr_info("\tcontrol_signal: 0x%x\n\r", control_signal);
  // check the pitDev.virt_addr
  iowrite32(control_signal, pitDev.virt_addr+PIT_CONTROL_OFFSET);
  return count;
}

ssize_t pit_countdown_enable_write(struct device * dev, struct device_attribute * attr,
                        const char * buf, size_t count){
  pr_info("\tPIT COUNTDOWN WRITE HAPPENED\n\r");
  int32_t enable;
  kstrtou32(buf, 10, &enable);
  pr_info("\t\tenable: %d\n\r", enable);

  //first bit, bit zero
  uint32_t writeVal = PIT_CNTDWN_ENABLE;
  control_signal = (enable) ? (writeVal | control_signal) : (~writeVal & control_signal);
  pr_info("\tcontrol_signal: 0x%x\n\r", control_signal);
  // uint32_t writeVal = (enable) ? PIT_CNTDWN_ENABLE : PIT_CNTDWN_DISABLE;
  iowrite32(control_signal, pitDev.virt_addr+PIT_CONTROL_OFFSET);
  return count;
}


static struct kobject * ko;
static struct device_attribute counter;
static struct device_attribute interrupt_enable;
static struct device_attribute countdown_enable;
static struct attribute_group pit_attr_grp;

/**
 * @brief  This is called when Linux loads the driver, initializes the driver and sets it up in system
 *        allocates chrdev region
 *        creates class
 *        registers platform driver
 * @retval error codes as based on standard error values or 0 (success)
 */
static int __init pit_init(void){
    uint8_t err;
    pr_info("%s: Beginning INIT~\n", MODULE_NAME);

    // Get a major number for the driver -- alloc_chrdev_region; // pg. 45, LDD3.
    err = alloc_chrdev_region(&devt, PIT_MINOR_NUMBER, DEVICE_NUMBER, pitname);
    //record major and minor numbers of device
    pitDev.major = MAJOR(devt);
    pitDev.minor_num = MINOR(devt);


    if (err){
        pr_info(CHARDEV_ALLOC_ERR_MESSAGE);
        goto failure_no_finished_setup;
    }
    // Create a device class. -- class_create()
    // struct class * class_create(struct moduel * owner, const char * name);
    // owner: pointer to the module that is to "own" this struct class
    // name: pointer to a string for the name of this class
    pitClass = class_create(THIS_MODULE, pitname);
    if (IS_ERR(pitClass)){
        pr_info(CLASS_CREATE_ERR_MESSAGE);
        err = PTR_ERR(pitClass);
        goto failure_unregister_chrdev_region;
    }
    pr_info("Before probe o\n\r");
    // Register the driver as a platform driver -- platform_driver_register
    int res = platform_driver_register(&pit_platform_driver);
    pr_info("After probe O\n\r");


    if (res){
      pr_info(PLAT_DRV_ERR_MESSAGE);
      err = res;
      goto failure_eliminate_class;
    }
    else
        return SUCCESS; //if no error return success

    // If any of the above functions fail, return an appropriate linux error code, and make sure
    // you reverse any function calls that were successful.

  failure_eliminate_class: //undo create device class
    class_unregister(pitClass);
    class_destroy(pitClass);
  failure_unregister_chrdev_region: // undo getting major number for driver
    unregister_chrdev_region(devt, DEVICE_NUMBER);
  failure_no_finished_setup: //nothing was set up yet
  return err;
}

/**
 * @brief  This is called when Linux unloads your driver. Frees up resources
 * @note
 * @retval None
 */
static void pit_exit(void)
{
  //undo same actions as init performs with the addition of platform driver unregister
    platform_driver_unregister(&pit_platform_driver);
    class_unregister(pitClass);
    class_destroy(pitClass);
    unregister_chrdev_region(devt, DEVICE_NUMBER);
    return;
}

/**
 * @brief  Called by kernel when a platform device is detected that matches the 'compatible' name of this driver.
 * @param  *pdev: the platform_device
 * @retval success (0) or error code of failure
 */
static int pit_probe(struct platform_device *pdev){
    pr_info("Probe initiated\n\r");
    struct resource* r;
    int err;
    cdev_init(&pitDev.cdev, &fops);
    // Register the character device with Linux (cdev_add)
    err = cdev_add(&pitDev.cdev,devt,DEVICE_NUMBER);
    if(err){
        pr_info(PIT_PROBE_CDEV_FAILURE_MESSAGE);
        goto failure_undo_cdev_init;
    }
    // Create a device file in /dev so that the character device can be accessed from user space (device_create).
    // returns pointer

    (pitDev.dev) = device_create(pitClass,NULL,devt,NULL,PIT_DRIVER_NAME);
    if(IS_ERR((pitDev.dev))) {
        pr_info(PIT_PROBE_DEVICE_CREATE_FAILURE_MESSAGE);
        err = -ENODEV;
        goto failure_destroy_device;
    }
    // Get the physical device address from the device tree -- platform_get_resource pitDev.pdev->name
    // returns resource pointer pitDev.pdev->name
    r = platform_get_resource(pdev, IORESOURCE_MEM,PLATFORM_GET_RESOURCE_INDEX);
    if(!r) {
        pr_info(PIT_PROBE_PLATFORM_GET_RESOURCE_FAILURE_MESSAGE);
        err = -ENODEV;
        goto failure_destroy_device;
    }
    pitDev.phys_addr = r->start;
    pitDev.mem_size = resource_size(r);
    // Reserve the memory region -- request_mem_region
    // start,length, char* name
    // return resource pointer

    if(!(request_mem_region(pitDev.phys_addr,pitDev.mem_size,pdev->name))){
        pr_info(PIT_PROBE_REQUEST_MEM_REGION_FAILURE_MESSAGE);
        err = -EBUSY;
        goto failure_release_mem_region;
    }interrupt_enable

    // Get a (virtual memory) pointer to the device -- ioremap
    pitDev.virt_addr = ioremap(pitDev.phys_addr,pitDev.mem_size);
     if(IS_ERR(pitDev.virt_addr)) {
       pr_info(PIT_PROBE_IOREMAP_FAILURE_MESSAGE);
       err = -ENOMEM;
       goto failure_unmap_io;
    }

    ko = &((pitDev.dev)->kobj); //TODO: check this with the reference slides

    struct attribute* bundle[4];

    counter.attr.name = "pit_counter";
    counter.attr.mode = 0666; // read and write functionality
    counter.show = pit_counter_read;
    counter.store = pit_counter_write;

    interrupt_enable.attr.name = "pit_interrupt_enable";
    interrupt_enable.attr.mode = 0666;  // read and write functionality
    interrupt_enable.show = pit_interrupt_enable_read;
    interrupt_enable.store = pit_interrupt_enable_write;

    pr_info("point 5");
    countdown_enable.attr.name = "pit_countdown_enable";
    countdown_enable.attr.mode = 0666;  // read and write functionality
    countdown_enable.show = pit_countdown_enable_read;
    countdown_enable.store = pit_countdown_enable_write;

    // Initialize each attribute
    bundle[0] = &counter.attr;
    bundle[1] = &interrupt_enable.attr;
    bundle[2] = &countdown_enable.attr;
    bundle[3] = NULL;

    pr_info("point 7");
    pit_attr_grp.attrs = bundle;

    pr_info("point 8");
    // Register the group of attributes
    if(sysfs_create_group(&((pitDev.dev)->kobj), &pit_attr_grp)){
      // Handle error
      pr_info(PIT_PROBE_CREATE_GROUP_FAILURE_MESSAGE);
      goto failure_create_group;
    }

    // // Get the IRQ number from the device tree -- platform_get_resource
    // irq = platform_get_resource(pdev,IORESOURCE_IRQ,PLATFORM_GET_RESOURCE_INDEX);
    // if(!irq){
    //     pr_info(PIT_PROBE_IRQ_RESOURCE_FAILURE_MESSAGE);
    //     err = -ENODEV;
    //     goto failure_unmap_io;
    // }
    pr_info("\tPhysical Address %u\n\r",pitDev.phys_addr);
    pr_info("\tMemory Size %u\n\r",pitDev.mem_size);
    pr_info("\tVirtual Address %u\n\r",(uint32_t)(pitDev.virt_addr));
    return SUCCESS;


    // If any of the above functions fail, return an appropriate linux error code, and make sure
    // you reverse any function calls that were successful.



  failure_create_group:
      //remove file that was just created
      sysfs_remove_group(&((pitDev.dev)->kobj), &pit_attr_grp);
      // sysfs_remove_file(ko, &countdown_enable.attr);
  // failure_cnt_enable:
      // sysfs_remove_file(ko, &interrupt_enable.attr);
  // failure_int_enable:
      // sysfs_remove_file(ko, &counter.attr);
  // failure_counter:
  failure_unmap_io:
      iounmap(pitDev.virt_addr);
  failure_release_mem_region:
      release_mem_region(pitDev.phys_addr, pitDev.mem_size);
  failure_destroy_device:
      device_destroy(pitClass,devt);
  failure_undo_cdev_init:
      unregister_chrdev_region(devt,DEVICE_NUMBER);
      cdev_del(&pitDev.cdev);
      pr_info(PIT_PROBE_ERROR_MESSAGE);
      return err;
}


/**
 * @brief  Removes the driver by freeing up any allocated resources and unmapping memory
 * @note
 * @param  *pdev: Device to remove
 * @retval 0 for success probably will cause seg break if it fails
 */
static int pit_remove(struct platform_device *pdev)
{
  pr_info("\tREMOVING PIT\n\r");
  //free allocated kernel space made during write

  //Remove the attribute group made for sysfs
  pr_info("\t\tremoving group\n\r");
  sysfs_remove_group(&((pitDev.dev)->kobj), &pit_attr_grp);
  pr_info("\t\tremoving group success\n\r");

  //unmap io virtual memory
  iounmap(pitDev.virt_addr);
  pr_info("\t\tiounmapped\n\r");

  //release claimed memory region
  release_mem_region(pitDev.phys_addr, pitDev.mem_size);
  pr_info("\t\treleased mem region\n\r");

  //destroy device
  device_destroy(pitClass,devt);
  pr_info("\t\tdestroyed device\n\r");

  //unregister driver space
  // unregister_chrdev_region(devt,DEVICE_NUMBER);
  // pr_info("\t\tunregistered chrdev region\n\r");

  //delete cdev
  cdev_del(&pitDev.cdev);
  pr_info("\t\tdeleted cdev\n\r");

  return SUCCESS;
}
