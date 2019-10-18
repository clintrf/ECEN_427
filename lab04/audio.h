#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>

// This is called when Linux loads your driver
static int audio_init(void);

// This is called when Linux unloads your driver
static void audio_exit(void);

/*
*  Called by kernel when a platform device is detected that matches the
*  'compatible' name of this driver.
*  pdev :
*  returns :
*/
static int audio_probe(struct platform_device *pdev);

/*
*  
*  pdev :
*  returns :
*/
static int audio_remove(struct platform_device * pdev);
