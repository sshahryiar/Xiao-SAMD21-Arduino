#include "GYTOF10M.h"


GYTOF10M::GYTOF10M()
{
}


void GYTOF10M::begin(void)
{
	Serial1.begin(9600);
	Serial1.flush();
	delay(100);

	factory_reset();
	set_interface_mode(GYTOF10M_INTERFACE_SELECT_UART);
	set_baudrate(GYTOF10M_BAUD_SET_9600);
	set_output_mode(GYTOF10M_OUTPUT_MODE_POLL);
	set_output_format(GYTOF10M_OUTPUT_FORMAT_HEX);
	set_update_rate(GYTOF10M_UPDATE_RATE_1HZ);
}


int8_t GYTOF10M::get_offset(void)
{
	return ((int8_t)read_byte(GYTOF10M_DISTANCE_OFFSET));
}


uint8_t GYTOF10M::get_address(void)
{
	return (read_byte(GYTOF10M_ID_BYTE));
}


uint8_t GYTOF10M::get_baudrate(void)
{
	return (read_byte(GYTOF10M_BAUD_SETTING));
}


uint8_t GYTOF10M::get_update_rate(void)
{
	return (read_byte(GYTOF10M_UPDATE_RATE));
}


uint8_t GYTOF10M::get_output_mode(void)
{
	return (read_byte(GYTOF10M_OUTPUT_MODE));
}


uint8_t GYTOF10M::get_output_format(void)
{
	return (read_byte(GYTOF10M_OUTPUT_FORMAT));
}


uint8_t GYTOF10M::get_interface_mode(void)
{
	return (read_byte(GYTOF10M_INTERFACE_SELECT));
}


uint8_t GYTOF10M::get_temperature(void)
{
	return (read_byte(GYTOF10M_TEMPERATURE));
}


uint16_t GYTOF10M::get_range(void)
{
	return (read_word(GYTOF10M_DISTANCE_H));
}


uint16_t GYTOF10M::get_amplitude(void)
{
	return (read_word(GYTOF10M_AMPLITUDE_H));
}


uint16_t GYTOF10M::get_upper_alarm(void)
{
	return (read_word(GYTOF10M_UPPER_ALARM_H));
}


uint16_t GYTOF10M::get_lower_alarm(void)
{
	return (read_word(GYTOF10M_LOWER_ALARM_H));
}


void GYTOF10M::save_settings(void)
{
	write(GYTOF10M_SAVE_SETTINGS, 0x55);
}


void GYTOF10M::factory_reset(void)
{
	write(GYTOF10M_SAVE_SETTINGS, 0xAA);
}


void GYTOF10M::set_offset(int8_t value)
{
	write(GYTOF10M_DISTANCE_OFFSET, value);
}


void GYTOF10M::set_address(uint8_t value)
{
	write(GYTOF10M_ID_BYTE, value);
}


void GYTOF10M::set_baudrate(uint8_t value)
{
	write(GYTOF10M_BAUD_SETTING, value);
}


void GYTOF10M::set_update_rate(uint8_t value)
{
	write(GYTOF10M_UPDATE_RATE, value);
}


void GYTOF10M::set_output_mode(uint8_t value)
{
	write(GYTOF10M_OUTPUT_MODE, value);
}


void GYTOF10M::set_output_format(uint8_t value)
{
	write(GYTOF10M_OUTPUT_FORMAT, value);
}


void GYTOF10M::set_interface_mode(uint8_t value)
{
	write(GYTOF10M_INTERFACE_SELECT, value);
}


void GYTOF10M::write(uint8_t addr, int8_t value)
{
	uint8_t data_stream[5] = {GYTOF10M_FRAME_HEADER, 
							  GYTOF10M_WRITE_COMMAND, 
							  addr,  
							  (uint8_t)value, 
							  0x00};
	uint8_t i = 0;
	uint8_t CRC = get_CRC(data_stream, 4);
	data_stream[4] = CRC;

	for(i = 0; i <= 4; i++)
	{
		Serial1.write(data_stream[i]);
	}
	
	delay(100);
}


uint8_t GYTOF10M::read_byte(uint8_t addr)
{
	uint8_t rx_bytes[18];
	uint8_t tmp_rx_bytes[6];
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t crc = 0;

	send_read_request(addr, 1);

	if(Serial1.available() > 0) 
	{
		while((Serial1.available() > 0) && (i < 18))
		{
			rx_bytes[i] = Serial1.read();
			i++;
		};
	}

	for(i = 0; i < 18; i++)
	{
		if((rx_bytes[i] == GYTOF10M_FRAME_HEADER) && (rx_bytes[(i + 1)] == GYTOF10M_READ_COMMAND))
		{
			j = i;
			break;
		}
	}

	for(i = 0; i < 6; i++)
	{
		tmp_rx_bytes[i] = rx_bytes[(j + i)];
	}

	crc = get_CRC(tmp_rx_bytes, 5);

	if(crc == tmp_rx_bytes[5])
	{
		return (rx_bytes[4]);
	}

	else
	{
		return 0xFF;
	}


	return 0xFF;
}


uint16_t GYTOF10M::read_word(uint8_t addr)
{
	uint8_t rx_bytes[21];
	uint8_t tmp_rx_bytes[7];
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t crc = 0;
	uint16_t value = 0;

	send_read_request(addr, 2);

	if(Serial1.available() > 0) 
	{
		while((Serial1.available() > 0) && (i < 21))
		{
			rx_bytes[i] = Serial1.read();
			i++;
		};
	}

	for(i = 0; i < 21; i++)
	{
		if((rx_bytes[i] == GYTOF10M_FRAME_HEADER) && (rx_bytes[(i + 1)] == GYTOF10M_READ_COMMAND))
		{
			j = i;
			break;
		}
	}

	for(i = 0; i < 7; i++)
	{
		tmp_rx_bytes[i] = rx_bytes[(j + i)];
	}

	crc = get_CRC(tmp_rx_bytes, 6);

	if(crc == tmp_rx_bytes[6])
	{
		value = tmp_rx_bytes[4];
		value <<= 8;
		value |= tmp_rx_bytes[5];

		return value;
	}

	else
	{
		return 0xFFFF;
	}

	return 0xFFFF;
}


uint8_t GYTOF10M::get_CRC(uint8_t *values, uint8_t length)
{
	uint8_t i = 0;
	uint8_t crc = 0;

	for(i = 0; i < length; i++)
	{
		crc += values[i];
	}

	return (crc & 0xFF);
}


void GYTOF10M::send_read_request(uint8_t addr, uint8_t length)
{
	uint8_t tx_stream[5] = {GYTOF10M_FRAME_HEADER, 
						    GYTOF10M_READ_COMMAND, 
						    addr, 
						    length, 
						    0x00};
	uint8_t i = 0;
	uint8_t CRC = get_CRC(tx_stream, 5);
	tx_stream[4] = CRC;

	for(i = 0; i <= 4; i++)
	{
		Serial1.write(tx_stream[i]);
	}
}
