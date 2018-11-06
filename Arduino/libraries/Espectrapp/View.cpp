/*
  Espectrapp.cpp -
*/

#include "Arduino.h"
#include "View.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include "Color.h"

#define MINPRESSURE 2
#define MAXPRESSURE 1000

View::View(){
  _tft = new Adafruit_TFTLCD(A3, A2, A1, A0, A4);
  _tft->reset();
  delay(500);
  _tft->begin(0x9341);
  _tft->setRotation(3); //Use esta linha para o controlador 9341
  _tft->fillScreen(DARK);
  _ts = new TouchScreen(9, A2, A3, 8, 100);
}

void View::start(){
  Color c = Color(0,0,0);
  if(_loadingCounter == 0){
    _tft->fillRect(0,120,320,240,DARK);
  }
  _tft->setTextSize(4);
  _tft->setCursor(40,50);
  _tft->setTextColor(c.getColorFromHSV(map(0,0,11,0,767),255,255));
  _tft->print("E");
  _tft->setTextColor(c.getColorFromHSV(map(1,0,11,0,767),255,255));
  _tft->print("s");
  _tft->setTextColor(c.getColorFromHSV(map(2,0,11,0,767),255,255));
  _tft->print("p");
  _tft->setTextColor(c.getColorFromHSV(map(3,0,11,0,767),255,255));
  _tft->print("e");
  _tft->setTextColor(c.getColorFromHSV(map(4,0,11,0,767),255,255));
  _tft->print("c");
  _tft->setTextColor(c.getColorFromHSV(map(5,0,11,0,767),255,255));
  _tft->print("t");
  _tft->setTextColor(c.getColorFromHSV(map(6,0,11,0,767),255,255));
  _tft->print("r");
  _tft->setTextColor(c.getColorFromHSV(map(7,0,11,0,767),255,255));
  _tft->print("A");
  _tft->setTextColor(c.getColorFromHSV(map(8,0,11,0,767),255,255));
  _tft->print("p");
  _tft->setTextColor(c.getColorFromHSV(map(9,0,11,0,767),255,255));
  _tft->print("p");

  _tft->setTextSize(2);
  _tft->setCursor(80,150);
  _tft->setTextColor(c.getColorFromHSV(0,0,255));
  if(_loadingCounter > 4){
    _loadingCounter = 1;
    _tft->fillRect(0,120,320,120,DARK);
  }
  _tft->print("Loading");
  for(int i=0; i<_loadingCounter; i++){
    _tft->print(".");
  }
  _loadingCounter++;
}

void View::drawGraph(int * data,int size,int max){
  _numPontos = size;
  int pl = _ml + 1;
  _tft->fillRect(0,0,_ml,40,DARK);
  _tft->setTextColor(LIGHT);
  _tft->setTextSize(1);
  _tft->setCursor(12,3);
  _tft->println(max);
  for(int i = 1 ; i < _numPontos-2; i++){
    if(data[i] > _mb+2){
      _tft->drawFastVLine((2*i + 0)+pl, 1, _tft->height() - _mb - 4, DARK);
      if(touched()){
        break;
      }
      _tft->drawFastVLine((2*i - 1)+pl, 1, _tft->height() - _mb - 4, DARK);
      if(touched()){
        break;
      }
      int x0 = (2*i + 0) + pl;
      int y0 = _tft->height() - data[i];
      int x1 = (2*i - 1) + pl;
      int y1 = _tft->height() - data[i-1];
      _tft->drawLine(x0,y0,x1,y1,SUCCESS);
      if(touched()){
        break;
      }
    }
  }
}

void View::menu(){
  _tft->fillScreen(DARK);
  _tft->setTextColor(WARNING);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("MENU");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER);
  _tft->drawLine(0 , 120  , _tft->width() , 120   , DANGER);

  _tft->setCursor(10, 60);
  _tft->println("Espect.");
  _tft->setCursor(10, 80);
  _tft->println("Transm.");
  _tft->setCursor(10,140);
  _tft->println("Tempo");
  _tft->setCursor(10,160);
  _tft->println("Real");

  _tft->setTextSize(2);
  _tft->setCursor(115, 60);
  _tft->println("Espect.");
  _tft->setCursor(115, 80);
  _tft->println("Absor.");

  _tft->setCursor(115,140);
  _tft->println("Tempo");
  _tft->setCursor(115,160);
  _tft->println("Integr.");
  _tft->setCursor(220,60);
  _tft->println("Linha de");
  _tft->setCursor(220,80);
  _tft->println("Base");

  _tft->setCursor(220,140);
  _tft->println("Salvar");
  _tft->setCursor(220,160);
  _tft->println("Dados");
  alerta();
}

void View::alerta(){
  if(_alerta == "Colete a linha de Base!"){
    _tft->setTextColor(DANGER);
  }else{
    _tft->setTextColor(SUCCESS);
  }
  _tft->fillRect(0,200,_tft->width(),_tft->height(),DARK);
  _tft->drawLine(0 , 200  , _tft->width() , 200   , DANGER);
  _tft->setCursor(10,210);
  _tft->println(_alerta);
}

