/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "Color.h"


Color::Color(int r, int g, int b){
  _r = r;
  _g = g;
  _b = b;
}

int Color::getColorFromRGB(int r, int g, int b){
  _r = r;
  _g = g;
  _b = b;
  return ( ((_r & 0xF8) << 8) | ((_g & 0xFC) << 3) | (_b >> 3) );
}

int Color::getColorFromRGB(){
  return ( ((_r & 0xF8) << 8) | ((_g & 0xFC) << 3) | (_b >> 3) );
}
double doubleMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 temp = (int) (4*temp + .5);
 return (double) temp/4;
}

int Color::getColorFromHSV(uint16_t index, uint8_t sat, uint8_t bright){
  uint8_t temp[5], n = (index >> 8) % 3;
  temp[0] = temp[3] = (uint8_t)(((sat ^ 255)  * bright) / 255);
  temp[1] = temp[4] = (uint8_t)((((((index & 255)*sat)/255) + (sat ^ 255)) * bright) / 255);
  temp[2] = (uint8_t)(((((((index & 255) ^ 255) * sat) / 255) + (sat ^ 255)) * bright) / 255);
  _r  = temp[n + 2];
  _g  = temp[n + 1];
  _b  = temp[n + 0];
  return getColorFromRGB();
}
