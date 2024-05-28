#ifndef _IoTcomLib
#define _IoTcomLib


//------------------- FUNCIONES PARA CONECTAR A LA RED ---------------------
#include <ESP8266WiFi.h>

/**
 * @brief Permite conectar el ESP a la Red, dependiendo de los parámetros
 * 
 * @param ssid identificador de la red
 * @param password contraseña de red
 */
void ConectarRed(char ssid[], char password[]);


//------------------- FUNCIONES PARA CONECTAR A THINGSPEAK ---------------------
#include <ThingSpeak.h>
#include <PubSubClient.h>
WiFiClient espClient;//Cliente Wifi para ThingSpeak
PubSubClient client(espClient);
// Información del Canal y Campos de ThingSpeak
//static char thingSpeakAddress[] = "api.thingspeak.com";
static unsigned long channelID = 2543925;
static char* readAPIKey=(char*)"DEIY5N2F5XM8V0TE"; 
static char* writeAPIKey = (char*)"TNW9ENUFDCWAU1LW"; 
unsigned int dataFieldOne = 1;   // Calpo para escribir el estado de la Temperatura
unsigned int dataFieldTwo = 2;   // Campo para escribir el estado del Bombillo
unsigned int dataFieldThree = 3; // Campo para escribir el estado del ventilador
unsigned int dataFieldFour = 4;  // Campo para enviar el tiempo de medición

//* MQTT
const char mqttAddress[] = "mqtt3.thingspeak.com";
static int port = 1883;
static char* clientId = (char*)"HCkJPBgROgAaHjAjDQw5Iiw";
static char* username = (char*)"HCkJPBgROgAaHjAjDQw5Iiw";
static char* password = (char*)"R5d2MwbKx+YxL1RThy+aoL0R";

/**
 * @brief Inicializa el cliente de ThinkSpeak y almacena los datos de conexión
 * 
 * @param TSChannel Id del canal en ThingSpeak
 * @param readAPIKey API KEY de ThingSpeak para solo lectura
 * @param writeAPIKey API KEY de ThinkSpeak para escritura
 * @return void 
 */
void InicializarThingSpeak();

/**
 * @brief Permite enviar un dato perticular a ThingSpeak
 * 
 * @param TSField Identificador del Campo a asignar el Dato
 * @param data Dato a ser enviado a ThingSpeak
 * @return int 
 */
int EnviarThingSpeakDatos(unsigned int TSField, float data );

/**
 * @brief Permite enviar cuatro datos al tiempo. Se puede expandir para enviar mas datos
 * 
 * @param TSField1 Identificador del Campo 1
 * @param field1Data Datos del Campo 1
 * @param TSField2 Identificador del Campo 2
 * @param field2Data Datos del Campo 2
 * @param TSField3 Identificador del Campo 3
 * @param field3Data Datos del Campo 3
 * @param TSField4 Identificador del Campo 4
 * @param field4Data Datos del Campo 4
 * @return int 
 */
int EnviarThingSpeakVariosDatos( 
  unsigned int TSField1, float field1Data,
  unsigned int TSField2, long field2Data,
  unsigned int TSField3, long field3Data ,
  unsigned int TSField4, long field4Data 
);

void reconnect();

#endif
