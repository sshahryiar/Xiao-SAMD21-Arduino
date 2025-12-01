#include "BME280.h"


BME280::BME280()
{
    _i2cPort = &Wire;
    _deviceAddress = BME280_I2C_ADDRESS;

    memset(&calibration, 0, sizeof(calibration));
    t_fine = 0;
}


bool BME280::begin(TwoWire &wirePort, uint8_t deviceAddress, float t_off, float rh_off, float p_off)
{
	bool state;
    
    _i2cPort = &wirePort;
    _deviceAddress = deviceAddress;
    
    t_offset = t_off;
    p_offset = p_off;
    rh_offset = rh_off;

    state = init();

    if(state)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool BME280::init(void)
{
    uint32_t start = 0;

	_i2cPort->begin();
    _i2cPort->flush();
    delay(100);
    
    if(read_byte(BME280_REGISTER_CHIPID) == 0x60)
    {
        Serial.println("BME280 found....");
        write_byte(BME280_REGISTER_SOFTRESET, 0xB6);
        delay(300);

        start = millis();
        while (reading_calibration())
        {
            if((millis() - start > 2000)) 
            {
                break;
            }
            delay(10);
        }

        read_coefficients();

        write_byte(BME280_REGISTER_CONTROLHUMID, 0x01);
        write_byte(BME280_REGISTER_CONTROL, 0x3F); 

        return true;
    }
    else 
    {
        Serial.println("BME280 not found!");
        return false;
    }

    Serial.println("................");
}



void BME280::write_byte(uint8_t reg, uint8_t value)
{
	_i2cPort->beginTransmission(_deviceAddress); 
	_i2cPort->write(reg);      
	_i2cPort->write(value);    
	_i2cPort->endTransmission(); 
}


bool BME280::read_bytes(uint8_t reg, uint8_t *buf, uint8_t length)
{
    uint8_t i = 0;
    
    _i2cPort->beginTransmission(_deviceAddress); 
    _i2cPort->write(reg);
    if(_i2cPort->endTransmission(false) != false) 
    {
        return false;
    }

    if (_i2cPort->requestFrom((int)_deviceAddress, (int)length) < length)
    {
        return false;
    }

    for (i = 0; i < length; ++i)
    {
        buf[i] = _i2cPort->read();
    }

    return true;
}


uint8_t BME280::read_byte(uint8_t reg)
{
	uint8_t buffer;

    if(read_bytes(reg, &buffer, 1) == false)
    {
        return 0xFF;
    }

    return buffer;
}


uint16_t BME280::read_word(uint8_t reg)
{
    uint8_t buffer[2] = {0, 0};

    if(read_bytes(reg, buffer, 2) == false)
    {
        return 0xFFFF;
    }

    return (((uint16_t)buffer[0] << 8) | buffer[1]);
}


uint32_t BME280::read_long(uint8_t reg)
{
    uint8_t buffer[3] = {0, 0, 0};

    if(read_bytes(reg, buffer, 3) == false)
    {
        return 0xFFFFFFFF;
    }

    return (((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | ((uint32_t)buffer[2] >> 4));;
}

uint16_t BME280::read_word_little_endian(uint8_t reg)
{
    uint8_t buffer[2] = {0, 0};

    if(read_bytes(reg, buffer, 2) == false)
    {
        return 0xFFFF;
    }

    return ((uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8));
}


int16_t BME280::read_signed_word(uint8_t reg)
{
	return (int16_t)read_word(reg);
}


int16_t BME280::read_signed_word_little_endian(uint8_t reg)
{
	return (int16_t)read_word_little_endian(reg);
}


uint8_t BME280::reading_calibration(void)
{
	int8_t st = (int8_t)read_byte(BME280_REGISTER_STATUS);

    if(st == (int8_t)0xFF) 
    {
        return 0;
    }       

    return ((uint8_t)(st & 0x01));
}


void BME280::read_coefficients(void)
{
    calibration.dig_T1 = read_word_little_endian(BME280_DIG_T1_REG);
    calibration.dig_T2 = read_signed_word_little_endian(BME280_DIG_T2_REG);
    calibration.dig_T3 = read_signed_word_little_endian(BME280_DIG_T3_REG);
    
    calibration.dig_P1 = read_word_little_endian(BME280_DIG_P1_REG);
    calibration.dig_P2 = read_signed_word_little_endian(BME280_DIG_P2_REG);
    calibration.dig_P3 = read_signed_word_little_endian(BME280_DIG_P3_REG);
    calibration.dig_P4 = read_signed_word_little_endian(BME280_DIG_P4_REG);
    calibration.dig_P5 = read_signed_word_little_endian(BME280_DIG_P5_REG);
    calibration.dig_P6 = read_signed_word_little_endian(BME280_DIG_P6_REG);
    calibration.dig_P7 = read_signed_word_little_endian(BME280_DIG_P7_REG);
    calibration.dig_P8 = read_signed_word_little_endian(BME280_DIG_P8_REG);
    calibration.dig_P9 = read_signed_word_little_endian(BME280_DIG_P9_REG);
    
    calibration.dig_H1 = read_byte(BME280_DIG_H1_REG);
    calibration.dig_H2 = read_signed_word_little_endian(BME280_DIG_H2_REG);
    calibration.dig_H3 = read_byte(BME280_DIG_H3_REG);
    calibration.dig_H4 = (read_byte(BME280_DIG_H4_REG) << 4) + (read_byte(BME280_DIG_H5_REG) & 0x0F);
    calibration.dig_H5 = (read_byte(1 + BME280_DIG_H5_REG) << 4) + (read_byte(BME280_DIG_H5_REG) >> 4);
    calibration.dig_H6 = (int8_t)read_byte(BME280_DIG_H6_REG);
}


float BME280::get_temperature(void)
{
    float BME280_t = 0.0;

    int32_t temp1 = 0;
    int32_t temp2 = 0;
    int32_t adc_T = read_long(BME280_REGISTER_TEMPDATA);
    
    temp1 = (((((adc_T >> 3) - ((int32_t)calibration.dig_T1 << 1)))
	     * ((int32_t)calibration.dig_T2)) >> 11);

    temp2 = ((((((adc_T >> 4) - ((int32_t)calibration.dig_T1))
	     * ((adc_T >> 4) - ((int32_t)calibration.dig_T1))) >> 12)
	     * ((int32_t)calibration.dig_T3)) >> 14);
    
    t_fine = (temp2 + temp1);
        
    temp1 = (((t_fine * 5) + 128) >> 8);
    BME280_t = (temp1 / 100.0);

    BME280_t += t_offset;

    return BME280_t;
}


float BME280::get_pressure(void) 
{
    float BME280_p = 0.0;
    
    int64_t p = 0;
    int64_t temp1 = 0;
    int64_t temp2 = 0;

    int32_t adc_P = read_long(BME280_REGISTER_PRESSUREDATA);     

    temp1 = (((int64_t)t_fine) - 128000);

    temp2 = (temp1 * temp1 * (int64_t)calibration.dig_P6);

    temp2 = (temp2 + ((temp1 * (int64_t)calibration.dig_P5) << 17));

    temp2 = (temp2 + (((int64_t)calibration.dig_P4) << 35));

    temp1 = ((((temp1 * temp1) * (int64_t)calibration.dig_P3) >> 8)
	      + ((temp1 * (int64_t)calibration.dig_P2) << 12));

    temp1 = ((((((int64_t)1) << 47) + temp1))
	      * ((int64_t)calibration.dig_P1) >> 33);
    
    if(temp1 == 0)
    {
        BME280_p = 0;
    }
    
    p = (1048576 - adc_P);
    p = ((((p << 31) - temp2) * 3125) / temp1);
    
    temp1 = ((((int64_t)calibration.dig_P9)
	      * (p >> 13)
	      * (p >> 13)) >> 25);
    temp2 = ((((int64_t)calibration.dig_P8) * p) >> 19);
        
    p = (((p + temp1 + temp2) >> 8)
	  + (((int64_t)calibration.dig_P7) << 4));

    BME280_p = ((float)p / 256);

    BME280_p /= 100.0;
    BME280_p += p_offset;

    return BME280_p;
}


float BME280::get_relative_humidity(void)
{
    float BME280_rh = 0.0;

    int32_t v_x1_u32r = 0;
    int32_t adc_H = read_word(BME280_REGISTER_HUMIDDATA);

    v_x1_u32r = (t_fine - ((int32_t)76800));
    
    v_x1_u32r = (((((adc_H << 14)
	         - (((int32_t)calibration.dig_H4) << 20) -
                 (((int32_t)calibration.dig_H5) * v_x1_u32r))
	         + ((int32_t)16384)) >> 15)
	         * (((((((v_x1_u32r * ((signed long)calibration.dig_H6)) >> 10)
	         * (((v_x1_u32r * ((signed long)calibration.dig_H3)) >> 11)
	         + ((int32_t)32768))) >> 10)
	         + ((int32_t)2097152))
	         * ((int32_t)calibration.dig_H2) + 8192) >> 14));
    
    v_x1_u32r = (v_x1_u32r
	         - (((((v_x1_u32r >> 15)
	         * (v_x1_u32r >> 15)) >> 7)
	         * ((int32_t)calibration.dig_H1)) >> 4));
    
    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;

    if(v_x1_u32r > 419430400)
    {
    	v_x1_u32r = 419430400;
    }
   
    adc_H = (v_x1_u32r >> 12);
    BME280_rh = (adc_H / 1024.0);
    BME280_rh += rh_offset;

    return BME280_rh;
}