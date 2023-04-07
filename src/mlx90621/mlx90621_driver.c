/*
 * mlx90621_driver.c
 *
 *  Created on: 6 de oct. de 2019
 *      Author: LAGOSTINI
 */


#include "chip.h"
#include <string.h>
#include <math.h>
#include "mlx90621_driver.h"



volatile float temperatures[64]; //Contains the calculated temperatures of each pixel in the array
volatile float Tambient; //Tracks the changing ambient temperature of the sensor


uint8_t eepromData[LEN_BUF];
int16_t ir_data[64];
uint16_t ptat_data;

float v_ir_off_comp, ksta, v_ir_tgc_comp, v_ir_comp, alpha_comp;
float tak4, resolution_comp;
int16_t a_common, a_i_scale, b_i_scale, k_t1_scale, k_t2_scale, resolution;
float k_t1, k_t2, emissivity, tgc, alpha_cp, a_cp, b_cp, v_th;
int16_t cpix = 0;
float a_ij, b_ij, alpha_ij;
float minTemp, maxTemp;

uint16_t unsigned_16(uint8_t highByte, uint8_t lowByte){
	return (highByte << 8) | lowByte;
}

int16_t twos_16(uint8_t highByte, uint8_t lowByte){
	uint16_t combined_word = 256 * highByte + lowByte;
	if (combined_word > 32767)
		return (int16_t) (combined_word - 65536);
	return (int16_t) combined_word;
}

int8_t twos_8(uint8_t byte) {
	if (byte > 127)
		return (int8_t) byte - 256;
	return (int8_t) byte;
}

void calculateTA(void) {
	Tambient = ((-k_t1 + sqrt(pow(k_t1,2) - (4 * k_t2 * (v_th - (float) ptat_data))))
			/ (2 * k_t2)) + 25.0;
}

//Poll the MLX90621 for its current status
//Returns true if the POR/Brown out bit is set
bool checkConfig() {
	bool check = !((readConfig() & 0x0400) >> 10);
	return check;
}

void calculateTO()
{
	float v_cp_off_comp = (float) cpix - (a_cp + b_cp * (Tambient - 25.0));
	tak4 = pow((float) Tambient + 273.15, 4.0);
	minTemp = 0, maxTemp = 0;
	for (int i = 0; i < 64; i++) {
		a_ij = ((float) a_common + eepromData[i] * pow(2.0, a_i_scale)) / resolution_comp;
		b_ij = (float) twos_8(eepromData[0x40 + i]) / (pow(2.0, b_i_scale) * resolution_comp);
		v_ir_off_comp = (float) ir_data[i] - (a_ij + b_ij * (Tambient - 25.0));
		v_ir_tgc_comp = (float) v_ir_off_comp - tgc * v_cp_off_comp;
		float alpha_ij = ((float) unsigned_16(eepromData[CAL_A0_H], eepromData[CAL_A0_L]) / pow(2.0, (float) eepromData[CAL_A0_SCALE]));
		alpha_ij += ((float) eepromData[0x80 + i] / pow(2.0, (float) eepromData[CAL_DELTA_A_SCALE]));
		alpha_ij = alpha_ij / resolution_comp;
		//ksta = (float) twos_16(eepromData[CAL_KSTA_H], eepromData[CAL_KSTA_L]) / pow(2.0, 20.0);
		//alpha_comp = (1 + ksta * (Tambient - 25.0)) * (alpha_ij - tgc * alpha_cp);
		alpha_comp = (alpha_ij - tgc * alpha_cp);  	// For my MLX90621 the ksta calibrations were 0
													// so I can ignore them and save a few cycles
		v_ir_comp = v_ir_tgc_comp / emissivity;
		float temperature = pow((v_ir_comp / alpha_comp) + tak4, 1.0 / 4.0) - 274.15;

		temperatures[i] = temperature;
		if (minTemp == 0 || temperature < minTemp) {
			minTemp = temperature;
		}
		if (maxTemp == 0 || temperature > maxTemp) {
			maxTemp = temperature;
		}
	}
}

void readIR()
{
	uint8_t packet[4] = {0,0,0,0};
	uint8_t ir_localData[128];
	uint8_t i = 0, j = 0;

	packet[0] = 0x02;
	packet[1] = 0x00;
	packet[2] = 0x01;
	packet[3] = 0x40;

	memset(ir_data,0,sizeof(int16_t)*64);

	Chip_I2C_MasterPckRead(I2C1, 0x60, packet, ir_localData, 128, 4);

	for(i = 0; i < 64; i ++)
	{
		ir_data[i] =  ((ir_localData[j+1] << 8) & 0xFF00) | (ir_localData[j] & 0xFF);

		if (ir_data[i] > 32767)
		{
			ir_data[i] = (int16_t) (ir_data[i] - 65536);
		}

		j = j+2;
	}
}

void Chip_I2C_MasterPckRead(I2C_ID_T id, uint8_t slaveAddr, uint8_t *pck, uint8_t *buff, int lenRx, int lenTx)
{
	I2C_XFER_T xfer = {0};
	xfer.slaveAddr = slaveAddr;
	xfer.txBuff = pck;
	xfer.txSz = lenTx;
	xfer.rxBuff = buff;
	xfer.rxSz = lenRx;
	while (Chip_I2C_MasterTransfer(id, &xfer) == I2C_STATUS_ARBLOST) {}
	//return lenRx - xfer.rxSz;
}

