#pragma once


#include <Arduino.h>
#include <Wire.h>


#define LM75_I2C_Address                     0x48       

//Registers                                                                                                          
                                                                                                          
#define LM75_pointer_temp_reg                0x00
#define LM75_pointer_conf_reg                0x01
#define LM75_pointer_thyst_reg               0x02
#define LM75_pointer_tos_reg                 0x03
                                                                                 
//Configure Register Bits     

#define LM75_queue_value_1                   0x00
#define LM75_queue_value_2                   0x08
#define LM75_queue_value_4                   0x10
#define LM75_queue_value_6                   0x18

#define LM75_9_bit_resolution                0x00
#define LM75_10_bit_resolution               0x20
#define LM75_11_bit_resolution               0x40
#define LM75_12_bit_resolution               0x60

#define LM75_OS_POL_LOW                      0x00
#define LM75_OS_POL_HIGH                     0x04

#define LM75_OS_Comp                         0x00                   
#define LM75_OS_INT                          0x02
                                        
#define LM75_normal                          0x00                       
#define LM75_shutdown                        0x01
 
#define LM75_OS_mode_disable                 0x00
#define LM75_OS_mode_enable                  0x80


class LM75
{
    public:
      LM75();
      void begin(void);
      float get_temp(void);

    private:
      void write_bytes(int8_t *bytes, int8_t bytes_length);
      int32_t read_bytes(int8_t pointer, int8_t bytes_length);     
};
