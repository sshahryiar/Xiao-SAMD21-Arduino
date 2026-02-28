#include "Arduino.h"
#include "BME680.h"


static const uint32_t LUT1[16] = 
{
  UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647),
  UINT32_C(2147483647), UINT32_C(2126008810), UINT32_C(2147483647), UINT32_C(2130303777),
  UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2143188679), UINT32_C(2136746228),
  UINT32_C(2147483647), UINT32_C(2126008810), UINT32_C(2147483647), UINT32_C(2147483647)
};

static const uint32_t LUT2[16] = 
{
  UINT32_C(4096000000), UINT32_C(2048000000), UINT32_C(1024000000), UINT32_C(512000000),
  UINT32_C(255744255),  UINT32_C(127110228),  UINT32_C(64000000),   UINT32_C(32258064),
  UINT32_C(16016016),   UINT32_C(8000000),    UINT32_C(4000000),    UINT32_C(2000000),
  UINT32_C(1000000),    UINT32_C(500000),     UINT32_C(250000),     UINT32_C(125000)
};


BME680::BME680()
{
  _i2cPort = &Wire;
  _deviceAddress = BME680_I2C_ADDRESS;
  _Temperature = 2500;
}


uint8_t BME680::begin(TwoWire &wirePort, uint8_t deviceAddress)
{
  uint8_t retval = false;

  _i2cPort = &wirePort;
  _deviceAddress = deviceAddress;


  _i2cPort->begin();
  _i2cPort->flush();

  if(read_byte(BME680_CHIPID_REGISTER) == BME680_CHIP_ID)
  {
    write_byte(BME680_SOFTRESET_REGISTER, BME680_RESET_CODE);

    while(read_byte(BME680_SPI_REGISTER) & 0x01)
    {
      delay(100);
    }

    get_calibratoledn();
    retval = true;
  }

  return retval;
}


uint8_t BME680::set_oversampling(uint8_t sensorSelect, int8_t samplingValue)
{
  uint8_t tmp = 0;
  uint8_t retval = samplingValue;
  uint32_t timeout = millis() + BME680_MEASUREMENT_TIMEOUT;

  while(measurement_ongoing()) 
  {
    if(millis() > timeout) 
    {
      return false;
    }

    delay(10);
  }

  switch(sensorSelect)
  {
    case BME680_T_Sensor:
    {
      tmp = read_byte(BME680_CONTROL_MEASURE_REGISTER);

      if((samplingValue < BME680_OVERSAMPLING_SKIP) || (samplingValue > BME680_OVERSAMPLING_X16))
      {
          retval = ((tmp & ~0x1F) >> 5);  
      }
      else
      {
          tmp &= 0x1F;
          tmp |= (samplingValue << 5);
          write_byte(BME680_CONTROL_MEASURE_REGISTER, tmp);
      }

      break;
    }

    case BME680_RH_Sensor:
    {
      tmp = read_byte(BME680_CONTROL_HUMIDITY_REGISTER);

      if((samplingValue < BME680_OVERSAMPLING_SKIP) || (samplingValue > BME680_OVERSAMPLING_X16))
      {
        retval = (tmp & ~0xF8);
      }
      else
      {
        tmp &= 0xF8;
        tmp |= samplingValue;
        write_byte(BME680_CONTROL_HUMIDITY_REGISTER, tmp);
      }

      break;
    }

    case BME680_P_Sensor:
    {
      tmp = read_byte(BME680_CONTROL_MEASURE_REGISTER);

      if((samplingValue < BME680_OVERSAMPLING_SKIP) || (samplingValue > BME680_OVERSAMPLING_X16))
      {
        retval = ((tmp & ~0xE3) >> 2);
      }
      else
      {
        tmp &= 0xE3;
        tmp |= (samplingValue << 2);
        write_byte(BME680_CONTROL_MEASURE_REGISTER, tmp);
      }

      break;
    }

    default:
    {
      return 0xFF;
    }
  }

  return retval;
}


uint8_t BME680::set_IIR_filter(uint8_t selectIIRfilter)
{
  uint8_t retval = 0;
  uint32_t timeout = millis() + BME680_MEASUREMENT_TIMEOUT;

  while(measurement_ongoing()) 
  {
    if(millis() > timeout) 
    {
      return false;
    }
    
    delay(10);
  }

  retval = read_byte(BME680_CONFIG_REGISTER);

  if(selectIIRfilter <= BME680_IIR_FILTER_128)
  {
    retval &= 0xE3;
    retval |= ((selectIIRfilter & 0x07) << 2);
    write_byte(BME680_CONFIG_REGISTER, retval);
  }

  retval >>= 2;
  retval &= 0x07;

  return retval;
}


