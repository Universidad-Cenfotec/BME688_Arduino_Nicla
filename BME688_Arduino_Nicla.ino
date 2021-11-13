/*
 Escrito por Tomás de Camino Beck, Ph.D.
 Sistemas Inteligentes
 Universidad Cenfotec

 Código para Arduino Nicla Sense Me
*/

#include "Arduino.h"
#include "Arduino_BHY2.h"
#include "Nicla_System.h"

Sensor temp(SENSOR_ID_TEMP);
Sensor gas(SENSOR_ID_GAS);
Sensor pressure(SENSOR_ID_BARO);
Sensor humidity(SENSOR_ID_HUM);

float meanTemp = 0;
float meanHumidity = 0;
float meanGas = 0;

const int samples = 200;
const int maxGas = 23000;
const int minGas = 5000;

void setup()
{
  //Abre puerto serial
  Serial.begin(115200);
  while (!Serial);

  //Inicia Sensores
  BHY2.begin();
  temp.begin();
  gas.begin();
  humidity.begin();

  //para utilizar el led del Nicla
  nicla::begin();
  nicla::leds.begin();

}

void loop()
{
  static auto printTime = millis();

  // Update function should be continuously polled
  BHY2.update();

  //calcula un priomedio de varias mediciones
  meanTemp = 0;
  meanHumidity = 0;
  meanGas = 0;
  for (int i = 0; i < samples; i++) {
    meanTemp += temp.value();
    meanHumidity += humidity.value();
    meanGas += gas.value();
    delay(20);
  }

  meanTemp = meanTemp / samples;
  meanHumidity = meanHumidity / samples;
  meanGas = meanGas / samples;

  // calcula porcerntaje a partir de valor de resistencia de Gases
    int pGas = round((1.0/(maxGas - minGas)*meanGas - (minGas*(1.0/(maxGas -minGas))))*100);
 
  //indicador Led, rojo indica que se sobrepasa capacidad
  if(pGas>65){
    nicla::leds.setColor(green); 
  }else{
    nicla::leds.setColor(red);  
  }
  
  
  Serial.println("*****************************");
  Serial.println(String("Temperatura: ") + String(meanTemp, 1)+String(" Celsius"));
  Serial.println(String("Resitencia Gas (VOC): ") + String(meanGas, 0)+String(" ohm"));
  Serial.println(String("Humedad: ") + String(meanHumidity, 1)+String("%"));
  Serial.println("_______________________");
  Serial.print("Seguridad Ambiental: ");
  Serial.print(pGas);
  Serial.println("%");
  Serial.println("_______________________");
  Serial.println(" ");
  Serial.println(" ");

}
