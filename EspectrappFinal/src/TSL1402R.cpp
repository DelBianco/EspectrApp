/* TSL1402R.cpp */

#include "Arduino.h"
#include "TSL1402R.h"
#include "SdFat.h"
#include <math.h>

TSL1402R::TSL1402R(unsigned int tInt){
  _tInt = tInt;
  _numMedias = 1;
  _clockSize = 10;
  TSL1402R::clearData();
}

void TSL1402R::start(){
  leConfig();
  TSL1402R::siPulse(); // colocando o SI em 0
  TSL1402R::clockPulse(); // colocando o Pulse em 0 (relativo ao setup)
}

void TSL1402R::demoData(){
  int placeholder[145] = {48,67,92,107,126,134,125,114,123,130,133,134,135,139,141,150,152,155,151,143,123,93,67,48,31,21,11,7,8,16,53,175,257,242,352,514,981,468,121,44,18,10,19,72,268,563,957,1259,1229,826,419,151,64,75,225,592,1196,1257,1172,767,148,45,11,9,10,3,85,156,224,84,19,5,3,2,7,30,39,49,26,10,13,19,52,255,711,1069,979,312,80,43,32,25,36,182,470,752,563,561,257,91,82,88,158,200,195,210,106,37,31,28,64,75,43,39,36,39,46,55,58,77,84,90,105,164,153,132,116,127,121,121,117,125,121,123,117,121,114,121,117,129,127,130,127,129,138};
  TSL1402R::integra();
  for(int i=0; i < 256; i++){
    int val = 0;
    if(i>_scaleMin && i<_scaleMax){
      int randseed = placeholder[i-_scaleMin]/5;
      val = placeholder[i-_scaleMin] + random(-randseed,randseed);
    }
    setData(i,val);
  }
}

void TSL1402R::coletaDados(){
  // Opçao A
  TSL1402R::siPulse();
  TSL1402R::limpaSensor();
  for(int i = 0; i< _numMedias; i++){
    //*********Inicio da Medida Real
    TSL1402R::siPulse();
    TSL1402R::integra(); //********Tempo de Integração********
    TSL1402R::siPulse();
    for(int i=0; i < 256; i++){
      delayMicroseconds(20);// <-- We add a delay to stabilize the AO output from the sensor
      _data[i] += analogRead(A0);
      TSL1402R::clockPulse();
    }
  }
  // calculando a media
  for(int i = 0; i < 256; i++){
    //_data[i] = (int)_data[i]/_numMedias;
    setData(i,(int)_data[i]/_numMedias);
  }
}

void TSL1402R::integra(){
  long t = (long)_tInt*_multiplicador;
  //unsigned long ti = micros();
  for(long ii = 0; ii < t; ii++){
    if(ii==18){
      // **********Possivel sinal de Trig*************
    }
    TSL1402R::clockPulse();
  }
  /*unsigned long tf = micros();
  Serial.print("Ti: ");
  Serial.println(ti);
  Serial.print("Tf: ");
  Serial.println(tf);
  Serial.print("_tInt: ");
  Serial.println(_tInt);
  Serial.print("_multiplicador: ");
  Serial.println(_multiplicador);*/
}

void TSL1402R::limpaSensor(){
  for(int i=0; i < 256; i++){
    delayMicroseconds(20);
    _data[i] = 0;
    TSL1402R::clockPulse();
  }
}

void TSL1402R::clockPulse(){
  digitalWrite(D2, HIGH);
  delayMicroseconds(_clockSize);
  digitalWrite(D2, LOW);
}

void TSL1402R::siPulse(){
  digitalWrite(D4, HIGH);
  TSL1402R::clockPulse();
  digitalWrite(D4, LOW);
}