void readPTAT()
{
	uint8_t packetRx[2];

	//I2C_XFER_T xfer = {0};
	uint8_t packet[4] ={ 0,0,0,0};

	packet[0] = 0x02;
	packet[1] = 0x40;
	packet[2] = 0x00;
	packet[3] = 0x01;

	memset(packetRx,0,2);

	Chip_I2C_MasterPckRead(I2C1, 0x60, packet, packetRx, 2, 4);

	ptat_data =  ((packetRx[1] << 8) & 0xFF00) | (packetRx[0] & 0xFF);
	//Chip_I2C_MasterSend(I2C1, 0x60, packet, 4);
	//Chip_I2C_MasterRead(I2C1, 0x60,ptat_data, 2);

}

void readCPIX() {

	uint8_t packet[5] ={ 0,0,0,0,0 };
	uint8_t packetRx[2];

	packet[0] = 0x02;
	packet[1] = 0x41;
	packet[2] = 0x00;
	packet[3] = 0x01;

	memset(packetRx,0,2);

	Chip_I2C_MasterPckRead(I2C1, 0x60, packet, packetRx, 2, 4);

	cpix =  ((packetRx[1] << 8) & 0xFF00) | (packetRx[0] & 0xFF);

}

void initializeMLX90621()
{
	readEEPROM();
	writeTrimmingValue();
	setConfiguration();
	preCalculateConstants();

	readIR();
	readPTAT();
	calculateTA();
	readCPIX();
	calculateTO();
}

void measureTemperature()
{
	if (checkConfig()) {
		readEEPROM();
		writeTrimmingValue();
		setConfiguration();
	}
	readPTAT();
	readIR();
	calculateTA();
	readCPIX();
	calculateTO();


}

void readEEPROM()
{
	Chip_I2C_MasterCmdRead(I2C1,0x50,0x00,eepromData,256);
}

void writeTrimmingValue()
{
	uint8_t packet[5] ={ 0,0,0,0,0 };

	packet[0] = 0x04;
	packet[1] = eepromData[0xF7] - 0xAA; //LSByte check
	packet[2] = eepromData[0xF7]; //LSByte
	packet[3] = 0x56; //MSByte check
	packet[4] = 0x00; //MSByte

	Chip_I2C_MasterSend(I2C1, 0x60, packet, 5);

}

uint16_t readConfig() {

	uint8_t packet[5] ={ 0,0,0,0,0 };
	uint8_t packetRx[2];
	uint16_t config;

	packet[0] = 0x02;
	packet[1] = 0x92;
	packet[2] = 0x00;
	packet[3] = 0x01;

	memset(packetRx,0,2);

	Chip_I2C_MasterPckRead(I2C1, 0x60, packet, packetRx, 2, 4);

	config =  ((packetRx[1] << 8) & 0xFF00) | (packetRx[0] & 0xFF);

	return config;
}

void setConfiguration()
{
	uint8_t packet[5] = { 0,0,0,0,0 };
	uint8_t Hz_LSB = 0b00111001; //conf. 32 hz
	uint8_t default_conf = 0b01000110;

	packet[0] = 0x03; //cmd
	packet[1] = Hz_LSB - 0x55;
	packet[2] = Hz_LSB;
	packet[3] = default_conf - 0x55;
	packet[4] = default_conf;

	Chip_I2C_MasterSend(I2C1, 0x60, packet, 5);

	resolution = (readConfig() & 0x30) >> 4;
}


void preCalculateConstants() {
	resolution_comp = pow(2.0, (3 - resolution));
	emissivity = unsigned_16(eepromData[CAL_EMIS_H], eepromData[CAL_EMIS_L]) / 32768.0;
	a_common = twos_16(eepromData[CAL_ACOMMON_H], eepromData[CAL_ACOMMON_L]);
	a_i_scale = (int16_t)(eepromData[CAL_AI_SCALE] & 0xF0) >> 4;
	b_i_scale = (int16_t) eepromData[CAL_BI_SCALE] & 0x0F;

	alpha_cp = unsigned_16(eepromData[CAL_alphaCP_H], eepromData[CAL_alphaCP_L]) /
			   (pow(2.0, eepromData[CAL_A0_SCALE]) * resolution_comp);
	a_cp = (float) twos_16(eepromData[CAL_ACP_H], eepromData[CAL_ACP_L]) / resolution_comp;
	b_cp = (float) twos_8(eepromData[CAL_BCP]) / (pow(2.0, (float)b_i_scale) * resolution_comp);
	tgc = (float) twos_8(eepromData[CAL_TGC]) / 32.0;

	k_t1_scale = (int16_t) (eepromData[KT_SCALE] & 0xF0) >> 4;
	k_t2_scale = (int16_t) (eepromData[KT_SCALE] & 0x0F) + 10;
	v_th = (float) twos_16(eepromData[VTH_H], eepromData[VTH_L]);
	v_th = v_th / resolution_comp;
	k_t1 = (float) twos_16(eepromData[KT1_H], eepromData[KT1_L]);
	k_t1 /= (pow(2, k_t1_scale) * resolution_comp);
	k_t2 = (float) twos_16(eepromData[KT2_H], eepromData[KT2_L]);
	k_t2 /= (pow(2, k_t2_scale) * resolution_comp);
}


