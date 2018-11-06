/*
  Colors.h - [description]
*/
#include "Arduino.h"

#ifndef Color_h
#define Color_h

//Definicao de cores
#define DARK            0x0000
#define SECONDARY       0x4249
#define SUCCESS         0xAFC2
#define WARNING         0xFD42
#define DANGER          0xF2E0
#define LIGHT           0xFFFF

class Color{
  public:
    Color(int,int,int);
    int getColorFromRGB(int,int,int);
    int getColorFromRGB();
    int getColorFromHSV(uint16_t,uint8_t,uint8_t);
  private:
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;

    uint16_t _h;
    uint16_t _s;
    uint16_t _v;
};

#endif
