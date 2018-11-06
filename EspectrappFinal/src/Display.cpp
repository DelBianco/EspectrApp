/* TSL1402R.cpp */

#include "Arduino.h"
#include "Display.h"
#include "TSL1402R.h"
#include "SdFat.h"

Display::Display(unsigned int baudRate){
  _baudRate = baudRate;
}

void Display::start(){
  Serial1.begin(_baudRate,SERIAL_8E1);
  while (!Serial1) {
    // wait Serial1 port initialization
  }
  // Initialize SD
  if (!_sd.begin(_chipSelect)){
    setComando('E');
    _sd.initErrorHalt();
  }
}

char Display::getComando(){
  return _comando;
}

void Display::setComando(char comando){
  //Particle.publish("Comando",&_comando);
  _comando = comando;
  Serial1.print("#");
  Serial1.print(_comando);
  Serial1.println("#");
}

void Display::showMenu(){
  setComando('M');
}

void Display::sendData(int y){
  Serial1.printlnf("%04d",y);
}

void Display::sendParams(unsigned int tint, unsigned int numMedias, unsigned int incremento, unsigned int s1, unsigned int s2, unsigned int s3, unsigned int s4 ){
  setComando('S');
  Serial1.print('P');
  Serial1.print('#');
  Serial1.printlnf("%08u", tint);
  Serial1.print('#');
  Serial1.printlnf("%02u", numMedias);
  Serial1.println('#');
  Serial1.printlnf("%05u", incremento);
  Serial1.println('#');
  Serial1.printlnf("%05u", logNumber()-1);
  Serial1.println('#');
  Serial1.printlnf("%04u", s1);
  Serial1.println('#');
  Serial1.printlnf("%04u", s2);
  Serial1.println('#');
  Serial1.printlnf("%04u", s3);
  Serial1.println('#');
  Serial1.printlnf("%04u", s4);
  Serial1.println('#');
}

int Display::logNumber(){
  SdFile file;
  // open the file for write at end like the "Native SD library"
  int fileCount = 0;
  _sd.vwd()->rewind();
  while (file.openNext(_sd.vwd(), O_READ)){
    if (!file.isHidden()) {
      fileCount++;
    }
    file.close();
  }
  return fileCount;
}

void Display::logData(TSL1402R monocromador){
  SdFile file;
  // open the file for write at end like the "Native SD library"
  int fileCount = 0;
  _sd.vwd()->rewind();
  while (file.openNext(_sd.vwd(), O_READ)){
    if (!file.isHidden()) {
      fileCount++;
    }
    file.close();
  }
  File myFile;
  String name = "log";
  name += (String)fileCount;
  name += ".txt";
  if(!myFile.open(name, O_CREAT | O_WRITE )){
      Serial.println("MSG,Erro ao criar arquivo de log");
  }else{
    myFile.printlnf("L(nm),Real,Base,Transm,Absorb");
      for(int i = 0; i < 145; i++){
        int data = monocromador.getData(i);
        int transm = monocromador.getTransmissao(i);
        int absor = monocromador.getAbsorbancia(i);
        int base = monocromador.getBase(i);
        myFile.printlnf("%f,%d,%d,%d,%d",scale(monocromador,i),data,transm,absor,base);
      }
  }
  myFile.close();
}


void Display::getLogs(){
  SdFile file;
  char name[260] = "log123.txt";
  const std::size_t N = 18;
  // open the file for write at end like the "Native SD library"
  _sd.vwd()->rewind();
  while (file.openNext(_sd.vwd(), O_READ)){
    if (!file.isHidden()) {
      memset(name, 0, sizeof(name));
      file.getName(name, sizeof(name));
      String nome = String(name);
      if(nome.substring(0,3) == "log"){
        Serial.print("LOG,");
        Serial.println(nome);
      }
    }
    file.close();
  }
}


float Display::scale(TSL1402R monocromador, int i){
  return monocromador.getScale(3)*pow(i,3)+monocromador.getScale(2)*pow(i,2)+monocromador.getScale(1)*pow(i,1)+monocromador.getScale(0)*pow(i,0);
}
