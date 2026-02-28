#pragma once


#define BME680_I2C_ADDRESS                      0x77

#define BME680_STATUS_REGISTER                  0x1D           
#define BME680_GAS_HEATER_REGISTER_0            0x5A
#define BME680_GAS_DURAToledN_REGISTER_0          0x64
#define BME680_CONTROL_GAS_REGISTER_1           0x70
#define BME680_CONTROL_GAS_REGISTER_2           0x71
#define BME680_CONTROL_HUMIDITY_REGISTER        0x72
#define BME680_SPI_REGISTER                     0x73
#define BME680_CONTROL_MEASURE_REGISTER         0x74
#define BME680_CONFIG_REGISTER                  0x75
#define BME680_CHIPID_REGISTER                  0xD0
#define BME680_SOFTRESET_REGISTER               0xE0
#define BME680_CHIP_ID                          0x61
#define BME680_RESET_CODE                       0xB6

#define BME680_COEFF_START_ADDR_1               0x89
#define BME680_COEFF_START_ADDR_2               0xE1

#define BME680_COEFF_SIZE_1                     25 
#define BME680_COEFF_SIZE_2                     16

#define BME680_T1_LSB_REG                       8
#define BME680_T1_MSB_REG                       9
#define BME680_T2_LSB_REG                       1            
#define BME680_T2_MSB_REG                       2
#define BME680_T3_REG                           3 

#define BME680_H1_LSB_REG                       1                   
#define BME680_H1_MSB_REG                       2 
#define BME680_H2_MSB_REG                       0                   
#define BME680_H2_LSB_REG                       1                                     
#define BME680_H3_REG                           3                       
#define BME680_H4_REG                           4                       
#define BME680_H5_REG                           5                       
#define BME680_H6_REG                           6                       
#define BME680_H7_REG                           7     

#define BME680_P1_LSB_REG                       5
#define BME680_P1_MSB_REG                       6
#define BME680_P2_LSB_REG                       7
#define BME680_P2_MSB_REG                       8
#define BME680_P3_REG                           9                     
#define BME680_P4_LSB_REG                       11
#define BME680_P4_MSB_REG                       12
#define BME680_P5_LSB_REG                       13
#define BME680_P5_MSB_REG                       14
#define BME680_P7_REG                           15
#define BME680_P6_REG                           16
#define BME680_P8_LSB_REG                       19
#define BME680_P8_MSB_REG                       20
#define BME680_P9_LSB_REG                       21
#define BME680_P9_MSB_REG                       22
#define BME680_P10_REG                          23

#define BME680_GH2_LSB_REG                      10
#define BME680_GH2_MSB_REG                      11
#define BME680_GH1_REG                          12
#define BME680_GH3_REG                          13                  

#define BME680_ADDR_RES_HEAT_VAL_ADDR           0x00
#define BME680_ADDR_RES_HEAT_RANGE_ADDR         0x02
#define BME680_ADDR_RANGE_SW_ERR_ADDR           0x04

#define BME680_T_Sensor                         0
#define BME680_RH_Sensor                        1
#define BME680_P_Sensor                         2
#define BME680_G_Sensor                         3

#define BME680_OVERSAMPLING_SKIP                0
#define BME680_OVERSAMPLING_X1                  1
#define BME680_OVERSAMPLING_X2                  2
#define BME680_OVERSAMPLING_X4                  3
#define BME680_OVERSAMPLING_X8                  4
#define BME680_OVERSAMPLING_X16                 5

#define BME680_IIR_FILTER_OFF                   0
#define BME680_IIR_FILTER_2                     1
#define BME680_IIR_FILTER_4                     2
#define BME680_IIR_FILTER_8                     3
#define BME680_IIR_FILTER_16                    4
#define BME680_IIR_FILTER_32                    5
#define BME680_IIR_FILTER_64                    6 
#define BME680_IIR_FILTER_128                   7

#define BME680_MEASUREMENT_TIMEOUT              1000