uint8_t BME680::gas_parameters(uint16_t gasTmp, uint16_t gasTime)
{
  uint8_t tmp, heater_res, factor, duratoledn;
  int32_t var1, var2, var3, var4, var5, heater_res_x100;
  uint32_t timeout = millis() + BME680_MEASUREMENT_TIMEOUT;

  factor = 0;

  while(measurement_ongoing()) 
  {
    if(millis() > timeout) 
    {
      return false;
    }
    
    delay(10);
  }

  tmp = read_byte(BME680_CONTROL_GAS_REGISTER_2);

  if((gasTime == 0) || (gasTmp == 0))
  {
    write_byte(BME680_CONTROL_GAS_REGISTER_1, 0x08);
    write_byte(BME680_CONTROL_GAS_REGISTER_2, (tmp & 0xEF));
  }
  else
  {
    write_byte(BME680_CONTROL_GAS_REGISTER_1, 0x00);

    if(gasTmp < 200)
    {
      gasTmp = 200;
    }
    else if(gasTmp > 400)
    {
      gasTmp = 400;
    }

    var1 = ((((int32_t)(_Temperature / 100) * _G3) / 1000) << 8);
    var2 = (((int32_t)_G1 + 784) * (((((int32_t)_G2 + 154009) * gasTmp * 5) / 100) + 3276800) / 10);
    var3 = var1 + (var2 / 2);
    var4 = (var3 / (_res_heat_range + 4));
    var5 = (131 * ((int32_t)_res_heat)) + 65536;
    heater_res_x100 = (int32_t)(((var4 / var5) - 250) * 34);
    heater_res = (uint8_t)((heater_res_x100 + 50) / 100);

    write_byte(BME680_GAS_HEATER_REGISTER_0, heater_res);

    if(gasTime >= 4032)
    {
      duratoledn = 0xFF;
    }
    else
    {
      while(gasTime >= 64)
      {
        gasTime >>= 2;
        factor++;
      };

      duratoledn = (uint8_t)(gasTime + (factor * 64));
    }

    write_byte(BME680_GAS_DURAToledN_REGISTER_0, duratoledn);
    write_byte(BME680_CONTROL_GAS_REGISTER_2, (tmp | 0x10));
  }

  return true;
}


