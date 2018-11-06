/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "Espectrapp.h"
#include "Controller.h"

Espectrapp::Espectrapp(){
  _controller = new Controller();
}

void Espectrapp::routeTo(String route){
  //_controller->setRoute(route);
  _route = route;
}

void Espectrapp::route(){
  if(_route != _controller->getRoute()){
    _route == _controller->getRoute();
  }
  if(_route == "graph"){
    //Serial.println("graph route");
    _controller->graphAction(_data,_numPontos);
  }
  if(_route == "start"){
    //Serial.println("start route");
    _controller->startAction(getScale(0),getScale(1),getScale(2),getScale(3));
    _tempoIntegracaoDefault = _tempoIntegracao;
  }
  if(_route == "menu"){
    //Serial.println("menu route");
    _controller->menuAction();
  }
  if(_route == "trans"){
    //Serial.println("trans route");
    _controller->transAction();
  }
  if(_route == "base"){
    //Serial.println("base route");
    _controller->baseAction();
  }
  if(_route == "real"){
    //Serial.println("real route");
    _controller->realAction();
  }
  if(_route == "absorb"){
    //Serial.println("absorb route");
    _controller->absorAction();
  }
  if(_route == "params"){
    //Serial.println("params route");
    _controller->paramsAction(_tempoIntegracao);
  }
  if(_route == "decrement"){
    //Serial.println("decrement route");
    if(_tempoIntegracao > 100)_tempoIntegracao -= _incremento;
    routeTo("params");
  }
  if(_route == "increment"){
    //Serial.println("increment route");
    if(_tempoIntegracao < 3000)_tempoIntegracao += _incremento;
    routeTo("params");
  }
  if(_route == "storage"){
    _controller->storageAction();
  }
  if(_route == "savesd"){
    _controller->savesdAction();
  }
  if(_route == "datalist"){
    _controller->dataListAction(_page,_numPaginas,_numLogs);
  }
  if(_route == "nextpage"){
    if(_page < _numPaginas){
      _page++;
    }
    _controller->dataListAction(_page,_numPaginas,_numLogs);
  }
  if(_route == "prevpage"){
    if(_page > 0){
      _page--;
    }
    _controller->dataListAction(_page,_numPaginas,_numLogs);
  }
  if(_route == "sendtint"){
    //Serial.println("send TIN route");
    Serial.print("#");
    Serial.print("TINT");
    Serial.print("#");
    Serial.print(_tempoIntegracao);
    Serial.println("#");
    _controller->menuAction();
  }

  if(_route == "resettint"){
    _tempoIntegracao = _tempoIntegracaoDefault;
    _controller->menuAction();
  }
}


void Espectrapp::loop(){
  route();
  routeTo(_controller->mapActions());
}

void Espectrapp::fakeData(){
  float freq = (6.28 + random(12,32));
  for(int i=0; i < _numPontos; i++){
    float val = 120*sin(i * freq/256.0);
    setData(120+(int)val,i);
  }
}

void Espectrapp::setData(int data,int i){
  if(data > 0 || data < 4096){
    _data[i] = data;
  }else{
    _data[i] = 0;
  }
}
void Espectrapp::dataChanged(){
  _controller->dataChanged();
}

void Espectrapp::setTempoIntegracao(int tInt){
  _tempoIntegracao = tInt;
}

void Espectrapp::setIncremento(int inc){
  _incremento = inc;
}

void Espectrapp::setNumMedias(int numMedias){
  _numMedias = numMedias;
}


void Espectrapp::setNumLog(int numLogs){
  _page = 0;
  _numLogs = numLogs;
  _controller->setNumLog(_numLogs);
  _numPaginas = ceil((float)numLogs / 4.0);
}

void Espectrapp::sendPhoton(String str) {
  Serial.print("#");
  Serial.print(str);
  Serial.println("#");
}

void Espectrapp::setScale(int i, int val){
  _scale[i] = val;
}

int Espectrapp::getScale(int i){
  return _scale[i];
}
