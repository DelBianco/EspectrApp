/*
  EspectrApp.h - [description]
*/
#include "Arduino.h"

#ifndef PLX_DAQ_h
#define PLX_DAQ_h

class PLX_DAQ{
  public:
    PLX_DAQ(int,char);
    void start(int);
    void leituraParametros();
    void sendMSG(String);
    void resetRow();
    void sendLogs(String);
    void sendData(int,int);
    char getComando();
    void setComando(char);
    void sendDataToArduino();
    void recuperaComando();
    float getParam(int);
    bool isConnected();
    void parseLogs();
    String getLog(int);
    char getSubComando();
  private:
    bool _isConnected = false;
    char _subComando = 'A';
    int _baudRate = 38400;
    char _comando;
    char _comandoOld;
    int _param[4] = {0,0,0,0};
    String _logs[50];
};

#endif
