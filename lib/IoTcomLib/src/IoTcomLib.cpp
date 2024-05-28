#include <IoTcomLib.h>

//------------------- FUNCIONES PARA CONECTAR A LA RED ---------------------

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

//------------------- FUNCIONES PARA CONECTAR A THINGSPEAK ---------------------
void InicializarThingSpeak()
{
  //************ Conectar Cliente ThinkSpeak *******
  ThingSpeak.begin( espClient );
  //******** Fin Conectar Cliente ThingSpeak *******
}

// Use this function if you want to write a single field
int EnviarThingSpeakDatos(unsigned int TSField, float data ){
  int  writeSuccess = ThingSpeak.writeField( channelID, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ){
    //lcd.setCursor(0, 1);
    //lcd.print("Send ThinkSpeak");
    Serial.println( String(data) + " Enviando a ThingSpeak." );
  }
  return writeSuccess;
}

//use this function if you want multiple fields simultaneously
int EnviarThingSpeakVariosDatos( 
  unsigned int TSField1, float field1Data,
  unsigned int TSField2, long field2Data,
  unsigned int TSField3, long field3Data ,
  unsigned int TSField4, long field4Data )
{
  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );
  ThingSpeak.setField( TSField3, field3Data );
  ThingSpeak.setField( TSField4, field4Data );

  Serial.print("Enviando Datos ThingSpeak...");
  int printSuccess = ThingSpeak.writeFields( channelID, writeAPIKey );
  Serial.println("Success: "); Serial.print(printSuccess);
  return printSuccess;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId, username, password)) {
      Serial.println("connected");
      //https://blogs.mathworks.com/iot/2021/07/21/thingspeak-mqtt-update-access-control-and-iot-device-management/
      client.subscribe("channels/2543925/subscribe"); //? suscribirse a todo el feed
      // client.subscribe("channels/2543925/subscribe/fields/+"); //? suscribirse a todos los campos del canal 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
