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
MODULE_DESCRIPTION("ECEn 427 Audio Driver");

#define MODULE_NAME "audio"
#define I2S_DATA_RX_L_REG_OFFSET 0 // input for left channel (low 24 bits)
#define I2S_DATA_RX_R_REG_OFFSET 1 // input for right channel (low 24 bits)
#define I2S_DATA_TX_R_REG_OFFSET 2 // output for right channel (low 24 bits)
#define I2S_DATA_TX_L_REG_OFFSET 3 // output for left channel (low 24 bits)
/*
 * Various statuses are held in this register
 * [0] : control interrupt generation (0 disabled, 1 enabled)
 * [10-1] : number of values currently in the audio FIFO right channel
 * [20-11] : number of values currently in the audio FIFO left channel
 * [21] : each time a new audio sample is available, this is 1, must be cleared
*/
#define I2S_STATUS_REG_OFFSET 4
#define I2S_LEFT_FIFO_STATUS 11
#define I2S_RIGHT_FIFO_STATUS 1
#define TX_DATACOUNT_L_MASK 2095104 // 0x00000000000111111111100000000000
#define TX_DATACOUNT_R_MASK 2046    // 0x00000000000000000000011111111110
#define FIRST_MINOR 0
#define NUM_OF_CONTIGUOUS_DEVS 1
#define FIRST_RESOURCE 0
#define SECOND_RESOURCE 1
#define INIT_SUCCESS 0
#define INIT_ERR -1
#define PROBE_SUCCESS 0
#define PROBE_ERR -1
#define REMOVE_SUCCESS 0
#define IOCTL_SUCCESS 0
#define IOCTL_ERR -1
#define INTERRUPTS_OFF 0x0
#define INTERRUPTS_ON 0x1
#define ZERO_BYTES_WRITTEN 0
#define FIFO_BUFFER_LIMIT 1023
#define WRITE_ERR -1
#define SOUND_NOT_PLAYING 0
#define SOUND_PLAYING 1
#define IOC_MAGIC  'k'
#define TURN_ON_LOOPING _IO(IOC_MAGIC, 1)
#define TURN_OFF_LOOPING _IO(IOC_MAGIC, 2)
#define BYTES_PER_WORD 4
/********************************* prototypes ********************************/
static ssize_t audio_read(struct file *f, char *buf, size_t len, loff_t *off);
static ssize_t audio_write(struct file *f, const char *buf, size_t len,
    loff_t *off);
static void check_full(void);// added by seth
static int audio_probe(struct platform_device *pdev);
static int audio_remove(struct platform_device * pdev);
static long audio_ioctl(struct file *f, unsigned int cmd,unsigned long arg);
uint32_t fifo_index = 0;

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

// struct containing the file operations data
static struct file_operations audio_fops = {
  .owner = THIS_MODULE,
  .read = audio_read,
  .write = audio_write,
  .unlocked_ioctl = audio_ioctl
};

// Link between the hardware and its driver
static struct of_device_id audio_of_match[] = {
  { .compatible = "byu,ecen427-audio_codec", },
  {}
};
MODULE_DEVICE_TABLE(of, audio_of_match);

// struct containing the platform driver
static struct platform_driver audio_platform_driver = {
  .probe = audio_probe,
  .remove = audio_remove,
  .driver = {
    .name = MODULE_NAME,
    .owner = THIS_MODULE,
    .of_match_table = audio_of_match,
  }
};

/****************************** global variables ****************************/
static audio_dev dev; // global audio device
static bool audio_probe_called_once = false; // makes audio_probe call once
static bool fifo_data_buffer_alloc = false; // needed to alloc space for fifo
static dev_t dev_nums; // contains major and minor numbers
static struct class *audio;
static struct device *device;
static struct cdev cdev; // character device for the driver
static struct resource *res; // Device Resource Structure
static struct resource *res_mem; // Device Resource Structure
static struct resource *res_irq; // Device Resource Structure
static unsigned int irq_num; // contains the irq number
static u32 *fifo_data_buffer = NULL;
static unsigned long buf_len = 0;
static bool isEmpty = false;
static bool isFull = false;
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
static ssize_t audio_read(struct file *f, char *buf, size_t len, loff_t *off) {
  printk(KERN_INFO "Driver: read()\n");
  check_full();
  if(isEmpty) { return SOUND_NOT_PLAYING; }
  else { return SOUND_PLAYING; }
}

