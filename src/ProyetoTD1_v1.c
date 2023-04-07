/*
===============================================================================
 Name        : ProyectoTD2.c
 Author      : $(Nicolas Baigorria, Luca Agsotini)
 Version     : 1.00
 Description : main definition
===============================================================================
*/

#include "chip.h"
#include <cr_section_macros.h>
#include <stdio.h>

#include "ILI9341/lcd.h"
#include "mlx90621/mlx90621_driver.h"
#include "ILI9341/lcd_font8x8.h"

#define I2C_DEVICE		(I2C1)
#define I2C_DEVICE_NUM	(1)
#define I2C_DEVICE_PORT	(0)
#define I2C_SDA_PIN		(0)
#define I2C_SCL_PIN		(1)
#define I2C_SPEED		(400000)
#define TICKRATE_HZ1 	(500)	/* 500 ticks per second */
#define SW_PRESSED		0
#define SW_NOT_PRESSED  1

// ------ Public variable ------------------------------------------
uint32_t SW_switch_pressed_G = SW_NOT_PRESSED;
volatile uint_least32_t ms_ticks=0;
extern volatile float temperatures[64];

void InitHardware(void);
void SysTick_Handler(void);

static uint8_t map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void GPIO_SWITCH_Update(void)
{
	static uint32_t counter = 0;
	static uint8_t lastState = 0;
	static uint8_t switch_input = 0;

	// Read TEST_GPIO_SWITCH
	switch_input = Chip_GPIO_ReadPortBit(LPC_GPIO, 0, 4);

	if (switch_input == lastState)
	{
		counter ++;

		if (counter >= 3)
		{
			// Switch pressed
			SW_switch_pressed_G = switch_input;
			counter = 0;
		}

	}
	else
	{
		counter = 0;
	}

	lastState = switch_input;
}


void SysTick_Handler()
{
	static int et = 0;
	int tmp;

	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	DWT->CYCCNT = 0;

	static uint16_t measureTempCounter = 1000;
	static uint16_t refreshDisplayCounter = 1500;
	static uint8_t SW_switch_pressed_G_ant;
	static uint8_t offset_x, offset_y = 0;
	uint8_t color = 0;


	char tempString[3];

	ms_ticks = ms_ticks+2;
	measureTempCounter --;
	refreshDisplayCounter --;

	GPIO_SWITCH_Update();

	//lcd_drawtext(10, 70, "Modo: Hold", 2, 0, 0, 0);

	if(measureTempCounter == 0)
	{
		if(SW_switch_pressed_G == SW_NOT_PRESSED)
		{
			//measureTemperature();
		}
		measureTempCounter = 150;
	}

	if(refreshDisplayCounter == 0)
	{
		if(SW_switch_pressed_G == SW_NOT_PRESSED)
		{
			lcd_drawtext(0,0, "", 1, 0, 0, 0);

			for(int y=0;y<240;y++)
			{ //go through all the rows

				for(int x=0;x<320;x++)
				{ //go through all the columns

					 //itoa((int)temperatures[y + x * 4], tempString, 10);
				     //color = map(temperatures[y + x * 4], 15, 60, 0, 255);`
				     //lcd_drawstart();

				     //lcd_fillrndrect((x*20),(y*30)+offset_x, (x*20+20),(y*30+30)+offset_y, 0, RGB(color,20,255-color) );
				     //lcd_drawtext((x*20+2), (y*30+15)+100, tempString, 1, 0, 0, 0);
					 //lcd_drawpixel(x,y,RGB(fontdata[(x+320*y)],fontdata[((x+320*y))+1],fontdata[((x+320*y))+2]));
					 //lcd_fillrndrect((x+50),(y*30), (x+51),(y*30+30), 0, fontdata[x+y]);
				}

			}
		}


		if(SW_switch_pressed_G_ant != SW_switch_pressed_G)
		{
			lcd_drawstart();
			lcd_drawtext(10, 70, "Modo:", 2, 0, 0, 0);

			if(SW_switch_pressed_G == SW_PRESSED)
			{

				lcd_drawtext(10, 70, "Modo: Hold", 2, 0, 0, 0);
			}
			else
			{
				lcd_drawtext(10, 70, "Modo: Muestreo", 2, 0, 0, 0);
			}
		}

		SW_switch_pressed_G_ant = SW_switch_pressed_G;

		refreshDisplayCounter = 10000;
	}

	//90ms , necesito tick de 100ms considerando solo procesamiento y deberia particionar dibujar la pantalla
	tmp = DWT->CYCCNT;
	if(et<tmp)
	{
		et = tmp;
	}

}

uint_least32_t get_ms(void)
{
  return ms_ticks;
}


void delay_ms(uint_least32_t delay)
{
  uint_least32_t current = get_ms();

  while((get_ms() - current) < delay);

  return;
}

int main(void)
{
	InitHardware();

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}

void initializePins()
{
	Chip_IOCON_Init(LPC_IOCON);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,0 , 4);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 24, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,1 , 24);

	/*Inicializo el I2C*/
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SDA_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SCL_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SCL_PIN);
	Chip_I2C_Init(I2C_DEVICE_NUM);
	Chip_I2C_SetClockRate(I2C_DEVICE_NUM,I2C_SPEED);
	Chip_I2C_SetMasterEventHandler(I2C_DEVICE,Chip_I2C_EventHandlerPolling);

}

void initializeDisplay()
{
	ms_ticks = 0;

	lcd_init();
	lcd_power(1);
	lcd_drawstart();
	//lcd_drawtext(10, 10, "Proyecto TD2", 2, 0, 0, 0);
	//lcd_drawtext(10, 40, "Luca Agostini.2019", 2, 0, 0, 0);
	//lcd_drawrndrect(0,0, 20, 20, 0, 0 );
	//cd_fillrndrect(0,0, 150, 150, 0, 0 );
}


void InitHardware(void)
{
	uint32_t i;

	/*Inicializo el Clock del sistema*/
	SystemCoreClockUpdate();

	initializePins();

	/*Inicializo el systick*/
	SysTick_Config(SystemCoreClock/TICKRATE_HZ1);

	/*Espero que arranque el display y lo configuro*/
	for(i=0;i<1000000;i++);

	//initializeMLX90621();
	initializeDisplay();

	lcd_fillScreen();
}


