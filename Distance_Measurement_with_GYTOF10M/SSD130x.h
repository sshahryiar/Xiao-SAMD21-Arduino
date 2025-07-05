#pragma once


#include <Arduino.h>
#include <Wire.h>


#define SSD1306_I2C_Address                               0x3C

#define SSD1306_Set_Lower_Column_Start_Address_CMD        0x00
#define SSD1306_Set_Higher_Column_Start_Address_CMD       0x10
#define SSD1306_Set_Memory_Addressing_Mode_CMD            0x20
#define SSD1306_Set_Column_Address_CMD                    0x21
#define SSD1306_Set_Page_Address_CMD                      0x22
#define SSD1306_Set_Display_Start_Line_CMD                0x40
#define SSD1306_Set_Contrast_Control_CMD                  0x81
#define SSD1306_Set_Charge_Pump_CMD                       0x8D
#define SSD1306_Set_Segment_Remap_CMD                     0xA0
#define SSD1306_Set_Entire_Display_ON_CMD                 0xA4
#define SSD1306_Set_Normal_or_Inverse_Display_CMD         0xA6
#define SSD1306_Set_Multiplex_Ratio_CMD                   0xA8
#define SSD1306_Set_Display_ON_or_OFF_CMD                 0xAE
#define SSD1306_Set_Page_Start_Address_CMD                0xB0
#define SSD1306_Set_COM_Output_Scan_Direction_CMD         0xC0
#define SSD1306_Set_Display_Offset_CMD                    0xD3
#define SSD1306_Set_Display_Clock_CMD                     0xD5
#define SSD1306_Set_Pre_charge_Period_CMD                 0xD9
#define SSD1306_Set_Common_HW_Config_CMD                  0xDA
#define SSD1306_Set_VCOMH_Level_CMD                       0xDB
#define SSD1306_Set_NOP_CMD                               0xE3

#define SSD1306_Horizontal_Addressing_Mode                0x00
#define SSD1306_Vertical_Addressing_Mode                  0x01
#define SSD1306_Page_Addressing_Mode                      0x02

#define SSD1306_Disable_Charge_Pump                       0x00
#define SSD1306_Enable_Charge_Pump                        0x04

#define SSD1306_Column_Address_0_Mapped_to_SEG0           0x00
#define SSD1306_Column_Address_0_Mapped_to_SEG127         0x01

#define SSD1306_Normal_Display                            0x00
#define SSD1306_Entire_Display_ON                         0x01

#define SSD1306_Non_Inverted_Display                      0x00
#define SSD1306_Inverted_Display                          0x01

#define SSD1306_Display_OFF                               0x00
#define SSD1306_Display_ON                                0x01

#define SSD1306_Scan_from_COM0_to_63                      0x00
#define SSD1306_Scan_from_COM63_to_0                      0x08

#define SSD1306_DAT                                       0x60
#define SSD1306_CMD                                       0x00

#define x_size                                            128
#define x_max                                             x_size
#define x_min                                             0
#define y_size                                            64
#define y_max                                             8
#define y_min                                             0

#define ON                                                true
#define OFF                                               false

#define YES                                               true
#define NO                                                false

#define Full_Circle                                       0
#define Upper                                             1
#define Lower                                             2

#define SQUARE                                            false
#define ROUND                                             true

#define buffer_size                                       1024//(x_max * y_max)


class OLED
{
  public:
    uint8_t buffer[buffer_size];

    OLED();
    void begin(void);
    void gotoxy(uint8_t x_pos, uint8_t y_pos);
    void fill(uint8_t bmp_data);
    void clear_screen(void);
    void clear_buffer(void);
    void cursor(int16_t x_pos, int16_t y_pos);
    void print_image(const uint8_t* bmp, uint8_t pixel);
    void chr(int16_t x_pos, int16_t y_pos, char ch);
    void Text(int16_t x_pos, int16_t y_pos, const char *ch);
    void Bitmap(int16_t xb, int16_t yb, int16_t xe, int16_t ye, uint8_t *bmp_img);
    void Pixel(int16_t x_pos, int16_t y_pos, uint8_t colour);
    void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colour);
    void V_Line(int16_t x1, int16_t y1, int16_t y2, uint8_t colour);
    void H_Line(int16_t x1, int16_t x2, int16_t y1, uint8_t colour);
    void Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t fill, uint8_t colour, uint8_t type);
    void Circle(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t colour);
    void Point(int16_t xc, int16_t yc, int16_t size, uint8_t colour);
    void Gauge(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t U_L, uint8_t colour);
    void Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t fill, uint8_t colour);

  private:
    void swap(int16_t *a, int16_t *b);
    void write_byte(uint8_t value, uint8_t control_byte);
    void Arc(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t circle_type, uint8_t colour);
};