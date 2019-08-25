#include <Servo.h>

Servo dedos[5];

int dedosL[4]={A0,A1,A2,A3};
int boton=11; 
int switchP=4;
int val = 0;  // variable to store the value read

int r_menor[4]={1400,1400,1400,1400};
int r_mayor[4]={0,0,0,0};
#include "SingleEMAFilterLib.h"
SingleEMAFilter<int> singleEMAFilter(0.75);
SingleEMAFilter<int> singleEMAFilter1(0.75);
SingleEMAFilter<int> singleEMAFilter2(0.75);
SingleEMAFilter<int> singleEMAFilter3(0.75);

void cerrar()
{
for(int i=0;i<5;i++)
{
  dedos[i].write(180);
}
}
void prueba(int d)
{
  dedos[d].write(0);
  delay(2000);
  dedos[d].write(180);
  delay(2000);
}
void abrir()
{
  for(int i=0;i<5;i++)
{
  dedos[i].write(10);
}
}

void paz()
{
  for(int i=0;i<5;i++)
{
  if(i!=1 && i!=2)
  dedos[i].write(180);
  else
  dedos[i].write(10);
}
}

void flex()
{
  cerrar();
  for(int i=0;i<5;i++)
  {
      dedos[i].write(10);
      delay(650);
  }
}
int readDedos(int i){
    int val=0;
    switch(i){
      case 0:
        delay(1);
        singleEMAFilter.AddValue(analogRead(dedosL[i]));
        delay(1);
        val=singleEMAFilter.AddValue(analogRead(dedosL[i]));
      break;
      case 1:
        delay(1);
        singleEMAFilter1.AddValue(analogRead(dedosL[i]));
        delay(1);
        val=singleEMAFilter1.AddValue(analogRead(dedosL[i]));
      break;
      case 2:
        delay(1);
        singleEMAFilter2.AddValue(analogRead(dedosL[i]));
        delay(1);
        val=singleEMAFilter2.AddValue(analogRead(dedosL[i]));
      break;
      case 3:
        delay(1);
        singleEMAFilter3.AddValue(analogRead(dedosL[i]));
        delay(1);
        val=singleEMAFilter3.AddValue(analogRead(dedosL[i]));
      break;
      }
      return val;
  }
void setup() {
Serial.begin(9600);
dedos[0].attach(7); //pulgar normal
dedos[1].attach(6);  //indice normal
dedos[2].attach(9); //maldicion vuelta continua
dedos[3].attach(10); // no parece que jale
dedos[4].attach(8); //meñique vuelata continua no parece que jale

pinMode(boton,OUTPUT);
unsigned long tiempo = millis();


while(digitalRead(boton)!=HIGH){

  Serial.println("Calibrando");
      for(int i=0;i<4;i++){
        Serial.print(i);
        Serial.print(": ");
        val = readDedos(i);
        Serial.println(val);

      
      if(r_menor[i]> val && val>200){
          r_menor[i]=val;
       } 
       if(r_mayor[i]< val && val<700){
          r_mayor[i]=val;
       } 
     }
   }

abrir();
delay(1000);

}


void loop() {
  
//prueba(3);

char a;
if(digitalRead(switchP)==HIGH)
{
if(Serial.available()>0)
{
 a=Serial.read();

  switch(a)
  {
    case 'A':
    Serial.println("A: Cerrar");
    cerrar();
    delay(1000);
    break;
    
    case 'B':
    Serial.println("B: Paz");
    paz();
    delay(1000);
    break;  

    default:
    Serial.println("0: Neutral/Abierta");
    abrir();
  }
}
}
else
{
  for(int i=0;i<4;i++){
    Serial.println("Flex Sensors");
      val = analogRead(dedosL[i]);  // read the input pin
      
      if(val>r_menor[i] && val <r_mayor[i])
      val=map(val,r_menor[i],r_mayor[i],0,180);
      
      dedos[i].write(val);
    }
    delay(100);
}
 
}
