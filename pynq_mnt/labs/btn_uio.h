/*
* btn_uio_.h
*
* ECEn 427
* Clint Frandsen, Dax Eckles
* BYU 2019
*/

#include <stdint.h>


/*********************************** macros ***********************************/
#define UIO_BTN_ERROR      -1      //error return value
#define UIO_BTN_SUCCESS     0       //success return value
#define MMAP_OFFSET     0


/**************************** function prototypes *****************************/
// initializes the uio driver
int32_t btn_init(char devDevice[]);

// write to a register of the UIO device
void btn_write(uint32_t offset, uint32_t value);

// read from a register of the UIO device
uint32_t btn_read(uint32_t offset);

// close the UIO device
void btn_exit();


