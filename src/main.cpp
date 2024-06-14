#include "Arduino.h"
#include "HX711.h"    // incluye libreria HX711
#include <ThingSpeak.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Servo.h"

#define DT D6     // DT de HX711 a pin digital 2
#define SCK D7      // SCK de HX711 a pin digital 3
#define SERVOPIN D3

#define MAX_STRINGS 10
String strings[MAX_STRINGS];
String horaactual;

HX711 celda;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void ConectarRed(char ssid[], char password[]);

void ConectarRed(char ssid[], char password[])
{
  Serial.println("Conectandose a la WIFI!");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi conectada");
  Serial.println(WiFi.localIP());
}

//* MQTT
static unsigned long channelID = 2575111;
static char* writeAPIKey = (char*)"979JJW97OBJG9ZY9"; 
const char* mqttAddress = "mqtt3.thingspeak.com";
static int port = 1883;
static char* clientId = (char*)"DCwSPAozOAo0PBIYKiEsEjs";
static char* username = (char*)"DCwSPAozOAo0PBIYKiEsEjs";
static char* password = (char*)"VDzLkls5S1K8FThTVdHTIHsv";
String PesoMax = "100";
float peso = 0;
Servo myservo;
unsigned long lastWeightReadTime = 0;
const unsigned long weightReadInterval = 2000;
const unsigned long postingInterval = 20L * 1000L;
long lastUpdateTime = 0;   

void InicializarThingSpeak()
{
  //************ Conectar Cliente ThinkSpeak *******
  ThingSpeak.begin( espClient );
  //******** Fin Conectar Cliente ThingSpeak *******
}

// Contador de strings almacenados
int numStrings = 0;

// Función para verificar si un string está presente en el array
bool stringPresente(String str) {
  for (int i = 0; i < numStrings; i++) {
    if (strings[i] == str) {
      return true;
    }
  }
  return false;
}

// Función para agregar un string al array si no está presente
bool agregarString(String str) {
  if (!stringPresente(str)) {
    if (numStrings < MAX_STRINGS) {
      strings[numStrings] = str;
      numStrings++;
      return true;
    } else {
      Serial.println("Error: El array está lleno.");
      return false;
    }
  } else {
    Serial.println("Error: String repetido no permitido.");
    return false;
  }
}

// Función para eliminar un string del array si está presente
bool eliminarString(String str) {
  for (int i = 0; i < numStrings; i++) {
    if (strings[i] == str) {
      for (int j = i; j < numStrings - 1; j++) {
        strings[j] = strings[j + 1];
      }
      numStrings--; 
      return true;
    }
  }
  Serial.println("Error: String no encontrado.");
  return false;
}

void imprimirStrings() {
  Serial.println("Contenido del array:");
  for (int i = 0; i < numStrings; i++) {
    Serial.println(strings[i]);
  }
}

int EnviarPeso(float data ){
  int  writeSuccess = ThingSpeak.writeField( channelID, 1, data, "979JJW97OBJG9ZY9" ); // Write the data to the channel
  if ( writeSuccess ){
    Serial.println( String(data) + " Enviando a ThingSpeak." );
  }
    
  return writeSuccess;
}

void LecturaPeso(){
  String lectura = ThingSpeak.readStringField(2575111,1,"CT2DV6AUJSN2K65A");
}

void LecturaPesoMax(){
  String lectura = ThingSpeak.readStringField(2575111,2,"CT2DV6AUJSN2K65A");
  if(lectura != PesoMax){
    PesoMax = lectura;
    Serial.println( "cambio el Peso Max: " + PesoMax );
  }
}

void agregarHorario(){
  String horario = ThingSpeak.readStringField(2575111,5,"CT2DV6AUJSN2K65A");
  Serial.println( "horario: " + horario );
  if(agregarString(horario)){
    Serial.println("horario " + horario + " agregado.");
  }else{
    Serial.println("horario " + horario + " no agregado.");
  }
}

void eliminarHorario(){
  String horario = ThingSpeak.readStringField(2575111,6,"CT2DV6AUJSN2K65A");
  if(eliminarString(horario)){
    Serial.println("horario " + horario + " eliminado.");
  }
}

void Dispensar(){
  //* Obtener la hora actual
  timeClient.update();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  horaactual = String(currentHour) + ":" + String(currentMinute); 
  Serial.println( "hora actual > " + horaactual );
  
  if(stringPresente(horaactual)){
    while(peso < PesoMax.toFloat()) {
      //* Leer sensor de peso
      celda.power_up();
      peso = celda.get_units(10);
      Serial.print("Valor (gramos): ");    
      Serial.println(peso);
      celda.power_down();                
      for(int angle = 0; angle < 180; angle += 1)
      {                          
        myservo.write(angle);
        delay(15);                       
      } 
      delay(1000);
      
      for(int angle = 180; angle>=0; angle-=15)
      {                                
        myservo.write(angle);
        delay(5);                       
      } 
    }
    
  }
}

void setup() {
  Serial.begin(115200);   // inicializa monitor serie a 9600 baudios
  myservo.attach(SERVOPIN);
  myservo.write(0);
  celda.begin(DT, SCK);  

  celda.set_scale(734.f);
  celda.tare();     
  ConectarRed("ssid", "pasword");
  InicializarThingSpeak();
  timeClient.begin();
  timeClient.setTimeOffset(-18000);
}

void loop() {
  
  if (millis() - lastWeightReadTime >= weightReadInterval) {
    lastWeightReadTime = millis();

    Dispensar();
  }

  if (millis() - lastUpdateTime >=  postingInterval) {
    lastUpdateTime = millis();
    EnviarPeso(peso);
    LecturaPeso();
    LecturaPesoMax();
    agregarHorario();
    imprimirStrings();
  }
}