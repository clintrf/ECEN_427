
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

/****************************** global variables ****************************/
volatile int pit_value = 0;

static dev_t dev = 0; 
static struct class *pit;
static struct cdev pit_value; 
struct kobject *kobj_ref;

/****************************** Driver Functions *****************************/
static int __init pit_driver_init(void);
static void __exit pit_driver_exit(void);
static int pit_probe(struct inode *inode, struct file *file); //like open
static int pit_remove(struct inode *inode, struct file *file); //release
static ssize_t pit_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static int pit_get(struct inode *inode, struct file *file);
static int pit_set(struct inode *inode, struct file *file);
static int pit_start(struct inode *inode, struct file *file);
static int pit_stop(struct inode *inode, struct file *file);
// static int pit_interrupts_on(struct inode *inode, struct file *file);
// static int pit_interrupts_off(struct inode *inode, struct file *file);

/*************** Sysfs Fuctions **********************/
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);

/*********************************** structs *********************************/
// struct containing the audio_device data
typedef struct audio_device {
    int minor_num;                      // Device minor number
    struct cdev cdev;                   // Character device structure
    struct platform_device * pdev;      // Platform device pointer
    struct device* dev;                 // device (/dev)
    phys_addr_t phys_addr;              // Physical address
    u32 mem_size;                       // Allocated mem space size
    u32* virt_addr;                     // Virtual address
    // Add any items to this that you need
} audio_dev;

typedef struct attribute {   
    char *           name;  // Name of file
    struct module *  owner; 
    umode_t          mode;  // Oct
    permissions
};

static struct pit_attribute { 
    struct attribute attr; 
    ssize_t (*show)( 
        struct device * dev,  
        structdevice_attribute *attr,
        char * buf 
    );
    ssize_t (*store)(
         stuct device * dev,
         structdevice_attribute * arrr,
         const char * buf, 
         size_t count 
    ); 
};

static struct pit_attribute my_pit_attr;
 
static struct file_operations pit_fops = {
    .owner          = THIS_MODULE,
    .name           = MODULE_NAME,
    .read           = pit_read,
    .write          = pit_write,
    .probe          = pit_probe,     //open
    .remove         = pit_remove,  //release
};

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

/****************************** global variables ****************************/
static pit_dev dev; // global pit device
static dev_t dev_nums; // contains major and minor numbers
static struct class *pit;
static struct device *device;
static struct cdev cdev; // character device for the driver
static struct resource *res; // Device Resource Structure
static struct resource *res_mem; // Device Resource Structure
static struct resource *res_irq; // Device Resource Structure
static unsigned int irq_num; // contains the irq number

/***************************** kernel definitions ****************************/
static int pit_init(void);
static void pit_exit(void);

module_init(pit_init);
module_exit(pit_exit);

// function that handles the irq
// irq : irq number
// dev_id : the device id
// returns a flag stating if the irq was handled properly
static irqreturn_t irq_isr(int irq_loc, void *dev_id) {
  pr_info("IRQ_ISR: Calling the irq_isr!\n");
}

// Extend your kernel driver to add ioctl to the list of file operations supported by your character device
//
//long pit_ioctl(int fd, unsigned int cmd,unsigned long arg)// or ,
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
  pr_info("%s: Initializing PIT Driver!\n", MODULE_NAME);
  
  return INIT_SUCCESS;
}

// This is called when Linux unloads your driver
static void pit_exit(void) {
  pr_info("%s: Exiting PIT Driver!\n", MODULE_NAME);
  return;
}

// Called by kernel when a platform device is detected that matches the
// 'compatible' name of this driver.
// pdev : platform device which to probe
// returns an int signalling a successful probe or some kind of error
static int pit_probe(struct platform_device *pdev) {
  pr_info("%s: Probing PIT Driver!\n", MODULE_NAME);
  return PROBE_SUCCESS;
}

// removes the pit_device by unloaded and unmapping it, destroys device
// returns : an int signalling success or failure
static int pit_remove(struct platform_device * pdev) {
  pr_info("%s: Removing PIT Driver!\n", MODULE_NAME);
  return REMOVE_SUCCESS;
}
