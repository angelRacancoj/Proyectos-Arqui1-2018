/**
 * Autor: Misael Saenz Flores
 * Esta obra está bajo una Licencia Creative Commons 
 * Atribución-NoComercial-SinDerivadas 4.0 Internacional.
 */

#define Motor1_1 2 //Pin 2 L293D
#define Motor1_2 3 //Pin 7 L293D
#define Motor2_1 4 //Pin 15 L293D
#define Motor2_2 5 //Pin 12 L293D

char dato;

void setup() {
  Serial.begin(9600);
  pinMode(Motor1_1, OUTPUT);
  pinMode(Motor1_2, OUTPUT);
  pinMode(Motor2_1, OUTPUT);
  pinMode(Motor2_2, OUTPUT);

}

void loop() {
  if (Serial.available() > 0) {
    dato = Serial.read();
  }

  switch (dato) {
    case 'w':
      Adelante();
      Serial.println("Adelante");
      break;
    case 'a':
      Serial.println("Izquierda");
      Derecha();
      //Izquierda();
      break;
    case 'd':
      Serial.println("Derecha");
      Izquierda();
      //Derecha();
      break;
    case 'f':
      Serial.println("Alto");
      Alto();
      break;
    case 's':
      Serial.println("Reversa");
      Reversa();
      break;
    default:
      Alto();
      break;
  }

}

void Adelante() {
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, HIGH);
  digitalWrite(Motor2_2, HIGH);
  digitalWrite(Motor2_1, LOW);
}

void Derecha() {
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, HIGH);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, LOW);
}

void Izquierda() {
  digitalWrite(Motor2_2, HIGH);
  digitalWrite(Motor2_1, LOW);
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, LOW);
}

void Alto() {
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, LOW);
  digitalWrite(Motor1_1, LOW);
  digitalWrite(Motor1_2, LOW);
}

void Reversa() {
  digitalWrite(Motor1_1, HIGH);
  digitalWrite(Motor1_2, LOW);
  digitalWrite(Motor2_2, LOW);
  digitalWrite(Motor2_1, HIGH);
}
