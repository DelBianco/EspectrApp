/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "Controller.h"
#include "View.h"
#include "Map.h"

Controller::Controller(){
  _view = new View();
  _map = new Map(_view->width(),_view->height());
}

void Controller::startAction(int s1, int s2, int s3, int s4){
  if(_route != "start"){
    _route = "start";
    _view->start();
    delay(2000);
    sendPhoton("RESET");
    _s1 = s1;
    _s2 = s2;
    _s3 = s3;
    _s4 = s4;
  }
}

void Controller::transAction(){
  if(hasBase()){
    sendPhoton("TRANS");
    _route = "graph";
    _view->drawBase(_s1,_s2,_s3,_s4,"Transm.(%)");
    //Serial.println("transAction");
  }else{
    _view->setAlerta("Colete a linha de base!");
    _route = "menu";
    _view->menu();
  }
}

void Controller::absorAction(){
  if(hasBase()){
    sendPhoton("ABSOR");
    _route = "graph";
    _view->drawBase(_s1,_s2,_s3,_s4,"Absor.(mAbs)");
    //Serial.println("absorAction");
  }else{
    _view->setAlerta("Colete a linha de base!");
    _route = "menu";
    _view->menu();
  }
}

void Controller::baseAction(){
  sendPhoton("BASE");
  _base = true;
  _view->setAlerta("Linha de base coletada!");
  _route = "menu";
  _view->menu();
  //Serial.println("baseAction");
}

void Controller::realAction(){
  sendPhoton("REAL");
  _route = "graph";
  _view->drawBase(_s1,_s2,_s3,_s4,"Intensidade");
  //Serial.println("realAction");
}

void Controller::menuAction(){
  if(_route != "menu"){
    _route = "menu";
    _view->menu();
    //Serial.println("menuAction");
  }
}

void Controller::paramsAction(int tInt){
  if(_route != "params"){
    _route = "params";
    _view->params(tInt);
    //Serial.println("paramsAction");
  }else{
    if(tInt != _tInt){
      _tInt = tInt;
      _view->params(tInt);
    }
  }
}

void Controller::storageAction(){
  if(_route != "storage"){
    _route = "storage";
    _view->storage();
  }
}

void Controller::savesdAction(){
  _route = "menu";
  sendPhoton("SALVA_SD");
  _numLogs++;
  _view->setAlerta("Salvo no SD, Arq. Esp"+String(_numLogs));
  _view->menu();
}

void Controller::dataListAction(int page, int numPages,int numlogs){
  if(_route != "datalist"){
    _route = "datalist";
    _view->listData(page,numPages,numlogs);
  }
}

void Controller::dataViewAction(){
  _route = "graph";
  _view->drawBase(_s1,_s2,_s3,_s4, "");
}

void Controller::graphAction(int * data, int size){
  if(_route != "graph"){
    _route = "graph";
    _view->drawBase(_s1,_s2,_s3,_s4,"");
  }
  if(_dataChanged == true){
    calcMax(data,size);
    for(int i = 0 ; i< size; i++){
      _dataStash[i] = map(data[120-i],0,_max,24,_view->height());
    }
    _view->drawGraph(_dataStash,size,_max);
    _dataChanged = false;
  }
}

void Controller::dataChanged(){
  _dataChanged = true;
}

void Controller::calcMax(int * data, int size){
  _max = 0;
  _min = 4098;
  for(int i = 0 ; i< size; i++){
    if(data[i] > _max){
      _max = data[i];
    }
    if(data[i] < _min){
      _min = data[i];
    }
  }
}

void Controller::sendPhoton(String str) {
  Serial.print("#");
  Serial.print(str);
  Serial.println("#");
}

String Controller::getRoute(){
  return _route;
}

bool Controller::hasBase(){
  return _base;
}

void Controller::setNumLog(int numlogs){
  _numLogs = numlogs;
}

void Controller::setRoute(String route){
  _route = route;
}

String Controller::mapActions(){
  _map->getTouchScreen();
  return _map->mapRoute(_route);
}