void View::setAlerta(String alerta){
  _alerta = alerta;
}

void View::params(int tInt){
  _tft->fillScreen(DARK);
  _tft->setTextColor(DANGER);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("TEMPO DE INTEGRACAO");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER); // TODO: trocar por fastHline
  _tft->setCursor(10, 45);
  _tft->setTextSize(2);
  _tft->println("Tempo de Integracao ");
  _tft->setTextSize(1);
  _tft->setCursor(10,65);
  _tft->println("(Tempo de exposicao do sensor a LUZ)");

  _tft->setTextSize(4);
  _tft->setCursor(10,85);
  _tft->print(tInt);
  _tft->println(" ms");

  _tft->setTextColor(LIGHT);
  _tft->fillRect(10, 150, 50, 50, DARK);
  _tft->drawRect(10, 150, 50, 50, SUCCESS);
  _tft->setTextSize(4);
  _tft->setCursor(25,162);
  _tft->println("-");

  _tft->fillRect(90, 150, 50, 50, DARK);
  _tft->drawRect(90, 150, 50, 50, SUCCESS);
  _tft->setTextSize(4);
  _tft->setCursor(105,162);
  _tft->println("+");

  _tft->setTextColor(LIGHT);
  _tft->fillRect(180, 90, 130, 50, DARK);
  _tft->drawRect(180, 90, 130, 50, SUCCESS);
  _tft->setTextSize(3);
  _tft->setCursor(195,102);
  _tft->println("Voltar");

  _tft->setTextColor(LIGHT);
  _tft->fillRect(180, 150, 130, 50, DARK);
  _tft->drawRect(180, 150, 130, 50, SUCCESS);
  _tft->setTextSize(3);
  _tft->setCursor(195,162);
  _tft->println("Enviar");
}

void View::debug(String msg,int i){
  int TS = 1;
  _tft->setTextColor(DANGER);
  _tft->setTextSize(TS);
  _tft->setCursor(12, 12*TS*i);
  _tft->println(msg);
}

void View::drawBase(int s1,int s2,int s3,int s4, String label){
  _tft->fillScreen(DARK);
  _tft->drawRect(_ml, 0 ,_numPontos*2, _tft->height() - _mb -2, LIGHT);
  _tft->setTextColor(LIGHT);
  _tft->setTextSize(2);
  _tft->setCursor(-20 + _ml, _tft->height() - _mb+2);
  _tft->println(s4);
  _tft->setCursor(-20 + _ml + _numPontos*2/3, _tft->height() - _mb+2);
  _tft->println(s3);
  _tft->setCursor(-20 + _ml +  _numPontos*4/3, _tft->height() - _mb+2);
  _tft->println(s2);
  _tft->setCursor(-20 + _ml + _numPontos*2, _tft->height() - _mb+2);
  _tft->println(s1);
  int textSize = 2;
  _tft->setTextSize(textSize);
  _tft->setRotation(2);
  int textX = (int) _tft->width()/2 - label.length() * 5 * textSize / 2;
  int textY = (int) _ml - 8 * textSize - 5;
  _tft->setCursor(textX,textY);
  _tft->println(label);
  _tft->setRotation(3);

}

void View::storage(){
  _tft->fillScreen(DARK);
  _tft->setTextColor(DANGER);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("ARMAZENAMENTO");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER);
  _tft->setCursor(10, 45);
  _tft->setTextSize(2);
  _tft->println("Os dados armazenados no ");
  _tft->setCursor(10, 63);
  _tft->println("SD card podem ser lidos");
  _tft->setCursor(10, 81);
  _tft->println("diretamente no computador");
  _tft->drawLine(0 , 108  , _tft->width() , 108   , DANGER);

  _tft->setCursor(30, 150);
  _tft->println("Salvar");
  _tft->setCursor(30, 168);
  _tft->println("Dados");

  _tft->setCursor(175, 140);
  _tft->println("Ver");
  _tft->setCursor(175, 158);
  _tft->println("Dados");
  _tft->setCursor(175, 176);
  _tft->println("Salvos");
}

void View::listData(int page,int numPages,int numLogs){
  _tft->fillScreen(DARK);
  _tft->setTextColor(DANGER);
  _tft->setTextSize(2);
  _tft->setCursor(10, 10);
  _tft->println("LISTANDO DADOS");
  _tft->drawLine(0 , 35  , _tft->width() , 35   , DANGER);
  _tft->setTextSize(3);
  _tft->setCursor(20, 45);
  _tft->println("Log"+String(numLogs));
  _tft->setCursor(20, 90);
  _tft->println("Log"+String(numLogs-1));
  _tft->setCursor(20, 135);
  _tft->println("Log"+String(numLogs-2));
}

int View::height(){
  return _tft->height();
}

int View::width(){
  return _tft->width();
}

bool View::touched(){
  TSPoint _p = _ts->getPoint();
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  bool touched = false;
  if(_p.z > MINPRESSURE && _p.z < MAXPRESSURE){
    touched = true;
  }
  return touched;
}
