#include <stdint.h>
#include "chip.h"
#include "settings.h"
//#include "main.h"
#include "lcd.h"
#include "lcd_mi0283qt9.h"

uint_least32_t get_ms(void);
void delay_ms(uint_least32_t delay);


//adaptacion a SPI


#ifdef LCD_MI0283QT9

#define LCD_PORT        2
#define RST_PIN         21
#define RS_PIN          13

#define SPI0

#ifdef SPI0
	#define LPC_SSP LPC_SSP0
	#define SPI_SSEL 0,16
	#define SPI_SCK 0,15
	#define SPI_MISO 0,17
	#define SPI_MOSI 0,18
#endif
#ifdef SPI1
	#define LPC_SSP LPC_SSP1
	#define SPI_SSEL 0,6
	#define SPI_SCK 0,7
	#define SPI_MISO 0,8
	#define SPI_MOSI 10,9
#endif


void INIT_PINS()
{
	uint32_t i;

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, RST_PIN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,0 , RST_PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, LCD_PORT, RS_PIN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LCD_PORT, RS_PIN);

	/*Habilito el controlador de SPI*/
	for(i=0;i<500000;i++);

	Chip_IOCON_PinMuxSet(LPC_IOCON,SPI_SSEL,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,SPI_SSEL);
	Chip_GPIO_SetPinOutLow(LPC_GPIO,SPI_SSEL);

	Chip_IOCON_PinMuxSet(LPC_IOCON,SPI_SCK,IOCON_FUNC2);
	Chip_IOCON_PinMuxSet(LPC_IOCON,SPI_MISO,IOCON_FUNC2);
	Chip_IOCON_PinMuxSet(LPC_IOCON,SPI_MOSI,IOCON_FUNC2);

	Chip_SSP_Init(LPC_SSP);
	Chip_SSP_Enable(LPC_SSP);
	//Chip_SPI_Init(LPC_SPI);



}


/*
__attribute__((always_inline)) __INLINE void lcd_drawstop(void)
{
  //lcd_disable();

  return;
}
*/

void lcd_draw(uint_least16_t color)
{
  return lcd_wrdata16(color);
}


void lcd_drawstart(void)
{
  //lcd_enable();

  lcd_wrcmd8(LCD_CMD_WRITE);

  return;
}


void lcd_setarea(uint_least16_t x0, uint_least16_t y0, uint_least16_t x1, uint_least16_t y1)
{
  //lcd_enable();

  lcd_wrcmd8(LCD_CMD_COLUMN);
  lcd_wrdata16(y0);
  lcd_wrdata16(y1);

  lcd_wrcmd8(LCD_CMD_PAGE);
  lcd_wrdata16(x0);
  lcd_wrdata16(x1);

  //lcd_disable();

  return;
}


uint_least32_t lcd_setbias(uint_least16_t o)
{
  uint_least32_t w, h;
  uint_least8_t param;

  #define MEM_Y   (7) //MY row address order
  #define MEM_X   (6) //MX column address order 
  #define MEM_V   (5) //MV row / column exchange 
  #define MEM_L   (4) //ML vertical refresh order
  #define MEM_H   (2) //MH horizontal refresh order
  #define MEM_BGR (3) //RGB-BGR Order 

  switch(o)
  {
    default:
    case 0:
    //case 36:
    //case 360:
      w     = LCD_WIDTH;
      h     = LCD_HEIGHT;
      param = (1<<MEM_BGR) | (1<<MEM_X) | (1<<MEM_Y);
      break;
    case 90:
      w     = LCD_HEIGHT;
      h     = LCD_WIDTH;
      param = (1<<MEM_BGR) | (1<<MEM_X) | (1<<MEM_V);
      break;
    case 180:
      w     = LCD_WIDTH;
      h     = LCD_HEIGHT;
      param = (1<<MEM_BGR) | (1<<MEM_L);
      break;
    case 270:
      w     = LCD_HEIGHT;
      h     = LCD_WIDTH;
      param = (1<<MEM_BGR) | (1<<MEM_Y) | (1<<MEM_V);
      break;
  }

  //lcd_enable();

  lcd_wrcmd8(LCD_CMD_MEMACCESS_CTRL);
  lcd_wrdata8(param);

  lcd_setarea(0, 0, w-1, h-1);

  //lcd_disable();

  return (w<<16) | (h<<0);
}


