#ifndef _LCD_MI0283QT9_H_
#define _LCD_MI0283QT9_H_

#ifdef LCD_MI0283QT9


//----- DEFINES -----
#define LCD_WIDTH                      320
#define LCD_HEIGHT                     240

#define LCD_CMD_NOP                    0x00
#define LCD_CMD_RESET                  0x01
#define LCD_CMD_SLEEPIN                0x10
#define LCD_CMD_SLEEPOUT               0x11
#define LCD_CMD_PARTIAL_MODE           0x12
#define LCD_CMD_NORMAL_MODE            0x13
#define LCD_CMD_INV_OFF                0x20
#define LCD_CMD_INV_ON                 0x21
#define LCD_CMD_GAMMA                  0x26
#define LCD_CMD_DISPLAY_OFF            0x28
#define LCD_CMD_DISPLAY_ON             0x29
#define LCD_CMD_COLUMN                 0x2A
#define LCD_CMD_PAGE                   0x2B
#define LCD_CMD_WRITE                  0x2C
#define LCD_CMD_READ                   0x2E
#define LCD_CMD_PARTIAL_AREA           0x30
#define LCD_CMD_TEARING_OFF            0x34
#define LCD_CMD_TEARING_ON             0x35
#define LCD_CMD_MEMACCESS_CTRL         0x36
#define LCD_CMD_IDLE_OFF               0x38
#define LCD_CMD_IDLE_ON                0x39
#define LCD_CMD_PIXEL_FORMAT           0x3A
#define LCD_CMD_WRITE_CNT              0x3C
#define LCD_CMD_READ_CNT               0x3E
#define LCD_CMD_BRIGHTNESS             0x51
#define LCD_CMD_BRIGHTNESS_CTRL        0x53
#define LCD_CMD_RGB_CTRL               0xB0
#define LCD_CMD_FRAME_CTRL             0xB1 //normal mode
#define LCD_CMD_FRAME_CTRL_IDLE        0xB2 //idle mode
#define LCD_CMD_FRAME_CTRL_PART        0xB3 //partial mode
#define LCD_CMD_INV_CTRL               0xB4
#define LCD_CMD_DISPLAY_CTRL           0xB6
#define LCD_CMD_ENTRY_MODE             0xB7
#define LCD_CMD_POWER_CTRL1            0xC0
#define LCD_CMD_POWER_CTRL2            0xC1
#define LCD_CMD_VCOM_CTRL1             0xC5
#define LCD_CMD_VCOM_CTRL2             0xC7
#define LCD_CMD_POWER_CTRLA            0xCB
#define LCD_CMD_POWER_CTRLB            0xCF
#define LCD_CMD_POS_GAMMA              0xE0
#define LCD_CMD_NEG_GAMMA              0xE1
#define LCD_CMD_DRV_TIMING_CTRLA       0xE8
#define LCD_CMD_DRV_TIMING_CTRLB       0xEA
#define LCD_CMD_POWERON_SEQ_CTRL       0xED
#define LCD_CMD_ENABLE_3G              0xF2
#define LCD_CMD_INTERF_CTRL            0xF6
#define LCD_CMD_PUMP_RATIO_CTRL        0xF7


#define ILI9341_TFTWIDTH   240      ///< ILI9341 max TFT width
#define ILI9341_TFTHEIGHT  320      ///< ILI9341 max TFT height

#define ILI9341_NOP        0x00     ///< No-op register
#define ILI9341_SWRESET    0x01     ///< Software reset register
#define ILI9341_RDDID      0x04     ///< Read display identification information
#define ILI9341_RDDST      0x09     ///< Read Display Status

#define ILI9341_SLPIN      0x10     ///< Enter Sleep Mode
#define ILI9341_SLPOUT     0x11     ///< Sleep Out
#define ILI9341_PTLON      0x12     ///< Partial Mode ON
#define ILI9341_NORON      0x13     ///< Normal Display Mode ON

#define ILI9341_RDMODE     0x0A     ///< Read Display Power Mode
#define ILI9341_RDMADCTL   0x0B     ///< Read Display MADCTL
#define ILI9341_RDPIXFMT   0x0C     ///< Read Display Pixel Format
#define ILI9341_RDIMGFMT   0x0D     ///< Read Display Image Format
#define ILI9341_RDSELFDIAG 0x0F     ///< Read Display Self-Diagnostic Result

