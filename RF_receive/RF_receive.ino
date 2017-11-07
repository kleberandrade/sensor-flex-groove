#include <Servo.h> 
#include <SPI.h>  
#include "RF24.h" 

Servo fingerServo1; 
//Servo fingerServo2;  //declarando servos
//Servo fingerServo3; 
//Servo fingerServo4; 
//Servo fingerServo5; 

RF24 myRadio (7, 8); 

typedef struct  //tipo de dado para mão, cotendo identificador(id) e angulos dos dedos
{
  int id=0;
  int finger1;
  int finger2;
  int finger3; //pacote
  int finger4;
  int finger5;
} Package;

byte addresses[][6] = {"0"};  //endereço do byte

Package data; //dat acessa dedos e identificador

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  // Configurando as portas do servos
  fingerServo1.attach(2);  
  //fingerServo2.attach(3);  
  //fingerServo3.attach(4);  
  //fingerServo4.attach(5);  
  //fingerServo5.attach(6);  

  myRadio.begin();  //configurando radio
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  delay(1000);
}

void debugData(){ //exibicação
    //Serial.print("\nPackage:");
    //Serial.print(data.id);
    //Serial.print("\n");
    Serial.println(data.finger1);
    //Serial.print("\t");
    //Serial.print(data.finger2);
    //Serial.print("\t");
    //Serial.print(data.finger3);
    //Serial.print("\t");
    //Serial.print(data.finger4);
    //Serial.print("\t");
    //Serial.println(data.finger5);
}



void loop()  
{
  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
        myRadio.read( &data, sizeof(data) ); //ler informação do radio, quuantidade de bytes
    }

    fingerServo1.write(data.finger1);
    //fingerServo2.write(data.finger2);
    //fingerServo3.write(data.finger3);
    //fingerServo4.write(data.finger4);
    //fingerServo5.write(data.finger5);
    
    debugData();
  }
}
