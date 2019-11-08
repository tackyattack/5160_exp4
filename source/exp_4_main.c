#include <stdio.h>
#include "main.h"
#include "PORT.H"
#include "Directory_Functions_struct.h"
#include "file_system.h"

xdata uint8_t buf1[512];

main()
{
   uint8_t input_value, error_flag;
   uint16_t entry_num;
   uint32_t cwd, clus;

   LEDS_ON(Red_LED);
   AUXR=0x0c;   // make all of XRAM available, ALE always on
   if(OSC_PER_INST==6)
   {
       CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
       CKCON0=0x00;  // set standard clock mode
   }
   Timer0_DELAY_1ms(300);
   uart_init(9600);


   LCD_Init();

   error_flag=SPI_Master_Init(400000UL);
   if(error_flag!=no_errors)
   {
      LEDS_ON(Red_LED);  // An error causes the program to stop
	  UART_Transmit('S');
      while(1);
   }
   LEDS_ON(Amber_LED);
   // SD Card Initialization
   error_flag=SD_card_init();
   if(error_flag!=no_errors)
   {
      LEDS_ON(Red_LED);  // An error causes the program to stop
	  UART_Transmit('C');
      while(1);
   }
   LEDS_OFF(Amber_LED);
   error_flag=SPI_Master_Init(20000000UL);
   if(error_flag!=no_errors)
   {
      LEDS_ON(Red_LED);  // An error causes the program to stop
	    UART_Transmit('P');
      while(1);
   }

   printf("Mounting SD card\n");
   if(mount_drive(buf1) == MOUNT_SUCCESS)
   {
     printf("SD card mounted\n");
   }
   else
   {
     printf("SD card failed to mount\n");
     while(1);
   }

   printf("Root directory:\n\n");
   cwd = Print_Directory(Export_Drive_values()->FirstRootDirSec, buf1);


   while(1)
   {
     printf("Enter an entry number: ");
     entry_num = (uint16_t)long_serial_input();
     // check to make sure entry is within the cwd
     if(entry_num <= Print_Directory(cwd, buf1))
     {
       clus = Read_Dir_Entry(cwd, entry_num, buf1);
       if(clus & directory_bit)
       {
         clus &= 0x0FFFFFFF; // mask off upper four bits to print another directory
         cwd = first_sector(clus);
       }
       else
       {
         print_file(clus, buf1);
       }
     }
     else
     {
       printf("Error: invalid entry choice\n");
     }
   }
}
