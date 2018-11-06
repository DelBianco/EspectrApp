/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"
#include "Controller.h"

#ifndef Espectrapp_h
#define Espectrapp_h

class Espectrapp{
  public:
    Espectrapp();
    void routeTo(String);
    void loop();
    void route();
    void fakeData();
    void setData(int,int);
    void setTempoIntegracao(int);
    void setIncremento(int);
    void setNumMedias(int);
    void setNumLog(int);
    void dataChanged();
    void setScale(int,int);
    int getScale(int);
    void sendPhoton(String);
  private:
    String _route = "start";
    Controller * _controller;
    int _data[145];
    int _numPontos = 120;
    int _page = 0;
    int _logid = 0;
    int _numPaginas = 1;
    int _numLogs = 1;
    int _incremento = 25;
    int _tempoIntegracao = 100;
    int _tempoIntegracaoDefault = 100;
    int _numMedias = 1;
    int _scale[4] = {1000,800,600,400};
};

#endif
