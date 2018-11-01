/**
 * Autor: Misael Saenz Flores
 * Modificado por: Angel Racancoj
 *
 * Este codigo originalmente se dedicaba al manejo de un automovil mediante bluetooth
 * ha sido optimizado para una respuesta mas rapida
 *
 * Las modificaciones principal en el codigo, son un manejo de enciendido y apagado de dos luces
 * y se ha agregado el manejo en modo sigue lineas indicado con el metodo "Automatico"
 *
 * +------------------------------------------------------------------------+
 * | Descripcion del Automovil:                                             |
 * | Se utilizo un auto a control remoto con un motor en la parte frontal   |
 * | encargado del giro y un motor en la parte posterior para la traccion   |
 * | el restro de atributos como la colocacion de los sensores y luces      |
 * | queda a criterio de quien construya el auto                            |
 * +------------------------------------------------------------------------+
 *
 * Modulos Utilizados:
 * Arduino Nano
 * Sensor Ultrasonico HC-SR04 (Para el control de freno automatico respecto a la distancia)
 * Modulo de sensor infrarojo MH-sensor
 * Modulo Bluetooth ZS-040
 * Modulo de Puente H 
 *
 * Esta obra está bajo una Licencia Creative Commons 
 * Atribución-NoComercial-SinDerivadas 4.0 Internacional.
 */

#define Motor1_1 2 //Pin 2 L293D
#define Motor1_2 3 //Pin 7 L293D
#define Motor2_1 4 //Pin 15 L293D
#define Motor2_2 5 //Pin 12 L293D

#define InfraD 6  //Pin entrada sensor infrarojo derecho
#define InfraI 7  //Pin entrada sensor infrarojo derecho

#define GreenLight 8 //green light
#define frontLight 9 //Principal Light

#define Trig 10 //Ultrasonic sensor trig port
#define Echo 11 //Untrasonic sensor echo port

char modoManejo;
char dato;
char directo;

int tiempo = 400;
int tiempo2 = 750; 

int valInfraD = 0;
int valInfraI = 0;

int blanco = 0;
int negro = 1;

void setup() {
  Serial.begin(9600);
  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);

  pinMode(InfraD, INPUT);
  pinMode(InfraI, INPUT);

  pinMode(GreenLight, OUTPUT);
  pinMode(frontLight, OUTPUT);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  Alto();

  modoManejo = 'x';
  directo = 'v';
}

void loop() {
  long distance = medirDistancia();

  Serial.print("D");
  Serial.println(distance);
  delay(10);


  while (Serial.available() > 0) {
    dato = Serial.read();
    //Serial.print("Dato entrada: ");
    //Serial.println(dato);
    switch(dato){
    case 'x':
    case 'X':
      modoManejo = dato;
      //Serial.print("Modo de automatico: ");
      //Serial.println(modoManejo);
      break;
    case 'V':
    case 'v':
      directo = dato;
      //Serial.print("Modo de automatico 2: ");
      //Serial.println(directo);
      break;      
    }

  }

  switch(modoManejo){
    //Manual -> Para ser utilizado a control remoto
  case 'x':
    Manual(dato);
    break;
    //Automatico -> Para ser utilizado como carro sigue lineas
  case 'X':

    valInfraD = digitalRead(InfraD);
    valInfraI = digitalRead(InfraI);
    Automatico(valInfraD, valInfraI, distance);
    break;
  }
}

long medirDistancia(){
  //Funcion de control del sensor ultra sonico, esta funcion esta unicamente habilitada para modo Sigue lineas
  digitalWrite(Trig, LOW);
  delayMicroseconds(5);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  long duration = pulseIn(Echo, HIGH);

  return microSecToCm(duration);
}

long microSecToCm(long microSeconds){
  return microSeconds/29/2;
}

