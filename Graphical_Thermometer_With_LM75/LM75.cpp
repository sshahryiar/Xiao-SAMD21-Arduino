#include "LM75.h"


LM75::LM75()
{
}


void LM75::begin(void)
{
  int8_t bytes[2] = {LM75_pointer_conf_reg, 
                    (LM75_queue_value_1 \
                    | LM75_OS_POL_LOW \
                    | LM75_OS_Comp \
                    | LM75_normal \
                    | LM75_12_bit_resolution \
                    | LM75_OS_mode_disable)};

  Wire.begin(D4, D5);
  Wire.setClock(100000);
  Wire.flush();
  delay(100);

  write_bytes(bytes, 2);
  delay(100);
}


float LM75::get_temp(void)
{
  float value = 0.0;

  value = ((float)(read_bytes(LM75_pointer_temp_reg, 2) >> 5));
  value *= 0.125;                                                                                      

  return value;
}


void LM75::write_bytes(int8_t *bytes, int8_t int8_t_length)
{
  int8_t i = 0;

  Wire.beginTransmission(LM75_I2C_Address); 

  for(i = 0; i < int8_t_length; i++)
  {
    Wire.write(*bytes++);  
  }         
             
  Wire.endTransmission(); 
}


int32_t LM75::read_bytes(int8_t pointer, int8_t bytes_length)
{
  int8_t bytes[bytes_length];
  int8_t i = 0;
  int32_t value = 0;
  
  Wire.beginTransmission(LM75_I2C_Address); 
  Wire.write(pointer);        
  Wire.endTransmission(); 

  Wire.requestFrom(LM75_I2C_Address, bytes_length);    

  if(Wire.available() <= bytes_length) 
  { 
    for(i = 0; i < bytes_length; i++)
    {
      value <<= 8;
      bytes[i] = Wire.read();
      value |= bytes[i];
    }
  }

  return value;
}