void TSL1402R::leConfig(){
  File configFile;
  if (!configFile.open("config.dat", O_READ )) {
      Serial.println("MSG,Erro ao ler arquivo de log, arquivo nao encontrado");
  }else{
    // read from the file until there's nothing else in it:
    int data;
    String paramName = "";
    String value = "";
    while ((data = configFile.read()) >= 0) {
      if((char)data == '='){
        data = configFile.read();
        while((char)data != '\n' && data >= 0){
          value += (char)data;
          data = configFile.read();
          Serial.print(data);
        }
        if(paramName == "tempo_integracao"){
          setTint(value.toInt());
        }
        if(paramName == "numero_medias"){
          setNumMedias(value.toInt());
        }
        if(paramName == "incremento_tint"){
          _incremento = value.toInt();
        }
        if(paramName == "multiplicador"){
          _multiplicador = value.toInt();
        }
        if(paramName == "scale_min"){
          _scaleMin = value.toInt();
        }
        if(paramName == "scale_max"){
          _scaleMax = value.toInt();
        }
        if(paramName == "scale_a"){
          _scale[0] = value.toFloat();
        }
        if(paramName == "scale_b"){
          _scale[1] = value.toFloat();
        }
        if(paramName == "scale_c"){
          _scale[2] = value.toFloat();
        }
        if(paramName == "scale_d"){
          _scale[3] = value.toFloat();
        }
        if(data >=0){
          paramName = "";
          value = "";
        }
      }else{
        paramName += (char)data;
      }
    }
    if(paramName == "tempo_integracao"){
      setTint(value.toInt());
    }
    if(paramName == "numero_medias"){
      setNumMedias(value.toInt());
    }
    if(paramName == "incremento_tint"){
      _incremento = value.toInt();
    }
    if(paramName == "multiplicador"){
      _multiplicador = value.toInt();
    }
    if(paramName == "scale_min"){
      _scaleMin = value.toInt();
    }
    if(paramName == "scale_max"){
      _scaleMax = value.toInt();
    }
    if(paramName == "scale_a"){
      _scale[0] = value.toFloat();
    }
    if(paramName == "scale_b"){
      _scale[1] = value.toFloat();
    }
    if(paramName == "scale_c"){
      _scale[2] = value.toFloat();
    }
    if(paramName == "scale_d"){
      _scale[3] = value.toFloat();
    }
  }
  configFile.close();
}

void TSL1402R::setTint(unsigned int tInt){
  if(tInt < 100){
    tInt = 100;
  }
  _tInt = tInt;
}

unsigned int TSL1402R::getTint(){
  return _tInt;
}

void TSL1402R::setNumMedias(unsigned int numMedias){

  _numMedias = numMedias;
}

unsigned int TSL1402R::getNumMedias(){
  return _numMedias;
}

void TSL1402R::clearData(){
  for(int i = 0; i < 256; i++){
    _data[i] = 0;
  }
}

void TSL1402R::setLinhaDeBase(){
  for(int i = 0; i < 256; i++){
    _base[i] = getData(i);
  }
}

unsigned int TSL1402R::getIncremento(){
  return _incremento;
}

void TSL1402R::setIncremento(unsigned int inc){
  _incremento = inc;
}

void TSL1402R::setData(int i,unsigned int data){
  if(i >= _scaleMin && i <= _scaleMax){
    _dataTemp[i-_scaleMin] = data;
    _data[i] = data;
  }else{
    _data[i] = 0;
  }
}

unsigned int TSL1402R::getData(int i){
  return _dataTemp[i];
}

unsigned int TSL1402R::getBase(int i){
  return _base[i];
}

float TSL1402R::getScale(int i){
  return _scale[i];
}

unsigned int TSL1402R::getTransmissao(int i){
  double rs;
  rs = ((float)getData(i) / (float)getBase(i));
  return  rs * 100;
}

unsigned int TSL1402R::getAbsorbancia(int i){
  double rs;
  rs = log10((float)getBase(i) / (float)getData(i));
  return rs * 1000;
}

void TSL1402R::parseLog(String fileName,char tipoc){
  int tipo = 1;
  if(tipoc == 'R'){
    tipo = 1;
  }
  if(tipoc == 'T'){
    tipo = 2;
  }
  if(tipoc == 'A'){
    tipo = 3;
  }
  File logFile;
  int data;
  String value = "";
  int i = 0;
  int inc = 0;
  if (!logFile.open(fileName, O_READ )) {
      Serial.println("MSG,Erro ao ler arquivo de log");
  }else{
    while((char)data != '\n' && data >= 0){
      data = logFile.read();// lendo a primeira linha
    }
    // read from the file until there's nothing else in it:
    while ((data = logFile.read()) >= 0) {
      while((char)data != '\n' && data >= 0){ // readline
        if(data != ','){
          value += (char)data;
        } else {
          if(inc == tipo){
            _dataLog[i] = value.toInt();
          }
          value = "";
          inc++;
        }
        data = logFile.read();// lendo a primeira linha
      }
      if(inc == tipo){
        _dataLog[i] = value.toInt();
      }
      value = "";
      inc = 0;
      i++;
    }
  }
}

unsigned int TSL1402R::getLogData(int i){
  return _dataLog[i];
}
