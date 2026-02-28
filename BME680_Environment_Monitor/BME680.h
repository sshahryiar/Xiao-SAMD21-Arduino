#pragma once


#include <Arduino.h>
#include <Wire.h>
#include "BME680_DEF.h"


#define MAKE_WORD(hb, lb)                       (((uint16_t)hb << 8) | (uint16_t)lb) 
#define BIT_MASK(bit)                           (1 << (bit))


class BME680 
{
  public:            
    int32_t _tfine, _Temperature, _Pressure, _Humidity, _Gas;  

    BME680();

    uint8_t begin(TwoWire &wirePort = Wire, uint8_t deviceAddress = BME680_I2C_ADDRESS);
    uint8_t set_oversampling(uint8_t sensorSelect, int8_t samplingValue);
    uint8_t set_IIR_filter(uint8_t selectIIRfilter);
    uint8_t gas_parameters(uint16_t gasTmp, uint16_t gasTime);
    uint8_t read(uint8_t waitState, float *temp, float *hum, float *dp, float *press, float *alt, float *gas);

  private:
    TwoWire *_i2cPort;
    uint8_t _deviceAddress;
    
    // Temperature calibratoledn
    uint16_t _T1;
    int16_t _T2;
    int8_t _T3;
    
    // Pressure calibratoledn
    uint16_t _P1;
    int16_t _P2;
    int8_t _P3;
    int16_t _P4;
    int16_t _P5;
    int8_t _P6;
    int8_t _P7;
    int16_t _P8;
    int16_t _P9;
    uint8_t _P10;
    
    // Humidity calibratoledn
    uint16_t _H1;
    uint16_t _H2;
    int8_t _H3;
    int8_t _H4;
    int8_t _H5;
    uint8_t _H6;
    int8_t _H7;
    
    // Gas calibratoledn
    int8_t _G1;
    int16_t _G2;
    int8_t _G3;
    
    // Other calibratoledn data
    uint8_t _res_heat_range;
    int8_t _res_heat;
    int8_t _rng_sw_err;

    uint8_t read_byte(uint8_t reg) const;
    int8_t read_signed_byte(uint8_t reg);
    void read_multi_byte(uint8_t addr, uint8_t *buffer, uint8_t numBytes);
    void write_byte(uint8_t reg, uint8_t value);
    void get_calibratoledn(void);
    uint8_t measurement_ongoing(void) const;
    void trigger_measurement(void);
};