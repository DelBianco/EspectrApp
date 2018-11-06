/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "Map.h"
#include <TouchScreen.h>

#define MINPRESSURE 2
#define MAXPRESSURE 1000

Map::Map(int w, int h){
  _ts = new TouchScreen(9, A2, A3, 8, 100);
  _width = w;
  _height = h;
}

void Map::startLoggin(){
  _loggin = true;
}

String Map::mapRoute(String route){
  if(_clicked){
    String newRoute = "start";
    if(route == "start"){
      newRoute = start();
    }
    if(route == "menu"){
      newRoute = menu();
    }
    if(route == "params"){
      newRoute = params();
    }
    if(route == "graph"){
      newRoute = graph();
    }
    if(route == "storage"){
      newRoute = storage();
    }
    if(route == "datalist"){
      newRoute = listData();
    }
    return newRoute;
  }
  return route;
}

String Map::start(){
  return "menu";
}

String Map::menu(){
  if( _y <= 3){ // primeira linha
    if(_x <= 3){
      return "trans";
    }
    if(_x > 3 && _x <=6){// Espectro de Absorcao
      return "absorb";
    }
    if(_x > 6){
      return "base";// Coletar linha de base
    }
  }
  if(_y  > 3){ // Segunda linha
    if(_x <= 3){
      return "real";
    }
    if(_x > 3 && _x <=6){
      return "params"; // Espectro de transmissao
    }
    if(_x > 6){
      return "savesd";
      //return "storage"; // Armazenamento
      //_comando = 'O';
    }
  }
  return "menu";
}

String Map::params(){
  if(_y > 4){
    if(_x <= 2){
      return "decrement";
    }
    if(_x > 2 && _x < 6){
      return "increment";
    }
  }
  if(_x >= 7){
    if(_y > 5){
      return "sendtint";
    }else{
      return "resettint";
    }
  }
  return "params";
}

String Map::graph(){
  if(_loggin){
    _count++;
    if(_count == 1) return "log_tran";
    if(_count == 2) return "log_abs";
    if(_count > 2){
      _count = 0;
      _loggin = false;
    }
  }
  return "menu";
}

String Map::storage(){
  if(_y > 4){
    if(_x < 5){
      return "savesd";
    }
    if(_x > 5){
      return "datalist";
    }
  }
  return "menu";
}

String Map::listData(){
  return "dataview";
}

void Map::checkTouch(){
  int timming = 64;
  if(_clicked){
    _clicked = false;
  }
  if(_p.z > MINPRESSURE && _p.z < MAXPRESSURE){
    //Serial.print("x: "); Serial.print(_p.x); Serial.print(" y: "); Serial.println(_p.y);
    if(_unClickBuffer > 0){
      _clickBuffer++; // posso fazer uma media da posicao do dedo para melhorar atualmente pega o ultimo
      _x = map(_p.x,900,280, 0, 10);
      _y = map(_p.y,300,900, 1, 8);
      //Serial.print("x: "); Serial.print(_p.x); Serial.print(" y: "); Serial.println(_y);
    }
  }
  if(_unClickBuffer >= timming){
    if(_clickBuffer > 0){
      _clickBuffer = 0;
      if(!_clicked){
        if((_p.x < 900 && _p.x > 280) && (_p.y < 900 && _p.y > 280)){
          _clicked = true;
          //Serial.print("Clicked - ");
          //Serial.print("x: "); Serial.print(_x); Serial.print(" y: "); Serial.println(_y);
        }
      }
      _unClickBuffer = -timming;
    }else{
      _unClickBuffer = 0;
    }
    _clickBuffer = 0;
  }
  _unClickBuffer++;
}

void Map::getTouchScreen(){
  _p = _ts->getPoint();
  //Serial.print("x: "); Serial.print(_p.x); Serial.print(" y: "); Serial.println(_p.y);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  checkTouch();
}
