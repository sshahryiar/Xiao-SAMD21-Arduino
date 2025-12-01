#pragma once


#include <Arduino.h>
#include <Wire.h>


#define BME280_I2C_ADDRESS				 			 0x76


#define BME280_REGISTER_CHIPID           0xD0
#define BME280_REGISTER_SOFTRESET        0xE0
#define BME280_REGISTER_CAL00            0x88
#define BME280_REGISTER_CAL01            0x89
#define BME280_REGISTER_CAL02            0x8A
#define BME280_REGISTER_CAL03            0x8B
#define BME280_REGISTER_CAL04            0x8C
#define BME280_REGISTER_CAL05            0x8D
#define BME280_REGISTER_CAL06            0x8E
#define BME280_REGISTER_CAL07            0x8F
#define BME280_REGISTER_CAL08            0x90
#define BME280_REGISTER_CAL09            0x91
#define BME280_REGISTER_CAL10            0x92
#define BME280_REGISTER_CAL11            0x93
#define BME280_REGISTER_CAL12            0x94
#define BME280_REGISTER_CAL13            0x95
#define BME280_REGISTER_CAL14            0x96
#define BME280_REGISTER_CAL15            0x97
#define BME280_REGISTER_CAL16            0x98
#define BME280_REGISTER_CAL17            0x99
#define BME280_REGISTER_CAL18            0x9A
#define BME280_REGISTER_CAL19            0x9B
#define BME280_REGISTER_CAL20            0x9C
#define BME280_REGISTER_CAL21            0x9D
#define BME280_REGISTER_CAL22            0x9E
#define BME280_REGISTER_CAL23            0x9F
#define BME280_REGISTER_CAL24            0xA0
#define BME280_REGISTER_CAL25            0xA1
#define BME280_REGISTER_CAL26            0xE1
#define BME280_REGISTER_CAL27            0xE2
#define BME280_REGISTER_CAL28            0xE3
#define BME280_REGISTER_CAL29            0xE4
#define BME280_REGISTER_CAL30            0xE5
#define BME280_REGISTER_CAL31            0xE6
#define BME280_REGISTER_CAL32            0xE7
#define BME280_REGISTER_CAL33            0xE8
#define BME280_REGISTER_CAL34            0xE9
#define BME280_REGISTER_CAL35            0xEA
#define BME280_REGISTER_CAL36            0xEB
#define BME280_REGISTER_CAL37            0xEC
#define BME280_REGISTER_CAL38            0xED
#define BME280_REGISTER_CAL39            0xEE
#define BME280_REGISTER_CAL40            0xEF
#define BME280_REGISTER_CAL41            0xF0
#define BME280_REGISTER_CONTROLHUMID     0xF2
#define BME280_REGISTER_STATUS			 		 0xF3
#define BME280_REGISTER_CONTROL          0xF4
#define BME280_REGISTER_CONFIG           0xF5
#define BME280_REGISTER_PRESSUREDATA     0xF7
#define BME280_REGISTER_TEMPDATA         0xFA
#define BME280_REGISTER_HUMIDDATA        0xFD

#define BME280_DIG_T1_REG                BME280_REGISTER_CAL00
#define BME280_DIG_T2_REG                BME280_REGISTER_CAL02
#define BME280_DIG_T3_REG                BME280_REGISTER_CAL04

#define BME280_DIG_P1_REG                BME280_REGISTER_CAL06
#define BME280_DIG_P2_REG                BME280_REGISTER_CAL08
#define BME280_DIG_P3_REG                BME280_REGISTER_CAL10
#define BME280_DIG_P4_REG                BME280_REGISTER_CAL12
#define BME280_DIG_P5_REG                BME280_REGISTER_CAL14
#define BME280_DIG_P6_REG                BME280_REGISTER_CAL16
#define BME280_DIG_P7_REG                BME280_REGISTER_CAL18
#define BME280_DIG_P8_REG                BME280_REGISTER_CAL20
#define BME280_DIG_P9_REG                BME280_REGISTER_CAL22
    
#define BME280_DIG_H1_REG                BME280_REGISTER_CAL25
#define BME280_DIG_H2_REG                BME280_REGISTER_CAL26
#define BME280_DIG_H3_REG                BME280_REGISTER_CAL28
#define BME280_DIG_H4_REG                BME280_REGISTER_CAL29
#define BME280_DIG_H5_REG                BME280_REGISTER_CAL30
#define BME280_DIG_H6_REG                BME280_REGISTER_CAL32


class BME280
{
	public:
		float t_offset = 0.0;
    float p_offset = 0.0;
    float rh_offset = 0.0;

		BME280();
		bool begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = BME280_I2C_ADDRESS, float t_off = 0, float rh_off = 0, float p_off = 0);
		float get_temperature(void);
		float get_pressure(void);
		float get_relative_humidity(void);


	private:	
		TwoWire *_i2cPort;
    uint8_t _deviceAddress;
		int32_t t_fine;

		struct
		{
			uint16_t dig_T1;
			int16_t  dig_T2;
			int16_t  dig_T3;

			uint16_t dig_P1;
			int16_t  dig_P2;
			int16_t  dig_P3;
			int16_t  dig_P4;
			int16_t  dig_P5;
			int16_t  dig_P6;
			int16_t  dig_P7;
			int16_t  dig_P8;
			int16_t  dig_P9;

			uint8_t  dig_H1;
			int16_t  dig_H2;
			uint8_t  dig_H3;
			int16_t  dig_H4;
			int16_t  dig_H5;
			int8_t   dig_H6;
		}calibration;

		bool init(void);
		void write_byte(uint8_t reg, uint8_t value);
		bool read_bytes(uint8_t reg, uint8_t *buf, uint8_t length);
		uint8_t read_byte(uint8_t reg);
		uint16_t read_word(uint8_t reg);
		uint32_t read_long(uint8_t reg);
		uint16_t read_word_little_endian(uint8_t reg);
		int16_t read_signed_word(uint8_t reg);
		int16_t read_signed_word_little_endian(uint8_t reg);
		uint8_t reading_calibration(void);
		void read_coefficients(void);
};