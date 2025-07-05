#pragma once


#include <Arduino.h>


#define GYTOF10M_ID_BYTE				0x00			
#define GYTOF10M_BAUD_SETTING			0x01
#define GYTOF10M_UPDATE_RATE			0x02
#define GYTOF10M_OUTPUT_MODE			0x03
#define GYTOF10M_OUTPUT_FORMAT			0x04
#define GYTOF10M_SAVE_SETTINGS			0x05
#define GYTOF10M_DISTANCE_OFFSET		0x06
#define GYTOF10M_INTERFACE_SELECT		0x07
#define GYTOF10M_DISTANCE_H				0x08
#define GYTOF10M_DISTANCE_L				0x09
#define GYTOF10M_AMPLITUDE_H			0x0A
#define GYTOF10M_AMPLITUDE_L			0x0B
#define GYTOF10M_TEMPERATURE			0x0C
#define GYTOF10M_UPPER_ALARM_H			0x0D
#define GYTOF10M_UPPER_ALARM_L			0x0E
#define GYTOF10M_LOWER_ALARM_H			0x0F
#define GYTOF10M_LOWER_ALARM_L			0x10
#define GYTOF10M_MODULE_MODEL			0x11
#define GYTOF10M_FIRMWARE_INFO			0x12

#define GYTOF10M_BAUD_SET_2400			0x00
#define GYTOF10M_BAUD_SET_4800			0x01
#define GYTOF10M_BAUD_SET_9600			0x02
#define GYTOF10M_BAUD_SET_19200			0x03
#define GYTOF10M_BAUD_SET_38400			0x04
#define GYTOF10M_BAUD_SET_57600			0x05
#define GYTOF10M_BAUD_SET_115200		0x06
#define GYTOF10M_BAUD_SET_230400		0x07

#define GYTOF10M_UPDATE_RATE_1HZ		0x00
#define GYTOF10M_UPDATE_RATE_10HZ		0x01
#define GYTOF10M_UPDATE_RATE_50HZ		0x02
#define GYTOF10M_UPDATE_RATE_100HZ		0x03
#define GYTOF10M_UPDATE_RATE_200HZ		0x04

#define GYTOF10M_OUTPUT_MODE_CONT		0x00
#define GYTOF10M_OUTPUT_MODE_INT		0x01
#define GYTOF10M_OUTPUT_MODE_POLL		0x02

#define GYTOF10M_OUTPUT_FORMAT_HEX		0x00
#define GYTOF10M_OUTPUT_FORMAT_ASCII	0x01

#define GYTOF10M_INTERFACE_SELECT_I2C	0x00
#define GYTOF10M_INTERFACE_SELECT_APM	0x01
#define GYTOF10M_INTERFACE_SELECT_UART	0x02

#define GYTOF10M_FRAME_HEADER			0xA4
#define GYTOF10M_READ_COMMAND			0x03
#define GYTOF10M_WRITE_COMMAND			0x06


class GYTOF10M
{
	public:
		GYTOF10M();
		void begin(void);
		int8_t get_offset(void);
		uint8_t get_address(void);
		uint8_t get_baudrate(void);
		uint8_t get_update_rate(void);
		uint8_t get_output_mode(void);
		uint8_t get_output_format(void);
		uint8_t get_interface_mode(void);
		uint8_t get_temperature(void);
		uint16_t get_range(void);
		uint16_t get_amplitude(void);
		uint16_t get_upper_alarm(void);
		uint16_t get_lower_alarm(void);
		void save_settings(void);
		void factory_reset(void);
		void set_offset(int8_t value);
		void set_address(uint8_t value);
		void set_baudrate(uint8_t value);
		void set_update_rate(uint8_t value);
		void set_output_mode(uint8_t value);
		void set_output_format(uint8_t value);
		void set_interface_mode(uint8_t value);

	private:
		void write(uint8_t addr, int8_t value);
		uint8_t read_byte(uint8_t addr);
		uint16_t read_word(uint8_t addr);
		uint8_t get_CRC(uint8_t *values, uint8_t length);
		void send_read_request(uint8_t addr, uint8_t length);
};
