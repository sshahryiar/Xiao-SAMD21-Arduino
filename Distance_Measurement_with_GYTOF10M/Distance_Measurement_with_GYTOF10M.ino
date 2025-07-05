#include "SSD130x.h"
#include "GYTOF10M.h"


char buffer[20];
int16_t bar = 0;
uint16_t r_past = 10000;
uint16_t r_present = 0;

OLED display;
GYTOF10M tof;


void draw_background(void);


void setup(void) 
{
  pinMode(13, OUTPUT);
  tof.begin();
  display.begin();
  draw_background();
}


void loop() 
{
  r_present = tof.get_range();

  if((r_past != r_present) && (r_present <= 10000))
  {
    digitalWrite(13, HIGH);
    bar = map(r_past, 0, 10000, 4, 124);
    bar = constrain(bar, 4, 124);
    display.Rectangle(bar, 13, 124, 20, YES, NO, SQUARE);

    bar = map(r_present, 0, 10000, 4, 124);
    bar = constrain(bar, 4, 124);
    display.Rectangle(4, 13, bar, 20, YES, YES, SQUARE);

    sprintf(buffer, "%d mm ", r_present);
    display.Text(46, 7, buffer);
    r_past = r_present;
  }

  delay(1600);
  digitalWrite(13, LOW);
}


void draw_background(void)
{
  uint8_t i = 0;

  display.fill(0);

  display.Text(0, 0, "XIAO SAMD21 GYTOF10M");
  display.Text(40, 5, "Distance");
  
  display.Rectangle(2, 11, 126, 22, NO, YES, ROUND);
  for(i = 0; i < 13; i++)
  {
    display.V_Line((4 + (i * 10)), 25, 30, YES);
    if(i < 12)
    {
      display.V_Line((9 + (i * 10)), 25, 28, YES);
    }
  }
}
