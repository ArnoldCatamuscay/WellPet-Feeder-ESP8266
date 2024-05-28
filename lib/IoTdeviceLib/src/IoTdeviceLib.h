#ifndef _IoTdeviceLib
#define _IoTdeviceLib
#include <SoftwareSerial.h>
#include <math.h>
#include <Servo.h>
#include <HX711.h>

Servo miServo;
HX711 celda;

// CANTIDAD DE ALIMENTO(Kg) POR COMIDA
float pesoMaximo = 100; //Valor por defecto, lo puede cambiar el usuario

//*------ SECCION DE LECTURA Y CONVERSION DIGITAL DE SENSORES ---------- 

//TODO Documentar
void CalibrarPeso();

//TODO Documentar
float LeerPeso();

//TODO Documentar
float LeerPromedioPeso(int numLecturas);

//*--------------- SECCION DE INTERACCIÓN CON ACTUADORES --------------- 

//TODO Documentar
void PosicionarServo(int grados);

//*--------------- SECCION DE INTERACCIÓN DE SENSORES-ACTUADORES ---------------

//TODO Documentar DispensarAlimento
void DispensarAlimento();

//*--------------- SECCION DE INTERACCIÓN M2H ---------------

/**
 * @brief Imprime el valor reportado en la salida serial
 * 
 * @param dato Datoa a imprimir
 * @param entidaddeInteres Valor semántico de la entidad medida
 * @param simboloMedida Simbolo de la medicion realizada
 */
void ImprimirValorSensor(float dato, String entidaddeInteres,String simboloMedida);

#endif