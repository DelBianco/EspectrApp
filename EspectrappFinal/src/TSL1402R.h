/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"
#include "SdFat.h"
#include <math.h>

#ifndef TSL1402R_h
#define TSL1402R_h

class TSL1402R{
  public:
    TSL1402R(unsigned int);
    void start();
    void coletaDados();
    void demoData();
    void integra();
    void limpaSensor();
    void clockPulse();
    void siPulse();
    void clearData();
    void leConfig();

    void setLinhaDeBase();
    void setTint(unsigned int);
    void setNumMedias(unsigned int);
    void setData(int,unsigned int);
    void setIncremento(unsigned int);
    void parseLog(String, char);

    unsigned int getIncremento();
    unsigned int getTint();
    unsigned int getNumMedias();

    unsigned int getData(int);
    unsigned int getTransmissao(int);
    unsigned int getBase(int);
    unsigned int getAbsorbancia(int);
    unsigned int getLogData(int);

    float getScale(int);
  private:
    unsigned int _tInt = 500;
    unsigned int _numMedias = 1;
    unsigned int _incremento = 25;
    int _multiplicador = 59;
    int _clockSize = 10;
    unsigned int _data[256];
    unsigned int _dataTemp[256];
    unsigned int _dataLog[256];
    unsigned int _base[256];
    int _scaleMin;
    int _scaleMax;
    float _scale[4];
};

#endif
