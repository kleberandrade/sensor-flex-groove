#include <SPI.h>  
#include "RF24.h"

#define FINGERS_NUMBER 5 //numero de dedos
#define MEAN_NUMBER 100  //media numeros 

const int FINGERS[] = { A0, A1, A2, A3, A4 };           // Pinos de entradas para os dedos

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};

typedef struct 
{
  int id=0;
  int finger1;
  int finger2;
  int finger3;
  int finger4;
  int finger5;
} Package;

Package data;

int currentIndexTime = 0;                               // Tempo atual

int minFingersValue[FINGERS_NUMBER];                    // Minimos valores dos pinos
int maxFingersValue[FINGERS_NUMBER];                    // Maximos valores dos pinos
int fingerValues[FINGERS_NUMBER][MEAN_NUMBER];          // Matriz dos valores coletados

void clearMatrix(){
  for (int i = 0; i < FINGERS_NUMBER; i++){
    for (int j = 0; j < MEAN_NUMBER; j++){
     fingerValues[i][j] = 0; 
    }
  }
}

void clearMinMax(){
   for (int i = 0; i < FINGERS_NUMBER; i++){
    minFingersValue[i] = 1023;
    maxFingersValue[i] = 0;
  }
}

void updateHand(){
  for (int i = 0; i < FINGERS_NUMBER; i++){
    int value = analogRead(FINGERS[i]);
    minFingersValue[i] = min(minFingersValue[i], value);
    maxFingersValue[i] = max(maxFingersValue[i], value);

    fingerValues[i][currentIndexTime] = value;
  }

  currentIndexTime = ++currentIndexTime % MEAN_NUMBER;
}

int mean(int index){
  int sum = 0;
  for (int i = 0; i < MEAN_NUMBER; i++){
    sum += fingerValues[index][i];
  }
  return sum / MEAN_NUMBER;
}


void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
  //pinMode(A2, INPUT);
  //pinMode(A3, INPUT);
  //pinMode(A4, INPUT);
  
  myRadio.begin();  //configurando radio
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);

  clearMatrix();
  clearMinMax();
}

void debugData(){ //exibindo informações
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.print(data.finger1);
    Serial.print(" | ");
    Serial.print(data.finger2);
    Serial.print(" | ");
    Serial.print(data.finger3);
    Serial.print(" | ");
    Serial.print(data.finger4);
    Serial.print(" | ");
    Serial.println(data.finger5);
}

void updatePackage(){ //atualizando dados
    data.id += 1;
    data.finger1 = map(mean(0), minFingersValue[0], maxFingersValue[0], 10, 170);
    //data.finger2 = map(mean(1), minFingersValue[1], maxFingersValue[1], 10, 170);
    //data.finger3 = map(mean(2), minFingersValue[2], maxFingersValue[2], 10, 170);
    //data.finger4 = map(mean(3), minFingersValue[3], maxFingersValue[3], 10, 170);
    //data.finger5 = map(mean(4), minFingersValue[4], maxFingersValue[4], 10, 170);
}

void loop()
{
    updateHand();
    updatePackage();
    myRadio.write(&data, sizeof(data)); 
    delay(3);
}
