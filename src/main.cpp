#include "Arduino.h"
#include <IoTdeviceLib.h>       //Librería con funciones de sensor - actuador
#include <IoTcomLib.h>          //Librería con funciones de comunicación del dispositivo
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

//* Servo Pin
const int servoPin = D7; //https://elosciloscopio.com/tutorial-servomotor-arduino-esp8266-y-esp32/

//* HX711 circuit wiring
const int DT = D6; //GPIO 12
const int SCK = D7; //GPIO 13
const float CALIBRATING_FACTOR = 734.f;

float peso;
float pesoPromedio;

int numMedicion = 0;                                //Establece el número consecutivo de observacion hecha
const unsigned long postingInterval = 25L * 1000L;  //? Establece cada cuanto se envia a ThingSpeak (25 s)
unsigned long lastConnectionTime = 0;               //Para controlar el tiempo de generar nueva medición
long lastUpdateTime = 0;                            //Momento de la última actualización

//* onMessageArrived
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //TODO Deserializar el JSON https://arduinojson.org/

  // Switch on the LED if an 0 was received as first character
  // if ((char)payload[0] == '0') {
  //   digitalWrite(bombillopin, LOW);   // Turn the LED on (Note that LOW is the voltage level
  //   // but actually the LED is on; this is because
  //   // it is acive low on the ESP-01)
  // } else {
  //   digitalWrite(bombillopin, HIGH);  // Turn the LED off by making the voltage HIGH
  // }

}

//metodo cliente para controlar los eventos R1 y R2
void setup()
{
  //*---------------- Abrir el puerto de lectura en el PC para mensajes -----------------
  Serial.begin(115200);

  //*---------------- Conectarse a red WiFi -----------------
  ConectarRed("ssid","password");
  //-----Comando para Conectarse y configurar desde el Celular--------
  // Creamos una instancia de la clase WiFiManager
  // WiFiManager wifiManager;

  // Descomentar para resetear configuración - Hacer el ejercicio con el celular
  // todas las veces.
  //wifiManager.resetSettings();

  // Creamos AP y portal para configurar desde el Celular
  // wifiManager.autoConnect("ESP8266Temp");
 
  // Serial.println("!Ya estás conectado¡");
  //----------- Fin de conexión ESP8266 -----------------------------

  //Establecer los modos de los puertos
  //*---------------- Servomotor -----------------
  miServo.attach(servoPin);
  // miServo.write(0);
  //*------------------- Hx711 ---------------------
  celda.begin(DT, SCK);
  celda.set_scale(CALIBRATING_FACTOR);
  celda.tare();
  //*-------------- Inicializar aqui thingspeak --------------
  InicializarThingSpeak(); //? Para escribir
  //*-------------- Inicializar servidor MQTT --------------
  // Configuración del cliente MQTT: https://pubsubclient.knolleary.net/api
  client.setServer(mqttAddress, port); //? Para leer
  client.setCallback(callback);
  // reconnect();
}

void loop()                    
{
  //* Conectarse al servidor MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Solamente actualiza si el tiempo de publicación es excedido
  /*if (millis() - lastUpdateTime >=  postingInterval) {
    lastUpdateTime = millis();

    //* Leer sensor de peso
    peso = LeerPeso();
    pesoPromedio = LeerPromedioPeso(5);

    //* Imprimir peso leído 
    Serial.print("=========== Medición No.: ");
    Serial.print(numMedicion++);
    Serial.println(" ============");
      
    ImprimirValorSensor(peso, "Peso en el plato", "Kg");
    ImprimirValorSensor(pesoPromedio, "Peso(promedio) en el plato", "Kg");

    Serial.println("========================================");

    DispensarAlimento();

    //* Enviar peso leído a ThinkSpeak  
    EnviarThingSpeakDatos(dataFieldOne, peso);
  }*/
}