#include <Servo.h>

// —– Pinos do driver L298N —–
const int ENA = 5;   // PWM motor A
const int IN1 = 2;
const int IN2 = 3;
const int ENB = 6;   // PWM motor B
const int IN3 = 4;
const int IN4 = 7;

// —– Pinos dos sensores de linha —–
const int sensorEsq = A0;
const int sensorMid = A1;
const int sensorDir = A2;
const int limiarLinha = 600;  // calibrar conforme seu sensor

// —– Pinos do ultrassônico HC-SR04 —–
const int trigPin = 8;
const int echoPin = 9;
const long distObstaculo = 15; // cm

// —– Pinos do sensor de cor TCS3200 —–
const int S0 = 10;
const int S1 = 11;
const int S2 = 12;
const int S3 = 13;
const int outPin = A3;
const int freqScale = HIGH; // S0/S1 = HIGH/HIGH → 100%

// —– Servo para manipulação —–
Servo servoGarra;
const int pinoServo =  A4;

// —– Setup —–
void setup() {
Serial.begin(9600);

// motores
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
pinMode(ENB, OUTPUT);
pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

// linha
pinMode(sensorEsq, INPUT);
pinMode(sensorMid, INPUT);
pinMode(sensorDir, INPUT);

// ultrassônico
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

// sensor de cor
pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
pinMode(outPin, INPUT);
digitalWrite(S0, freqScale);
digitalWrite(S1, freqScale);

// servo
servoGarra.attach(pinoServo);
servoGarra.write(0); // garra fechada
}

// —– Funções de movimentação —–
void moveFrente(int speed=200) {
analogWrite(ENA, speed);
digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
analogWrite(ENB, speed);
digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveTras(int speed=200) {
analogWrite(ENA, speed);
digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
analogWrite(ENB, speed);
digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void giraEsq(int speed=180) {
analogWrite(ENA, speed);
digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
analogWrite(ENB, speed);
digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void giraDir(int speed=180) {
analogWrite(ENA, speed);
digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
analogWrite(ENB, speed);
digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void paraRobô() {
digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// —– Leitura ultrassônico —–
long leDistancia() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
long dur = pulseIn(echoPin, HIGH);
return (dur * 0.034) / 2;
}

// —– Leitura de cor (frequência) —–
long leCor(char modo) {
// configura S2/S3 para R, G ou B
if (modo=='R') { digitalWrite(S2, LOW);  digitalWrite(S3, LOW);  }
if (modo=='G') { digitalWrite(S2, HIGH); digitalWrite(S3, HIGH); }
if (modo=='B') { digitalWrite(S2, LOW);  digitalWrite(S3, HIGH); }
delay(100);
return pulseIn(outPin, LOW);
}

// —– Detecta “vítima” vermelha —–
bool detectaVitima() {
long r = leCor('R');
long g = leCor('G');
long b = leCor('B');
// calibrar estes valores no seu ambiente
if (r < 30 && g > 40 && b > 40) {
Serial.println("Vitima detectada!");
return true;
}
return false;
}

// —– Rotina de resgate —–
void rotinaResgate() {
paraRobô();
// abre garra
servoGarra.write(90);
delay(400);
// avança para encaixar o token
moveFrente(180);
delay(600);
paraRobô();
// fecha garra
servoGarra.write(0);
delay(400);
// retorna
moveTras(200);
delay(600);
paraRobô();
}

void loop() {
// 1) primeiro, checa se está próximo de obstáculo/vítima
if (leDistancia() < distObstaculo && detectaVitima()) {
rotinaResgate();
return;
}

// 2) senão, segue a linha
int esq = analogRead(sensorEsq);
int mid = analogRead(sensorMid);
int dir = analogRead(sensorDir);

if (mid < limiarLinha) {
moveFrente();
} else if (esq < limiarLinha) {
giraEsq();
} else if (dir < limiarLinha) {
giraDir();
} else {
// linha perdida: para e busca
paraRobô();
}
}