void lcd_invert(uint_least8_t on)
{
  lcd_enable();

  if(on == 0)
  {
     lcd_wrcmd8(LCD_CMD_INV_OFF);
  }
  else
  {
     lcd_wrcmd8(LCD_CMD_INV_ON);
  }

  lcd_disable();

  return;
}


void lcd_power(uint_least8_t on)
{
  lcd_enable();

  if(on == 0)
  {
    lcd_wrcmd8(LCD_CMD_DISPLAY_OFF);
    delay_ms(20);
    lcd_wrcmd8(LCD_CMD_SLEEPIN);
    delay_ms(120);
  }
  else
  {
    lcd_wrcmd8(LCD_CMD_SLEEPOUT);
    delay_ms(120);
    lcd_wrcmd8(LCD_CMD_DISPLAY_ON);
    delay_ms(20);
  }

  lcd_disable();

  return;
}


void lcd_reset(void)
{
	uint_least32_t s = 0;

    //init pins
    INIT_PINS();

    //hardware reset
    //GPIO_CLRPIN(LCD_PORT, RST_PIN);
    Chip_GPIO_SetPinOutLow(LPC_GPIO,0,RST_PIN);
    delay_ms(20);

    Chip_GPIO_SetPinOutHigh(LPC_GPIO,0,RST_PIN);
    //GPIO_SETPIN(LCD_PORT, RST_PIN);
    delay_ms(120);

  	lcd_enable();

    lcd_wrcmd8(0x01);//soft reset
  	delay_ms(1000);

  	//power control A
  	lcd_wrcmd8(0xCB);
  	lcd_wrdata8(0x39);
  	lcd_wrdata8(0x2C);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0x34);
  	lcd_wrdata8(0x02);

  	//power control B
  	lcd_wrcmd8(0xCF);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0xC1);
  	lcd_wrdata8(0x30);

  	//driver timing control A
  	lcd_wrcmd8(0xE8);
  	lcd_wrdata8(0x85);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0x78);

  	//driver timing control B
  	lcd_wrcmd8(0xEA);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0x00);

  	//power on sequence control
  	lcd_wrcmd8(0xED);
  	lcd_wrdata8(0x64);
  	lcd_wrdata8(0x03);
  	lcd_wrdata8(0x12);
  	lcd_wrdata8(0x81);

  	//pump ratio control
  	lcd_wrcmd8(0xF7);
  	lcd_wrdata8(0x20);

  	//power control,VRH[5:0]
  	lcd_wrcmd8(0xC0);
  	lcd_wrdata8(0x23);

  	//Power control,SAP[2:0];BT[3:0]
  	lcd_wrcmd8(0xC1);
  	lcd_wrdata8(0x10);

  	//vcm control
  	lcd_wrcmd8(0xC5);
  	lcd_wrdata8(0x3E);
  	lcd_wrdata8(0x28);

  	//vcm control 2
  	lcd_wrcmd8(0xC7);
  	lcd_wrdata8(0x86);

  	//memory access control
  	lcd_wrcmd8(0x36);
  	lcd_wrdata8(0x48);

  	//pixel format
  	lcd_wrcmd8(0x3A);
  	lcd_wrdata8(0x55);

  	//frameration control,normal mode full colours
  	lcd_wrcmd8(0xB1);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0x18);

  	//display function control
  	lcd_wrcmd8(0xB6);
  	lcd_wrdata8(0x08);
  	lcd_wrdata8(0x82);
  	lcd_wrdata8(0x27);

  	//3gamma function disable
  	lcd_wrcmd8(0xF2);
  	lcd_wrdata8(0x00);

  	//gamma curve selected
  	lcd_wrcmd8(0x26);
  	lcd_wrdata8(0x01);

  	//set positive gamma correction
  	lcd_wrcmd8(0xE0);
  	lcd_wrdata8(0x0F);
  	lcd_wrdata8(0x31);
  	lcd_wrdata8(0x2B);
  	lcd_wrdata8(0x0C);
  	lcd_wrdata8(0x0E);
  	lcd_wrdata8(0x08);
  	lcd_wrdata8(0x4E);
  	lcd_wrdata8(0xF1);
  	lcd_wrdata8(0x37);
  	lcd_wrdata8(0x07);
  	lcd_wrdata8(0x10);
  	lcd_wrdata8(0x03);
  	lcd_wrdata8(0x0E);
  	lcd_wrdata8(0x09);
  	lcd_wrdata8(0x00);

  	//set negative gamma correction
  	lcd_wrcmd8(0xE1);
  	lcd_wrdata8(0x00);
  	lcd_wrdata8(0x0E);
  	lcd_wrdata8(0x14);
  	lcd_wrdata8(0x03);
  	lcd_wrdata8(0x11);
  	lcd_wrdata8(0x07);
  	lcd_wrdata8(0x31);
  	lcd_wrdata8(0xC1);
  	lcd_wrdata8(0x48);
  	lcd_wrdata8(0x08);
  	lcd_wrdata8(0x0F);
  	lcd_wrdata8(0x0C);
  	lcd_wrdata8(0x31);
  	lcd_wrdata8(0x36);
  	lcd_wrdata8(0x0F);

  	//exit sleep
  	lcd_wrcmd8(0x11);
  	delay_ms(120);
  	//display on
  	lcd_wrcmd8(0x29);

  return;
}

