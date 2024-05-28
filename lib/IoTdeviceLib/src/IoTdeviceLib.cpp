#include <IoTdeviceLib.h>

//*------ SECCION DE LECTURA Y CONVERSION DIGITAL DE SENSORES ---------- 
void CalibrarPeso() 
{
    if (celda.is_ready()) 
    {
        celda.set_scale();    
        Serial.println("Tara... retire cualquier peso de la balanza.");
        delay(5000);
        celda.tare();
        Serial.println("Tara realizada...");
        Serial.print("Coloque un peso conocido en la balanza...");
        delay(5000);
        long lectura = celda.get_units(10);
        Serial.print("Resultado: ");
        Serial.println(lectura);
    }
    else 
    {
        Serial.println("No se encontro el modulo HX711.");
    }
    delay(1000);
    //El factor de calibracion sera: (lectura)/(peso conocido)
}

float LeerPeso() 
{
    return celda.get_units();
}

float LeerPromedioPeso(int numLecturas) 
{
    return celda.get_units(numLecturas);
}

//*--------------- SECCION DE INTERACCIÓN CON ACTUADORES --------------- 

void PosicionarServo(int grados) 
{
    miServo.write(grados);
}

//*--------------- SECCION DE INTERACCIÓN DE SENSORES-ACTUADORES --------------- 

void DispensarAlimento() 
{
    if(celda.get_units() < pesoMaximo) //TODO Si el peso es menor que el indicado dispensar
    { 
        PosicionarServo(90);
        delay(1000); // Wait for 1000 millisecond(s)
        PosicionarServo(0);
    }
}

//*--------------- SECCION DE INTERACCIÓN M2H --------------- 
void ImprimirValorSensor(float dato, String entidaddeInteres,String simboloMedida)
{
    //Imprimir el dato reportado
    Serial.print(entidaddeInteres);
    Serial.print(": ");
    Serial.print(dato);
    Serial.print(" ");
    Serial.println(simboloMedida);
}
