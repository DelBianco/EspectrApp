  #include <Espectrapp.h>

Espectrapp * app;
bool connectedToPhoton = false;

void setup(){
  Serial.begin(115200,SERIAL_8E1);
  while(!Serial){
    // Wait serial port initialization
  }
  app = new Espectrapp();
}

void loop(){
  
  app->loop();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent(){
  connectedToPhoton = true;
  char inChar;
  while(Serial.available()){
    inChar = (char)Serial.read();
    if(inChar != '#'){
      inChar = (char)Serial.read();
    }else{
      inChar = (char)Serial.read();
      if(inChar == 'D' || inChar == 'M' || inChar == 'G' || inChar == 'S'){
        if(inChar == 'M'){
          app->routeTo("menu");
        }
        if(inChar == 'G'){
          app->routeTo("graph");
        }
        if(inChar == 'S'){
          app->routeTo("params");
        }
        Serial.read();Serial.read(); // Trailling
      }
      if(inChar == 'D'){ // dados vindo espera
        while(!Serial.available()){} 
        Serial.read(); // # Trailling
        for(int i = 0; i < 145; i++){
          while (Serial.available() < 5) {} // Espera ter certeza de que tem um dado do tipo 9999\n no buffer
          int data = Serial.parseInt();
          app->setData(data,i);
          Serial.read(); // \n
        }
        app->dataChanged();
      } 
      if(inChar == 'S'){ // Starting
        app->routeTo("menu");
        while(Serial.available() < 18){}
        inChar = (char)Serial.read(); // P
        inChar = (char)Serial.read(); // P
        if(inChar == 'P'){
          Serial.read(); // #
          int i = Serial.parseInt();
          app->setTempoIntegracao(i);
          i = Serial.parseInt();
          app->setNumMedias(i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setIncremento(i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setNumLog(i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setScale(0,i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setScale(1,i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setScale(2,i);
          Serial.read(); // #
          i = Serial.parseInt();
          app->setScale(3,i);
          Serial.read(); // #
          Serial.read(); // #
          Serial.read(); // \n
        }
      }
    }
  }
}
