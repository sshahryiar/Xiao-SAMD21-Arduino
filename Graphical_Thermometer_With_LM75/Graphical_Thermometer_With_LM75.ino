#include "LM75.h"
#include "SSD130x.h"

LM75 ts;
OLED oled;


char buffer[40];
char floatStr[20];

int16_t bar = 0;
float t_past = 90;
float t_present = 0;


void setup(void)
{
  int8_t i = 0;

  ts.begin();

  oled.begin();
  oled.fill(0);
  
  oled.Circle(9, 55, 5, false, true);
  oled.Circle(9, 55, 3, true, true);
  oled.V_Line(7, 2, 49, true);
  oled.V_Line(11, 2, 49, true);
  oled.H_Line(8, 10, 1, true);

  for(i = 0; i < 10; i++)
  {
    oled.H_Line(16, 20, (4 + (5 * i)), true);
  }

  oled.Text(23, 0, "90");
  oled.Text(23, 3, "45");
  oled.Text(23, 6, "0");

  oled.Text(52, 1, "XIAO - LM75");
  oled.Text(59, 3, "Tc/deg C:");
}


void loop(void)
{
  t_present = ts.get_temp();

  if(t_present != t_past)
  {
    bar = map(t_past, 0, 90, 49, 4);
    bar = constrain(bar, 4, 49);
    oled.V_Line(9, 49, bar, false);
    bar = map(t_present, 0, 90, 49, 4);
    bar = constrain(bar, 4, 49);
    oled.V_Line(9, 49, bar, true);

    dtostrf(t_present, 2, 3, floatStr);
    sprintf(buffer, "%s ", floatStr);
    oled.Text(66, 5, buffer);

    t_past = t_present;
  }

  delay(600);
}
