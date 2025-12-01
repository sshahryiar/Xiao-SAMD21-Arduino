#include "BME280.h"
#include "SSD130x.h"


BME280 bme;
OLED display;


char buffer[40];
char floatStr[20];

float t = 0;
float p = 0;
float rh = 0;


void draw_background(void)
{
  display.fill(false);

  display.Text(8, 0, "Xiao Weather Station");

  display.Circle(5, 23, 3, true, true);
  display.V_Line(5, 11, 23, true);
  display.V_Line(3, 8, 21, true);
  display.V_Line(7, 8, 21, true);
  display.H_Line(4, 6, 7, true);

  display.H_Line(1, 6, 34, true);
  display.H_Line(1, 8, 36, true);
  display.H_Line(1, 4, 39, true);
  display.H_Line(8, 9, 41, true);
  display.H_Line(3, 4, 42, true);
  display.Pixel(2, 41, true);
  display.Pixel(6, 31, true);
  display.Pixel(7, 30, true);
  display.Pixel(7, 33, true);
  display.Pixel(9, 37, true);
  display.V_Line(5, 40, 41, true);
  display.V_Line(7, 39, 40, true);
  display.V_Line(8, 31, 32, true);
  display.V_Line(10, 38, 40, true);
  
  display.V_Line(3, 46, 53, true);
  display.Line(1, 51, 3, 53, true);
  display.Line(5, 51, 3, 53, true);
  display.V_Line(9, 46, 53, true);
  display.Line(7, 51, 9, 53, true);
  display.Line(11, 51, 9, 53, true);
  display.H_Line(1, 11, 55, true);
  display.H_Line(1, 3, 57, true);
  display.H_Line(5, 7, 57, true);
  display.H_Line(9, 11, 57, true);
  display.H_Line(1, 4, 59, true);
  display.H_Line(8, 11, 59, true);
}


void setup(void) 
{
  display.begin();
  bme.begin();

  draw_background();
}


void loop(void) 
{
  t = bme.get_temperature();
  dtostrf(t, 2, 2, floatStr);
  sprintf(buffer, "%s'C ", floatStr);
  display.Text(20, 2, buffer);

  rh = bme.get_relative_humidity();
  dtostrf(rh, 2, 2, floatStr);
  sprintf(buffer, "%s%%", floatStr);
  display.Text(20, 4, buffer);

   p = bme.get_pressure();
  dtostrf(p, 2, 2, floatStr);
  sprintf(buffer, "%s mbar ", floatStr);
  display.Text(20, 6, buffer);

  delay(1000);
}