uint8_t BME680::read(uint8_t waitState, float *temp, float *hum, float *dp, float *press, float *alt, float *gas)
{
  uint8_t  buffer[15], gas_range;                          
  uint16_t adc_hum, adc_gas_res;   
  uint32_t adc_temp, adc_pres;     
  uint32_t timeout = millis() + BME680_MEASUREMENT_TIMEOUT;
  int64_t  var1, var2, var3, var4, var5, var6, temp_scaled;
  uint64_t uvar2;

  trigger_measurement();
  if(waitState)
  {
    while(measurement_ongoing()) 
    {
      if(millis() > timeout) 
      {
        return false;
      }
      
      delay(10);
    }
  }
  else 
  {
    delay(250);
  }

  read_multi_byte(BME680_STATUS_REGISTER, buffer, 15);

  adc_pres = (uint32_t)(((uint32_t)buffer[2] << 12) | ((uint32_t)buffer[3] << 4) | ((uint32_t)buffer[4] >> 4));  // put the 3 bytes of Pressure
  adc_temp = (uint32_t)(((uint32_t)buffer[5] << 12) | ((uint32_t)buffer[6] << 4) | ((uint32_t)buffer[7] >> 4));  // put the 3 bytes of Temperature
  adc_hum = (uint16_t)(((uint32_t)buffer[8] << 8) | (uint32_t)buffer[9]);  // put the 2 bytes of Humidity
  adc_gas_res = (uint16_t)((uint32_t)buffer[13] << 2 | (((uint32_t)buffer[14]) >> 6));  // put the 2 bytes of Gas
  gas_range = (buffer[14] & 0x0F);                                           

  var1 = ((int32_t)adc_temp >> 3) - ((int32_t)_T1 << 1); 
  var2 = ((var1 * (int32_t)_T2) >> 11);                     
  var3 = (((var1 >> 1) * (var1 >> 1)) >> 12);              
  var3 = (((var3) * ((int32_t)_T3 << 4)) >> 14);
  _tfine = ((int32_t)(var2 + var3));
  _Temperature = ((int16_t)(((_tfine * 5) + 128) >> 8));

  var1 = ((((int32_t)_tfine) >> 1) - 64000);
  var2 = (((((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)_P6) >> 2));
  var2 = var2 + ((var1 * (int32_t)_P5) << 1);
  var2 = (var2 >> 2) + ((int32_t)_P4 << 16);
  var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * ((int32_t)_P3 << 5)) >> 3) + (((int32_t)_P2 * var1) >> 1);
  var1 = (var1 >> 18);
  var1 = (((32768 + var1) * (int32_t)_P1) >> 15);
  _Pressure = (1048576 - adc_pres);
  _Pressure = ((int32_t)((_Pressure - (var2 >> 12)) * ((uint32_t)3125)));

  if(_Pressure >= INT32_C(0x40000000))
  {
    _Pressure = ((_Pressure / (uint32_t)var1) << 1);
  }
  else
  {
    _Pressure = ((_Pressure << 1) / (uint32_t)var1);
  }

  var1 = (((int32_t)_P9 * (int32_t)(((_Pressure >> 3) * (_Pressure >> 3)) >> 13)) >> 12);
  var2 = (((int32_t)(_Pressure >> 2) * (int32_t)_P8) >> 13);
  var3 = (((int32_t)(_Pressure >> 8) * (int32_t)(_Pressure >> 8) * (int32_t)(_Pressure >> 8) * (int32_t)_P10) >> 17);
  _Pressure = ((int32_t)(_Pressure) + ((var1 + var2 + var3 + ((int32_t)_P7 << 7)) >> 4));

  temp_scaled = ((((int32_t)_tfine * 5) + 128) >> 8);
  var1 = (int32_t)(adc_hum - ((int32_t)((int32_t)_H1 << 4))) - (((temp_scaled * (int32_t)_H3) / ((int32_t)100)) >> 1);
  var2 = (((int32_t)_H2 * (((temp_scaled * (int32_t)_H4) / ((int32_t)100)) + (((temp_scaled * ((temp_scaled * (int32_t)_H5) / ((int32_t)100))) >> 6) / ((int32_t)100)) + (int32_t)(1 << 14))) >> 10);
  var3 = (var1 * var2);
  var4 = ((int32_t)_H6 << 7);
  var4 = ((var4 + ((temp_scaled * (int32_t)_H7) / ((int32_t)100))) >> 4);
  var5 = (((var3 >> 14) * (var3 >> 14)) >> 10);
  var6 = ((var4 * var5) >> 1);
  _Humidity = ((((var3 + var6) >> 10) * ((int32_t)1000)) >> 12);

  if(_Humidity >= 100000) 
  {
    _Humidity = 100000;
  }
  else if(_Humidity <= 0)
  {
    _Humidity = 0;
  }

  var1  = ((int64_t)((1340 + (5 * (int64_t)_rng_sw_err)) * ((int64_t)LUT1[gas_range])) >> 16);
  uvar2 = (((int64_t)((int64_t)adc_gas_res << 15) - (int64_t)(16777216)) + var1);
  var3  = (((int64_t)LUT2[gas_range] * (int64_t)var1) >> 9);
  _Gas  = ((uint32_t)((var3 + ((int64_t)uvar2 >> 1)) / (int64_t)uvar2));
         
  *gas = (((float)_Gas) / 1000.0);  
  *hum = (((float)_Humidity) / 1000.0);  
  *temp = (((float)_Temperature) / 100.0);  
  *press = (((float)_Pressure) / 100.0); 
  *dp = (*temp - ((100 - *hum) / 5.0)); 
  *alt = ((1013.25 - *press) * 8.5);

  return (buffer[14] & 0X30);
}


uint8_t BME680::read_byte(uint8_t reg) const
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  
  if(_i2cPort->endTransmission() != 0) 
  {
    return 0; 
  }
  
  _i2cPort->requestFrom(_deviceAddress, 1);
  
  if(_i2cPort->available()) 
  {
    return _i2cPort->read();
  }
  
  return 0;
}


int8_t BME680::read_signed_byte(uint8_t reg) 
{
  return ((int8_t)read_byte(reg));
}


void BME680::read_multi_byte(uint8_t addr, uint8_t *buffer, uint8_t numBytes)
{
  uint8_t idx = 0;   

  _i2cPort->beginTransmission(_deviceAddress); 
  _i2cPort->write(addr);

  if(_i2cPort->endTransmission() != 0)
  {
    memset(buffer, 0, numBytes);
    return;
  }

  _i2cPort->requestFrom(_deviceAddress, numBytes);

  while((_i2cPort->available()) && (idx < numBytes))   
  {
    buffer[idx++] = _i2cPort->read();               
  }
}


