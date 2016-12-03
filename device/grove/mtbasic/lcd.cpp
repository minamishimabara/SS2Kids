
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 55;
const int colorG = 55;
const int colorB = 55;

void lcdprint(char *p) 
{
    lcd.begin(16, 2);    
    lcd.setRGB(colorR, colorG, colorB);
    lcd.setCursor(0, 1);
    lcd.print(p);
}

