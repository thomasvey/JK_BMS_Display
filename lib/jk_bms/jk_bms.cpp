#include "jk_bms.h"

JK_BMS::JK_BMS()
{

}

void JK_BMS::req_status()
{
	Serial1.write(req_buffer, req_len);

	// we need a minimum delay of 70ms to recv valid data
	// we need 80ms when the app is active
	delay(80);

	rsp_len = Serial1.readBytes(rsp_buffer, rsp_len);

	if(rsp_len != 256){
		Serial.println("ERROR rsp len != 256: " + String(rsp_len));
	}
}

float JK_BMS::get_voltage()
{
	u16 val = rsp_buffer[INDEX::volt + 1] << 8 | rsp_buffer[INDEX::volt + 2];
	return float(val) / 100.00;
}

float JK_BMS::get_current()
{
	// sign bit
	int sign = -1;
	if (rsp_buffer[INDEX::current + 1] >> 7)
	{
		sign = 1;
	}

	// cut highes byte
	u8 highByte = rsp_buffer[INDEX::current + 1] & 0x7F;

	// get float
	u16 val = highByte << 8 | rsp_buffer[INDEX::current + 2];
	return sign * float(val) / 100.00;
}

u8 JK_BMS::get_SOC()
{
	return rsp_buffer[INDEX::soc + 1];
}

u16 JK_BMS::get_current_calibration()
{
	return rsp_buffer[INDEX::current_callibration + 1] << 8 |
		   rsp_buffer[INDEX::current_callibration + 2];
}

u8 JK_BMS::get_temprature_MOS()
{
	return rsp_buffer[INDEX::temprature_MOS + 2];
}

u8 JK_BMS::get_temprature_1()
{
	return rsp_buffer[INDEX::temprature_1 + 2];
}

u8 JK_BMS::get_temprature_2()
{
	return rsp_buffer[INDEX::temprature_2 + 2];
}

u8 JK_BMS::get_sensors()
{
	return rsp_buffer[INDEX::sensors + 1];
}

u8 JK_BMS::get_cycels()
{
	return rsp_buffer[INDEX::cycels + 1];
}

void JK_BMS::print_status()
{
	Serial.println();
	Serial.println("Voltage: " + String(get_voltage()) + "V");
	Serial.println("Current: " + String(get_current()) + "A");
	Serial.println("Level  : " + String(get_SOC()) + "%");
}

void JK_BMS::print_full_status()
{
	print_status();

	Serial.println("Curr Cal: " + String(get_current_calibration()) + "mA");
	Serial.println("Temp MOS: " + String(get_temprature_MOS()) + "°C");
	Serial.println("Temp 1  : " + String(get_temprature_1()) + "°C");
	Serial.println("Temp 2  : " + String(get_temprature_2()) + "°C");
	Serial.println("Sensors : " + String(get_sensors()));
	Serial.println("Cycles  : " + String(get_cycels()));

}