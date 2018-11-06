SYSTEM_MODE(MANUAL);
#include "TSL1402R.h"
#include "Display.h"
#include "PLX_DAQ.h"

int buffer[8][256];
int maxBuffer = 8;
int bufferCount = 0;
int menu = 1;
bool click = false;
bool coleta = true;
bool logSent = true;
void btnPress(void);
int logid = 0;
int tipo = 3;

PLX_DAQ excelComm(4800,'K');
Display display(115200);
TSL1402R monocromador(5000);

void setup(){
  // Initialize two pins as digital output:
  pinMode(D2, OUTPUT);  // D2 envia CLK pulse to pin 2 of the TSL1402R
  pinMode(D4, OUTPUT);   // D4 Pino de SI 4 (analog output 1) of the TSL1402R
  pinMode(D6, INPUT_PULLUP); // pino de interrupcao do botao

  display.start();
  monocromador.start(); // inicializa o monocromador, elimina qualquer dado que esteja flutuando lá
  excelComm.start(2000);
  display.sendParams(
    monocromador.getTint(),
    monocromador.getNumMedias(),
    monocromador.getIncremento(),
    monocromador.getScale(144),
    monocromador.getScale(97),
    monocromador.getScale(57),
    monocromador.getScale(17)
  );
  attachInterrupt(D6, btnPress, FALLING);
}

void loop(){

  noInterrupts();
  if(coleta){
    monocromador.coletaDados();
    //monocromador.demoData();
  }

  interrupts();
  if(click){
    //display.sendParams(monocromador.getTint(),monocromador.getNumMedias(),monocromador.getIncremento());
    coleta = !coleta;
    click = false;
  }

  processaComandos();
  excelComm.leituraParametros();
}

void processaComandos(){
  //Particle.publish("COMANDO",(String)excelComm.getComando());
  if(excelComm.getComando() == 'A'){
    excelComm.sendMSG("Exibindo padrao de transmisao");
    display.setComando('D');
    Serial.println("LABEL,L(nm),Trans.");
    for(int i=0;i<145;i++){
      excelComm.sendData(display.scale(monocromador,i),monocromador.getTransmissao(i));
      display.sendData(monocromador.getTransmissao(i));
    }
    excelComm.resetRow();
  }

  if(excelComm.getComando() == 'B'){
    excelComm.sendMSG("Coletando linha de base");
    monocromador.setLinhaDeBase();
    excelComm.setComando('C');
  }

  if(excelComm.getComando() == 'C'){
    excelComm.sendMSG("Tempo Real");
    display.setComando('D');
    Serial.println("LABEL,L(nm),Real.");
    for(int i=0;i<145;i++){
      excelComm.sendData(display.scale(monocromador,i),monocromador.getData(i));
      display.sendData(monocromador.getData(i));
    }
    excelComm.resetRow();
  }

  if(excelComm.getComando() == 'D'){
    excelComm.sendMSG("Exibindo padrao de absorbancia");
    display.setComando('D');
    Serial.println("LABEL,L(nm),Absor.");
    for(int i=0;i<145;i++){
      excelComm.sendData(display.scale(monocromador,i),monocromador.getAbsorbancia(i));
      display.sendData(monocromador.getAbsorbancia(i));
    }
    excelComm.resetRow();
  }

  // Seta os parametros
  if(excelComm.getComando() == 'E'){
    excelComm.sendMSG("Setando Parametros");
    excelComm.leituraParametros();
    monocromador.setTint(excelComm.getParam(0));
    monocromador.setNumMedias(excelComm.getParam(1));
    display.sendParams(
      monocromador.getTint(),
      monocromador.getNumMedias(),
      monocromador.getIncremento(),
      monocromador.getScale(144),
      monocromador.getScale(97),
      monocromador.getScale(57),
      monocromador.getScale(17)
    );
    excelComm.recuperaComando();
  }

  if(excelComm.getComando() == 'F'){
    display.logData(monocromador);
    logSent = false;
    display.sendParams(
      monocromador.getTint(),
      monocromador.getNumMedias(),
      monocromador.getIncremento(),
      monocromador.getScale(144),
      monocromador.getScale(97),
      monocromador.getScale(57),
      monocromador.getScale(17)
    );
    excelComm.recuperaComando();
  }

  if(excelComm.getComando() == 'G'){
    display.sendParams(
      monocromador.getTint(),
      monocromador.getNumMedias(),
      monocromador.getIncremento(),
      monocromador.getScale(144),
      monocromador.getScale(97),
      monocromador.getScale(57),
      monocromador.getScale(17)
    );
    excelComm.setComando('C');
  }
  if(excelComm.getComando() == 'H'){
    display.getLogs();
    excelComm.sendMSG("Enviando logs");
    excelComm.recuperaComando();
  }

  if(excelComm.getComando() == 'L'){
    //Serial.println("OFFSET_COLUMN,1");
    excelComm.resetRow();
    String label;
    for(int i=0;i<145;i++){
      excelComm.sendData(display.scale(monocromador,i),0);
    }
    for(int i = 0; i < 50; i++){
      String logName = excelComm.getLog(i);
      label += ","+logName;
      if(logName.length() > 3){
        monocromador.parseLog(logName,excelComm.getSubComando());
        Serial.println("OFFSET_COLUMN,1");
        excelComm.resetRow();
        for(int i=0;i<145;i++){
          Serial.print("DATA,");
          Serial.println(monocromador.getLogData(i));
        }
      }
    }
    Serial.println("RESET_COLUMN");
    label = "LABEL,L(nm)"+label;
    Serial.println(label);
    excelComm.resetRow();
    excelComm.setComando('K');
  }
}

void btnPress(){
  while(digitalRead(D6) == 0){}
  click = true;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent1(){
  char inChar;
  while(Serial1.available()){
    inChar = (char)Serial1.read();
    if(inChar == '#'){
      inChar = (char)Serial1.read();
      if(inChar != '\n'){
        while (Serial1.available() < 6) {}
        String comando = "";
        while(inChar != '#'){
          comando += inChar;
          inChar = (char)Serial1.read();
        }
        if(comando == "RESET"){
          Serial.println("RESETANDO");
          System.reset();
        }
        if(comando == "SALVA_SD"){
          excelComm.setComando('F');
        }
        if(comando == "TINT"){
          unsigned int val = Serial1.parseInt();
          inChar = (char)Serial1.read(); // #
          monocromador.setTint(val);
          excelComm.setComando('G');
        }
        if(comando == "REAL"){
          excelComm.setComando('C');
        }
        if(comando == "TRANS"){
          excelComm.setComando('A');
        }
        if(comando == "ABSOR"){
          excelComm.setComando('D');
        }
        if(comando == "BASE"){
          excelComm.setComando('B');
        }
      }
    }
  }
}
