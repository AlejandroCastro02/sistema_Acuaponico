/*
===================================================================================
Codigo para simulador SimulIDE, cargar el .hex en Arduino Nano
Pines entrada:
2 - DS18B20
3 - DHT11


Pines salida:
6 - Ventilador 1
7 - Ventilador 2
9 - Calentador

A4 - SDA_I2C (Pantalla LCD)
A6 - SCL_I2C (Pantalla LCD)
===================================================================================
*/
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <LiquidCrystal_I2C.h> 
//Se sustituye la libreria LiquidCrystal_I2C por LiquidCrystal_AIP31068_I2C (tema de compatibilidad en SimulIDE)
#include <LiquidCrystal_AIP31068_I2C.h>
#include <DHT.h>

#define ONE_WIRE_BUS 2 //Pin del sensor DS18B20
#define DHTTYPE DHT11 //Tipo de sensor utilizado (DHT11)
#define DHTPIN 3 //Pin del sensor DHT11
DHT dht(DHTPIN, DHTTYPE); //Define el pin y tipo de sensor

//Crea una conexión para comunicarse con otros dispositivos OneWire.
OneWire oneWire(ONE_WIRE_BUS);  
 
//Pasa la referencia de la libreria OneWire a la libreria DallasTemperatue
DallasTemperature sensors(&oneWire);

// Dirección I2C del LCD (generalmente 0x27 o 0x3F)
//LiquidCrystal_I2C lcd(0x27, 16, 2);

// Direcciones LCD I2C = 0x3E , 20x4 
LiquidCrystal_AIP31068_I2C lcd( 0x3E, 20, 4 );
 
void setup(void)
{
  sensors.begin();  // Inicializa las librerias
  Serial.begin(9600); 

  pinMode(6 , OUTPUT); //Se define la salida del ventilador 1
  pinMode(7 , OUTPUT); //Se define la salida del ventilador 2
  pinMode(9 , OUTPUT); //Se define la salida del calentador
  
   lcd.init();
  lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("DS18B20 "); //Mostramos en el LCD este mensaje
   lcd.setCursor(0,1);
  lcd.print("DHT11 "); //Mostramos en el LCD este mensaje
    dht.begin();

     lcd.setCursor(14,0);
  lcd.print(" C"); //Mostramos en el LCD este mensaje
     lcd.setCursor(14,1);
  lcd.print(" C"); //Mostramos en el LCD este mensaje
}
 
void loop(void)
{ 

  sensors.requestTemperatures(); //Manda comando para obtener las temperaturas
 
  lcd.display();
  lcd.setCursor(9, 0);
        
  lcd.print(sensors.getTempCByIndex(0)); //Imprime temp. del DS18B20 en Celsius


 float temperaturas2 = dht.readTemperature();
 lcd.setCursor(9, 1);
lcd.print(temperaturas2);//Imprime temp. del DHT11 en Celsius

if(sensors.getTempCByIndex(0)>30){
  //Se detecta temp. mayor a 30 grados, sistema se enfria
  lcd.setCursor(0, 1);
  lcd.print("Enfriando       ");
  digitalWrite(6 , HIGH);
  digitalWrite(7 , HIGH);
  } else if ((sensors.getTempCByIndex(0)<=30)&&(sensors.getTempCByIndex(0)>28)){
  digitalWrite(6 , LOW);
  digitalWrite(7 , LOW);
     lcd.setCursor(0,1);
  lcd.print("DHT11    ");
       lcd.setCursor(14,1);
  lcd.print(" C");
  delay(1000);
    }

if(sensors.getTempCByIndex(0)<27){
  //Se detecta temp. menor a 27 grados, sistema se calienta
   lcd.setCursor(0, 1);
    lcd.print("Calentando       ");
  digitalWrite(9 , HIGH);
  } else if ((sensors.getTempCByIndex(0)>=27)&&(sensors.getTempCByIndex(0)<28)){
  digitalWrite(9 , LOW);
     lcd.setCursor(0,1);
  lcd.print("DHT11    ");
       lcd.setCursor(14,1);
  lcd.print(" C");
  delay(1000);
    }
    

  delay(500);

  
  
}
