/*
  Colors.h - [description]
*/
#include "Arduino.h"
#include <TouchScreen.h>

#ifndef Map_h
#define Map_h

class Map{
  public:
    Map(int,int);
    String mapRoute(String);
    String start();
    String menu();
    String params();
    String graph();
    String storage();
    String listData();
    void checkTouch();
    void getTouchScreen();
    void startLoggin();
  private:
    TouchScreen * _ts;
    TSPoint _p;
    uint8_t _clickBuffer = 0;
    int _unClickBuffer = 0;
    int _x = 0, _y = 0;
    bool _clicked = false;
    int _width;
    int _height;
    bool _loggin = false;
    int _count = 0;
};

#endif
