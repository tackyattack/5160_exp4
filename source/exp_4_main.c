#include <stdio.h>
#include "main.h"
#include "PORT.H"
#include "Directory_Functions_struct.h"

xdata uint8_t buf1[512];

main()
{
   uint8_t input_value, error_flag;

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
   Print_Directory(Export_Drive_values()->FirstRootDirSec, buf1);


   while(1)
   {
     input_value=UART_Receive();
     UART_Transmit(input_value);
   }
}
