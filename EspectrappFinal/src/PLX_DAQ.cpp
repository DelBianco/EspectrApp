/*
  PLX_DAQ.cpp -
*/
#include "Arduino.h"
#include "PLX_DAQ.h"


PLX_DAQ::PLX_DAQ(int baudRate, char ComandoInicial){
  _baudRate = baudRate;
  _comando = ComandoInicial;
}

void PLX_DAQ::start(int timeout){
  Serial.begin(_baudRate);
  Serial.setTimeout(timeout);
  while (!Serial) {
    // wait Serial port initialization
  }
  Serial.println("MSG,Iniciando");
  Serial.println("CLEARDATA");
}

char PLX_DAQ::getComando(){
  return _comando;
}

char PLX_DAQ::getSubComando(){
  return _subComando;
}

void PLX_DAQ::recuperaComando(){
  _comando = _comandoOld;
}

void PLX_DAQ::setComando(char comando){
  _comandoOld = _comando;
  _comando = comando;
}

void PLX_DAQ::sendMSG(String msg){
  Serial.print("MSG,");
  Serial.println(msg);
}

void PLX_DAQ::parseLogs(){
  Serial.print("MSG,");
  Serial.println("parseando os logs");
  _comando = 'H';
}

void PLX_DAQ::resetRow(){
  Serial.println("RESETROW");
}

void PLX_DAQ::sendData(int x, int y){
  Serial.print("DATA");
  Serial.print(",");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}

float PLX_DAQ::getParam(int i){
  return _param[i];
}

bool PLX_DAQ::isConnected(){
  sendMSG("Conectado");
  return _isConnected;
}

String PLX_DAQ::getLog(int i){
  return _logs[i];
}

/**
 * Esta funcao seta as variaveis Comando, param1 e param2 elas precisam ser declaradas no inicio
 * Os parametros sao convertidos para Float mas podem ser tratados como int bastando substituir o parseFloat() para parseInt()
 * Alem de substituir na declaracao inicial.
 */
void  PLX_DAQ::leituraParametros() {
  int count = 0;
  String l = "";
  if(Serial.available()>0) {
    char tmpComando = (int)Serial.read();
    if(tmpComando == 'A' || tmpComando == 'B' || tmpComando == 'C'|| tmpComando == 'D'|| tmpComando == 'E' || tmpComando == 'H' || tmpComando == 'L') {
      setComando(tmpComando);
      _isConnected = true;
      char Separador = Serial.read();
      if(getComando() == 'E') {
        _param[0] = Serial.parseInt();
        _param[1] = Serial.parseInt();
        _param[2] = Serial.parseInt();
        _param[3] = Serial.parseInt();
      }
      if(getComando() == 'L') {
        char inChar;
        _subComando = Serial.read();
        char Separador = Serial.read();
        for(int i = 0; i < 50; i++){
          _logs[i] = "";
        }
        while(Serial.available()) {
          inChar = (int)Serial.read();
          if(inChar == ',') {
            _logs[count] = l;
            l = "";
            count++;
          } else {
            l += inChar;
          }
        }
      }
    }
  }
}
