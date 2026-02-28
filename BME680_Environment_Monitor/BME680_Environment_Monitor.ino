#include <math.h>
#include <Wire.h>
#include "BME680.h"
#include "SSD130x.h"
#include "bitmap.h"


#define Temperature_Display       0
#define Humidity_Display          1
#define Air_Pressure_Display      2
#define Dew_Point_Display         3
#define AQI_Display               4

#define scroll_speed              4000


char buffer[40];
char floatStr[20];
uint8_t display_switcher = 0;
uint16_t tmp1 = 0;
uint16_t tmp2 = 0;
float tmp3 = 0;
float T, RH, P, G, DP, ALT;


BME680 bme; 
OLED oled;


float map_value(float value, float xmin, float xmax, float ymin, float ymax)
{
  return (ymin + (((ymax - ymin) / (xmax - xmin)) * (value - xmin)));
}


void setup(void) 
{
  Serial.begin(9600);
  bme.begin();
  oled.begin();
  bme.set_oversampling(BME680_T_Sensor, BME680_OVERSAMPLING_X16);
  bme.set_oversampling(BME680_RH_Sensor, BME680_OVERSAMPLING_X16);
  bme.set_oversampling(BME680_P_Sensor, BME680_OVERSAMPLING_X8);
  bme.set_IIR_filter(BME680_IIR_FILTER_32);
  bme.gas_parameters(320, 150);
}

void loop(void) 
{
  bme.read(true, &T, &RH, &DP, &P, &ALT, &G);

  Serial.print("Temp: ");    
  Serial.print(T, 2);  
  Serial.println(" 'C");

  Serial.print("Humidity: "); 
  Serial.print(RH, 1); 
  Serial.println(" %");
  
  Serial.print("Pressure: "); 
  Serial.print(P, 2);  
  Serial.println(" hPa");

  Serial.print("Gas: ");     
  Serial.print(G);              
  Serial.println(" kOhm");

  Serial.print("Dew Point: ");     
  Serial.print(DP);              
  Serial.println(" 'C");

  Serial.print("Altitude: ");     
  Serial.print(ALT);              
  Serial.println(" m");

  for(display_switcher = 0; display_switcher <= 4; display_switcher++)
  {
    oled.fill(0);

    switch(display_switcher)
    {
      case Humidity_Display:
      {
        oled.print_image(gauge_dial, false);
        
        tmp3 = constrain(RH, 0, 100);
        tmp3 = map_value(tmp3, 0, 100, -1.571, 1.571);
        oled.Circle(63, 63, 3, true, true);
        oled.Line(63, 63, ((int16_t)(63 + (43 * sin(tmp3)))), (63 - ((int16_t)(43 * cos(tmp3)))), true);

        oled.Text(0, 0, "RH/%");
        dtostrf(RH, 3, 2, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(95, 0, buffer);
        break;
      }

      case Air_Pressure_Display:
      {
        if((P >= 960) && (P < 980))
        {
          oled.Bitmap(0, 1, 48, 6, stormy);
        }
        
        else if((P >= 980) && (P < 1000))
        {
          oled.Bitmap(0, 1, 48, 6, rainy);
        }

        else if((P >= 1000) && (P < 1020))
        {
          oled.Bitmap(0, 1, 48, 6, cloudy);
        }

        else if((P >= 1020) && (P < 1070))
        {
          oled.Bitmap(0, 1, 48, 6, sunny);
        }

        oled.Text(75, 1, "P/hPA:");
        dtostrf(P, 4, 1, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(75, 3, buffer);

        oled.Text(75, 5, "Alt/m:");
        dtostrf(ALT, 3, 1, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(75, 7, buffer);

        break;
      }

      case Dew_Point_Display:
      { 
        oled.print_image(gauge_dial, false);
      
        tmp3 = constrain(DP, 0, 100);
        tmp3 = map_value(tmp3, 0, 100, -1.571, 1.571);
        oled.Circle(63, 63, 3, true, true);
        oled.Line(63, 63, ((int16_t)(63 + (43 * sin(tmp3)))), (63 - ((int16_t)(43 * cos(tmp3)))), true);
 
        oled.Text(0, 0, "DP/'C");
        dtostrf(DP, 3, 2, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(95, 0, buffer);
        break;
      }

      case AQI_Display:
      {
        oled.Bitmap(0, 5, 128, 7, aqi);

        oled.Text(36, 0, "Air Quality");
        dtostrf(G, 3, 2, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(50, 2, buffer);

        if(G >= 100)
        {
          tmp1 = 4;
        }
        else if((G >= 50) and (G < 100))
        {
          tmp1 = 3;
        } 
        else if((G >= 10) and (G < 50))
        {
          tmp1 = 2;
        }       
        else if((G >= 1) and (G < 10))
        {
          tmp1 = 1;
        } 
        else
        {
          tmp1 = 0;
        } 

        oled.Triangle((13 + (tmp1 * 26)), 35, (10 + (tmp1 * 26)), 32, (16 + (tmp1 * 26)), 32, true, true);
        break;
      }

      default:
      {
        oled.Bitmap(0, 0, 30, 7, thermo);

        oled.Text(64, 0, "BME680");
        oled.Text(36, 1, "Weather Station");

        oled.Text(70, 4, "T/'C");
        dtostrf(T, 3, 2, floatStr);
        sprintf(buffer, "%s ", floatStr);
        oled.Text(70, 6, buffer);

        tmp1 = constrain(T, 0, 100);
        tmp2 = map_value(tmp1, 0, 100, 53, 3);
        oled.V_Line(5, 53, tmp2, true);        
        break;
      }
    }

    delay(scroll_speed);
  }
  
}
