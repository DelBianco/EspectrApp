/*
  Colors.h - [description]
*/
#include "Arduino.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "Color.h"

#ifndef View_h
#define View_h

//Definicao de cores
#define DARK            0x0000
#define SECONDARY       0x4249
#define SUCCESS         0xAFC2
#define WARNING         0xFD42
#define DANGER          0xF2E0
#define LIGHT           0xFFFF

class View{
  public:
    View();
    void start();
    void menu();
    void params(int);
    void drawGraph(int*,int,int);
    void drawBase(int,int,int,int,String);
    void storage();
    void listData(int,int,int);
    void debug(String,int);
    int height();
    int width();
    void alerta();
    void setAlerta(String);
    bool touched();
  private:
    Adafruit_TFTLCD * _tft;
    String _alerta = "Bem vindo ao EspectrApp";
    int _loadingCounter = 0;
    int _numPontos = 120;
    int _counter = 0;
    int _lambInicial = 400;
    int _lambFinal = 1000;
    int _ml = 39;
    int _mr = 39;
    int _mb = 20;
    int _dataOffset = 24;
    TouchScreen * _ts;
};

#endif
