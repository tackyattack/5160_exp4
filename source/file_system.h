#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "main.h"

//------- Public Constant definitions --------------------------------
#define MOUNT_SUCCESS (0)
#define MOUNT_FAIL (1)


// ------ Public function prototypes -------------------------------
uint8_t mount_drive(uint8_t xdata *xram_data_array);

#endif