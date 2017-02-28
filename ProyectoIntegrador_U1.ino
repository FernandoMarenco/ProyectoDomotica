#include <DHT11.h>

//Variables potenciometro
int pinPotenciometro = 0;
int valorPot = 0; //calcular el dia y la noche 0: noche - 1023: dia

//Variables sensor PIR
int pinPIR = 4;
int valorPIR = 0;

//Variables alarma/bocina
int pinZumb = 10; //pin de la bocina
int pinBoton = 3; //pin del boton
int pinLedRojo = 11; //pin del led rojo
int pinLedVerde = 12; //pin del led verde
int salidaBoton = 0; //leer salida del boton
int activado = 0;

//Variables sensor temperatura
int pinDHT = 9;
DHT11 dht11(pinDHT);
int pinLedAzul = 13; //led de riego

void setup() {
  //Entrada potenciometro
  pinMode(pinPotenciometro, INPUT);

  //Entrada PIR
  pinMode(pinPIR, INPUT);

  //Entradas y salidas alarma
  pinMode(pinBoton, INPUT);
  pinMode(pinZumb, OUTPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);

  //Salida riego
  pinMode(pinLedAzul, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  valorPot = analogRead(pinPotenciometro);
  digitalWrite(pinLedRojo, LOW);
  digitalWrite(pinLedVerde, LOW);
  //Serial.println(valorPot);

  int leerDTH;
  float t, h; //temperatura y humedad
  
  if((leerDTH = dht11.read(h, t)) == 0) {
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" Humedad: ");
    Serial.println(h);

    if(h >= 42){
      Serial.println("/ REGANDO PLANTA");
      digitalWrite(pinLedAzul, HIGH);
      delay(3000);
      digitalWrite(pinLedAzul, LOW);
    }
  }

  if(valorPot == 0) { //si el potenciometro llega a 0
    //activar sensor movimiento
    Serial.println("/ ES DE NOCHE");

    valorPIR = digitalRead(pinPIR);
    
    if(valorPIR == HIGH) { //si existe movimiento
      //activar alarma
      Serial.println("HAY MOVIMIENTO");

      funcionarAlarma();
      
    }
    else {
      Serial.println("NO HAY MOVIMIENTO");
      //break;
    }

    delay(100);
    
  }
  else if(valorPot == 1023) {
    Serial.println("/ ES DE DIA");
    //break;
  }
  
  delay(1000);
}

void funcionarAlarma() {
  for(int i = 0; i <= 50; i++) {
    
    salidaBoton = digitalRead(pinBoton);
    if(salidaBoton == HIGH){
      activado = 1;
    }
    else {
      activado = 0;
    }
    
    if(activado == 1){ //si se activa el boton la alarma se desactiva
      Serial.println("ALARMA DESACTIVADA");
      digitalWrite(pinLedVerde, HIGH);
      noSonar();
      break;
    }
    else if (activado == 0 && i >= 50) { //si no se activa el boton la alarma empieza a sonar
      Serial.println("ALARMA ACTIVADA");
      sonar();
      digitalWrite(pinLedRojo, HIGH);
      delay(2000);
    }

    Serial.println("TIEMPO CORRIENDO");
    delay(100);
  }
}

void sonar() {
  tone(pinZumb, 20000, 2000);
}

void noSonar(){
  noTone(pinZumb);
  delay(100);
}