// reads a certain amount of bytes from a buffer
// f : the file to read from
// buf : accepts a signed 32 bit buffer containing an audio clip
// len : the number of bytes to write
// off : indicates the file position the user is accessing
// returns how many byteirq_locs were written
static ssize_t audio_write(struct file *f, const char *buf, size_t len,
    loff_t *off) {
  printk(KERN_INFO "Driver: Write()\n");
  buf_len = len;
  // Immediately disable interrupts from the audio core.
  iowrite32(INTERRUPTS_OFF,(dev.virt_addr)+I2S_STATUS_REG_OFFSET);
  // Free the buffer used to store the old sound sample if applicable
  if(fifo_data_buffer != NULL) { // check if there is anything inside the fifo
      kfree(fifo_data_buffer);
      printk("Write: Needed to free FIFO before continuing...\n");
      fifo_data_buffer = NULL;
  }
  else {
    printk("Write: First iteration, did not free FIFO.\n");
  }
  // allocate a buffer for the new clip (kmalloc).
  fifo_data_buffer = kmalloc((len)*BYTES_PER_WORD, GFP_KERNEL);
  fifo_index = 0;
  fifo_data_buffer_alloc = true;
  if (!fifo_data_buffer) { // allocation failed, need to free pointers
    printk(KERN_INFO "kmalloc Error\n");
    kfree(fifo_data_buffer);
    fifo_data_buffer_alloc = false;
    return ZERO_BYTES_WRITTEN; // zero bytes were written, the write failed.
  }

  // Copy the audio data from userspace to your newly allocated buffer
  // (including safety checks on the userspace pointer) - LDD page 64.
  unsigned int bytes_written = copy_from_user(fifo_data_buffer,buf,
    (len)*BYTES_PER_WORD);

  // check to see if we have written any bytes
  if(bytes_written < ZERO_BYTES_WRITTEN){
    printk(KERN_INFO "Audio Write Error\n");
    return WRITE_ERR;
  }
  // Make sure the audio core has interrupts enabled.
  iowrite32(INTERRUPTS_ON,(dev.virt_addr)+I2S_STATUS_REG_OFFSET);
  //enable_irq(irq_num);
  pr_info("IRQ_ISR: Interrupts enabled!!\n");
  return bytes_written;
}

// used to check if the fifos are full or not.
// returns true is the FIFO is full and false if there is space in it
static void check_full(void) {
  isFull = false;
  isEmpty = false;
  // Determine how much free space is in the audio FIFOs
  // Only need to check 1 tx_data because they empty at the same time
  unsigned int raw_data = ioread32((dev.virt_addr)+I2S_STATUS_REG_OFFSET);
  uint32_t DataL = (raw_data&TX_DATACOUNT_L_MASK)>>I2S_LEFT_FIFO_STATUS;

  if(DataL == 0) { // check if the FIFO is empty
    isFull = false;
    isEmpty = true;
  }
  else if(DataL < FIFO_BUFFER_LIMIT) { // check to see if the FIFO is not full
    isFull = false;
    isEmpty = false;
  }
  else { // check if the FIFO is full
    isFull = true;
    isEmpty = false;
  }
}

// function that handles the irq
// irq : irq number
// dev_id : the device id
// returns a flag stating if the irq was handled properly
static irqreturn_t irq_isr(int irq_loc, void *dev_id) {
  pr_info("IRQ_ISR: Calling the irq_isr!\n");
  // Determine how much free space is in the audio FIFOs
  if(fifo_data_buffer_alloc) { // only write if space is allocated to the fifo
    check_full();
    while(!isFull) { // while the FIFO is not full, loop here
      if(fifo_index < buf_len) { // write the info in the buffer until the end
        iowrite32(fifo_data_buffer[fifo_index],
          (dev.virt_addr)+I2S_DATA_TX_L_REG_OFFSET);
        iowrite32(fifo_data_buffer[fifo_index],
          (dev.virt_addr)+I2S_DATA_TX_R_REG_OFFSET);
        fifo_index++;
      }
      else { // when we reach the end of the buffer, turn off interrupts
        iowrite32(INTERRUPTS_OFF,(dev.virt_addr)+I2S_STATUS_REG_OFFSET);
        fifo_index = 0;
        fifo_data_buffer_alloc = false;
        return IRQ_HANDLED;
      }
      check_full(); // check to see if the FIFO is full or not
    }
  }
  return IRQ_HANDLED;
}

// Handles the audio looping for the audio
// f : necessary for prototype, but serves no purpose in our implementation
// cmd : the command to turn on looping or turn off looping
// arg : necessary for prototype, but serves no purpose in our implementation
// returns a long signalling error or success
static long audio_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
  switch(cmd) { // You should support two ioctl commands
    // Turn on looping for the current audio clip.
    case TURN_ON_LOOPING:
    {
      printk("AUDIO_IOCTL: TURN_ON_LOOPING");
       // turn on the register to write
      iowrite32(INTERRUPTS_ON,(dev.virt_addr)+I2S_STATUS_REG_OFFSET);
      break;
    }
    //Turn off looping for the current audio clip.
    case TURN_OFF_LOOPING:
    {
      printk("AUDIO_IOCTL: TURN_OFF_LOOPING");
      // turn off the register
      iowrite32(INTERRUPTS_OFF,(dev.virt_addr)+I2S_STATUS_REG_OFFSET);
      break;
    }
    default: // default case, should not reach this point
    {
      printk("AUDIO_IOCTL: Default case, no valid cmd given");
      return IOCTL_ERR;
    }
  }
  return IOCTL_SUCCESS;
}

/********************************** functions ********************************/
// This is called when Linux loadrite (buffer);s your driver
// returns : an int signalling a successful initialization or an error
static int audio_init(void) {
  pr_info("%s: Initializing Audio Driver!\n", MODULE_NAME);
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