void SendDatoSPI(uint8_t dato)
{
	uint16_t dummy;
	uint16_t datoSerie;
	Chip_SSP_DATA_SETUP_T datospi;
	datoSerie = dato;
	datospi.rx_cnt=0;
	datospi.tx_cnt=0;
	datospi.rx_data=&dummy;
	datospi.tx_data=&datoSerie;
	datospi.length= 1;
	Chip_SSP_RWFrames_Blocking(LPC_SSP,&datospi);
}

void SendDatoSPI2(uint8_t dato)
{
	//Chip_GPIO_SetPinOutLow(LPC_GPIO,SPI_SSEL);
	Chip_GPIO_WritePortBit(LPC_GPIO,1 , 24, 1);
	Chip_SSP_WriteFrames_Blocking(LPC_SSP,&dato,1);
	//Chip_GPIO_SetPinOutHigh(LPC_GPIO,SPI_SSEL);
	Chip_GPIO_WritePortBit(LPC_GPIO,1 , 24, 0);
}


//TODO: aun tiene algunos problemas. ver
void SendDatoSPI16(uint16_t dato)
{
	uint16_t dummy;
	Chip_SSP_DATA_SETUP_T datospi;
	datospi.rx_cnt=0;
	datospi.tx_cnt=0;
	datospi.rx_data=&dummy;
	datospi.tx_data=&dato;
	datospi.length= 2;
	//Chip_GPIO_SetPinOutLow(LPC_GPIO,SPI_SSEL);
	Chip_SSP_RWFrames_Blocking(LPC_SSP,&datospi);
	//Chip_GPIO_SetPinOutHigh(LPC_GPIO,SPI_SSEL);
}




void lcd_wrdata16(uint_least16_t data)
{
  //GPIO_SETPIN(LCD_PORT, RS_PIN); //data
  Chip_GPIO_SetPinOutHigh(LPC_GPIO,LCD_PORT,RS_PIN); //rs high cuando es data
  //lcd_enable();
  //delay_ms(1);

  SendDatoSPI2(data>>8);
  SendDatoSPI2(data);
  //SendDatoSPI16(data);
  //lcd_disable();
  return;
}


void lcd_wrdata8(uint_least8_t data)
{
  Chip_GPIO_SetPinOutHigh(LPC_GPIO,LCD_PORT,RS_PIN); //rs high cuando es data
  //lcd_enable();
  //delay_ms(1);

  SendDatoSPI(data);

  //lcd_disable();
  return;
}


void lcd_wrcmd8(uint_least8_t cmd)
{
	Chip_GPIO_SetPinOutLow(LPC_GPIO,LCD_PORT,RS_PIN); //rs low cuando es cmd
	//lcd_enable();
	//delay_ms(1);
	SendDatoSPI(cmd);
	//lcd_disable();
	return;
}

void lcd_disable(void)
{
  //  GPIO_SETPIN(LCD_PORT, CS_PIN);
  Chip_GPIO_SetPinOutHigh(LPC_GPIO,SPI_SSEL);
  return;
}


void lcd_enable(void)
{
  Chip_GPIO_SetPinOutLow(LPC_GPIO,SPI_SSEL);
  return;
}


#endif //LCD_MI0283QT9