void BME680::write_byte(uint8_t reg, uint8_t value) 
{
  _i2cPort->beginTransmission(_deviceAddress); 
  _i2cPort->write(reg);
  _i2cPort->write(value);
  _i2cPort->endTransmission(); 
}


void BME680::get_calibratoledn(void)
{
  uint8_t coeff_array_1[BME680_COEFF_SIZE_1] = {0};      
  uint8_t coeff_array_2[BME680_COEFF_SIZE_2] = {0};

  read_multi_byte(BME680_COEFF_START_ADDR_1, coeff_array_1, BME680_COEFF_SIZE_1);
  read_multi_byte(BME680_COEFF_START_ADDR_2, coeff_array_2, BME680_COEFF_SIZE_2);

  _T1 = (uint16_t)(MAKE_WORD(coeff_array_2[BME680_T1_MSB_REG], coeff_array_2[BME680_T1_LSB_REG]));
  _T2 = (int16_t)(MAKE_WORD(coeff_array_1[BME680_T2_MSB_REG], coeff_array_1[BME680_T2_LSB_REG]));
  _T3 = (int8_t)(coeff_array_1[BME680_T3_REG]);

  _P1  = (uint16_t)(MAKE_WORD(coeff_array_1[BME680_P1_MSB_REG], coeff_array_1[BME680_P1_LSB_REG]));
  _P2  = (int16_t)(MAKE_WORD(coeff_array_1[BME680_P2_MSB_REG], coeff_array_1[BME680_P2_LSB_REG]));
  _P3  = (int8_t)coeff_array_1[BME680_P3_REG];
  _P4  = (int16_t)(MAKE_WORD(coeff_array_1[BME680_P4_MSB_REG], coeff_array_1[BME680_P4_LSB_REG]));
  _P5  = (int16_t)(MAKE_WORD(coeff_array_1[BME680_P5_MSB_REG], coeff_array_1[BME680_P5_LSB_REG]));
  _P6  = (int8_t)(coeff_array_1[BME680_P6_REG]);
  _P7  = (int8_t)(coeff_array_1[BME680_P7_REG]);
  _P8  = (int16_t)(MAKE_WORD(coeff_array_1[BME680_P8_MSB_REG], coeff_array_1[BME680_P8_LSB_REG]));
  _P9  = (int16_t)(MAKE_WORD(coeff_array_1[BME680_P9_MSB_REG], coeff_array_1[BME680_P9_LSB_REG]));
  _P10 = (uint8_t)(coeff_array_1[BME680_P10_REG]);

  _H1 = (coeff_array_2[BME680_H1_MSB_REG] << 4) | (coeff_array_2[BME680_H1_LSB_REG] & 0x0F);
  _H2 = (coeff_array_2[BME680_H2_MSB_REG] << 4) | (coeff_array_2[BME680_H2_LSB_REG] >> 4);

  _H3 = (int8_t)coeff_array_2[BME680_H3_REG];
  _H4 = (int8_t)coeff_array_2[BME680_H4_REG];
  _H5 = (int8_t)coeff_array_2[BME680_H5_REG];
  _H6 = (uint8_t)coeff_array_2[BME680_H6_REG];
  _H7 = (int8_t)coeff_array_2[BME680_H7_REG];

  _G1 = (int8_t)coeff_array_2[BME680_GH1_REG];
  _G2 = (int16_t)(MAKE_WORD(coeff_array_2[BME680_GH2_MSB_REG], coeff_array_2[BME680_GH2_LSB_REG]));
  _G3 = (int8_t)coeff_array_2[BME680_GH3_REG];

  _res_heat_range = ((read_byte(BME680_ADDR_RES_HEAT_RANGE_ADDR) & 0x30) / 16);
  _res_heat = read_signed_byte(BME680_ADDR_RES_HEAT_VAL_ADDR);
  _rng_sw_err = ((read_signed_byte(BME680_ADDR_RANGE_SW_ERR_ADDR) & (int8_t)0xF0) / 16);
}


uint8_t BME680::measurement_ongoing(void) const
{
  bool res = false;

  if(read_byte(BME680_STATUS_REGISTER) & BIT_MASK(0x05))
  {
    res = true;
  }

  return res;
}


void BME680::trigger_measurement(void)
{
  uint8_t tmp = read_byte(BME680_CONTROL_MEASURE_REGISTER);

  write_byte(BME680_CONTROL_MEASURE_REGISTER, (tmp | 1));
  while(read_byte(BME680_SPI_REGISTER) & 0x01)
  {
    delay(1);
  };
}

