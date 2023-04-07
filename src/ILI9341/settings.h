#ifndef _SETTINGS_H_
#define _SETTINGS_H_


//----- DEFINES -----
#define VERSION         "0.07b"

//LCD support (define only one)
#define LCD_MI0283QT9 //MI0283QT-9/-11 (ILI9341)
//#define LCD_UG12D228AA
//#define LCD_QVGA_TFT_SPI_240x320

#ifdef LCD_MI0283QT9
#define TP_SUPPORT //resistive touch panel support
#endif

#define DEFAULT_CLOCK   48000000UL    //Hz, 12 16* 24 32* 36 48* MHz (*crystal)
#define DEFAULT_POWER   0             //0-100 power for backlight
#define DEFAULT_LDRTIME 500           //check LDR every 500 milliseconds
#define DEFAULT_INTERF  INTERFACE_I2C //INTERFACE_I2C INTERFACE_SPI INTERFACE_UART
#define DEFAULT_BAUD    9600          //UART baud rate
#define DEFAULT_ADDR    0x20          //I2C address
#define DEFAULT_ORDER   0             //byte order (0=big, 1=little)




#endif //_SETTINGS_H_