#define ILI9341_INVOFF     0x20     ///< Display Inversion OFF
#define ILI9341_INVON      0x21     ///< Display Inversion ON
#define ILI9341_GAMMASET   0x26     ///< Gamma Set
#define ILI9341_DISPOFF    0x28     ///< Display OFF
#define ILI9341_DISPON     0x29     ///< Display ON

#define ILI9341_CASET      0x2A     ///< Column Address Set
#define ILI9341_PASET      0x2B     ///< Page Address Set
#define ILI9341_RAMWR      0x2C     ///< Memory Write
#define ILI9341_RAMRD      0x2E     ///< Memory Read

#define ILI9341_PTLAR      0x30     ///< Partial Area
#define ILI9341_MADCTL     0x36     ///< Memory Access Control
#define ILI9341_VSCRSADD   0x37     ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT     0x3A     ///< COLMOD: Pixel Format Set

#define ILI9341_FRMCTR1    0xB1     ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_FRMCTR2    0xB2     ///< Frame Rate Control (In Idle Mode/8 colors)
#define ILI9341_FRMCTR3    0xB3     ///< Frame Rate control (In Partial Mode/Full Colors)
#define ILI9341_INVCTR     0xB4     ///< Display Inversion Control
#define ILI9341_DFUNCTR    0xB6     ///< Display Function Control

#define ILI9341_PWCTR1     0xC0     ///< Power Control 1
#define ILI9341_PWCTR2     0xC1     ///< Power Control 2
#define ILI9341_PWCTR3     0xC2     ///< Power Control 3
#define ILI9341_PWCTR4     0xC3     ///< Power Control 4
#define ILI9341_PWCTR5     0xC4     ///< Power Control 5
#define ILI9341_VMCTR1     0xC5     ///< VCOM Control 1
#define ILI9341_VMCTR2     0xC7     ///< VCOM Control 2

#define ILI9341_RDID1      0xDA     ///< Read ID 1
#define ILI9341_RDID2      0xDB     ///< Read ID 2
#define ILI9341_RDID3      0xDC     ///< Read ID 3
#define ILI9341_RDID4      0xDD     ///< Read ID 4

#define ILI9341_GMCTRP1    0xE0     ///< Positive Gamma Correction
#define ILI9341_GMCTRN1    0xE1     ///< Negative Gamma Correction
//#define ILI9341_PWCTR6     0xFC

// Color definitions
#define ILI9341_BLACK       0x0000  ///<   0,   0,   0
#define ILI9341_NAVY        0x000F  ///<   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0  ///<   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF  ///<   0, 125, 123
#define ILI9341_MAROON      0x7800  ///< 123,   0,   0
#define ILI9341_PURPLE      0x780F  ///< 123,   0, 123
#define ILI9341_OLIVE       0x7BE0  ///< 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618  ///< 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF  ///< 123, 125, 123
#define ILI9341_BLUE        0x001F  ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0  ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF  ///<   0, 255, 255
#define ILI9341_RED         0xF800  ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F  ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0  ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF  ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20  ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define ILI9341_PINK        0xFC18  ///< 255, 130, 198


//----- PROTOTYPES -----
#define                                lcd_drawstop()
//void                                   lcd_drawstop(void); //empty function
void                                   lcd_draw(uint_least16_t color);
void                                   lcd_drawstart(void);
void                                   lcd_setarea(uint_least16_t x0, uint_least16_t y0, uint_least16_t x1, uint_least16_t y1);
uint_least32_t                         lcd_setbias(uint_least16_t o);
void                                   lcd_invert(uint_least8_t on);
void                                   lcd_power(uint_least8_t on);
void                                   lcd_reset(void);
void                                   lcd_wrdata16(uint_least16_t data);
void                                   lcd_wrdata8(uint_least8_t data);
void                                   lcd_wrcmd8(uint_least8_t cmd);
void                                   lcd_disable(void);
void                                   lcd_enable(void);


#endif //LCD_MI0283QT9


#endif //_LCD_MI0283QT9_H_