/*
* "Automatico" es la funcion asignada para el modo Siguelineas
* tiene dos modos de uso en esta modalidad:
* -> La primera es "semi automatica" donde se debe presionar la hacia adelante para
*    que funciones como sigue lineas (Previamente se debe seleccionar modo siguelineas)
* -> Completamente "automatico", donde "encender" esta segunda funcion el auto 
*    se convierte en completamente siguelineas
*/
void Automatico(int derecha, int izquierda, long distance){

  if(directo == 'v'){
    switch(dato){
        
    case'F':
        /*
        * "Semi automatico"
        */
      if(distance>20){
        Auto(derecha, izquierda);
      } 
      else {
        Alto();
      }
      break;
    case 'S':
      Alto();
      break;
    case 'W':
      Serial.println("Front Light On");
      digitalWrite(frontLight, HIGH);
      break;
    case 'w':
      Serial.println("Front Light Off");
      digitalWrite(frontLight, LOW);
      break;
    case 'U':
      Serial.println("Green Light On");
      digitalWrite(GreenLight, HIGH);
      break;
    case 'u':
      Serial.println("Green Light Off");
      digitalWrite(GreenLight, LOW);
      break;
    default:
      Alto();
      break;
    }
  } 
  else if(directo == 'V'){
    /*
    * "Automatico"
    */
    if(distance>20){
      Auto(derecha, izquierda);
    } 
    else {
      Alto();
    }

    switch(dato){
    case 'W':
      Serial.println("Front Light On");
      digitalWrite(frontLight, HIGH);
      break;
    case 'w':
      Serial.println("Front Light Off");
      digitalWrite(frontLight, LOW);
      break;
    case 'U':
      Serial.println("Green Light On");
      digitalWrite(GreenLight, HIGH);
      break;
    case 'u':
      Serial.println("Green Light Off");
      digitalWrite(GreenLight, LOW);
      break;
    }
  }

}

void Auto(int Der, int Izq){
  if((Der == blanco) && (Izq == blanco)){  //Adelante
    Adelante();
    delay(175);
    Reversa();
    delay(5);
  } 
  else if((Izq == blanco) && (Der == negro)){  //Izquierda
    IzquierdaRet();
    delay(tiempo);
    DerechaAd();
    delay(tiempo);
  } 
  else if ((Izq == negro) && (Der == blanco)){  //Derecha
    DerechaRet();
    delay(tiempo);
    IzquierdaAd();
    delay(tiempo);
  } 
  else {
    Alto();
  }
}

void Manual(char dato){
  switch (dato) {
  case 'F':
    Adelante();
    break;
  case 'B':
    Reversa();
    break; 
  case 'L':
    Izquierda();
    break;
  case 'R':
    Derecha();
    break;
  case 'G':
    IzquierdaAd();
    break;
  case 'I':
    DerechaAd();
    break;
  case 'H':
    IzquierdaRet();
    break;
  case 'J':
    DerechaRet();
    break;
  case 'S':
    Alto();
    break;
  case 'W':
    Serial.println("Front Light On");
    digitalWrite(frontLight, HIGH);
    break;
  case 'w':
    Serial.println("Front Light Off");
    digitalWrite(frontLight, LOW);
    break;
  case 'U':
    Serial.println("Green Light On");
    digitalWrite(GreenLight, HIGH);
    break;
  case 'u':
    Serial.println("Green Light Off");
    digitalWrite(GreenLight, LOW);
    break; 
  default:
    Alto();
    break;
  }
}

void Adelante() {
  Serial.println("Adelante");
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, HIGH);
  digitalWrite(Motor2_1, LOW);
}

void Derecha() {
  Serial.println("Derecha");
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, HIGH);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, LOW);
}

void Izquierda() {
  Serial.println("Izquierda");
  digitalWrite(Motor1_1, HIGH);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, LOW);
}

void DerechaAd() {
  Serial.println("Derecha Adelante");
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, HIGH);
  digitalWrite(Motor2_2, HIGH);
  digitalWrite(Motor2_1, LOW);
}

void IzquierdaAd() {
  Serial.println("Izquierda Adelante");
  digitalWrite(Motor1_1, HIGH);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, HIGH);
  digitalWrite(Motor2_1, LOW);
}

void DerechaRet() {
  Serial.println("Derecha Retroceso");
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, HIGH);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, HIGH);
}

void IzquierdaRet() {
  Serial.println("Izquierda Retroceso");
  digitalWrite(Motor1_1, HIGH);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, HIGH);
}

void Alto() {
  Serial.println("Alto");
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, LOW);
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, LOW);
}

void Reversa() {
  Serial.println("Reversa");
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, HIGH);
}




