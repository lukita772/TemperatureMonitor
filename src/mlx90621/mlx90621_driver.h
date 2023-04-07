/*
 * mlx90621_driver.h
 *
 *  Created on: 6 de oct. de 2019
 *      Author: LAGOSTINI
 */

#ifndef MLX90621_MLX90621_DRIVER_H_
#define MLX90621_MLX90621_DRIVER_H_

#define LEN_BUF			(256)//(32)

#define CAL_ACOMMON_L 0xD0
#define CAL_ACOMMON_H 0xD1
#define CAL_ACP_L 0xD3
#define CAL_ACP_H 0xD4
#define CAL_BCP 0xD5
#define CAL_alphaCP_L 0xD6
#define CAL_alphaCP_H 0xD7
#define CAL_TGC 0xD8
#define CAL_AI_SCALE 0xD9
#define CAL_BI_SCALE 0xD9

#define VTH_L 0xDA
#define VTH_H 0xDB
#define KT1_L 0xDC
#define KT1_H 0xDD
#define KT2_L 0xDE
#define KT2_H 0xDF
#define KT_SCALE 0xD2

//Common sensitivity coefficients
#define CAL_A0_L 0xE0
#define CAL_A0_H 0xE1
#define CAL_A0_SCALE 0xE2
#define CAL_DELTA_A_SCALE 0xE3
#define CAL_EMIS_L 0xE4
#define CAL_EMIS_H 0xE5


uint16_t unsigned_16(uint8_t highByte, uint8_t lowByte);
int16_t twos_16(uint8_t highByte, uint8_t lowByte);
int8_t twos_8(uint8_t byte);
void calculateTA(void);
void calculateTO();
void readIR();
void Chip_I2C_MasterPckRead(I2C_ID_T id, uint8_t slaveAddr, uint8_t *pck, uint8_t *buff, int lenRx, int lenTx);
void readPTAT();
void readCPIX();
void initializeMLX90621();
void readEEPROM();
void writeTrimmingValue();
uint16_t readConfig();
void setConfiguration();
void preCalculateConstants();
void measureTemperature();

#endif /* MLX90621_MLX90621_DRIVER_H_ */
