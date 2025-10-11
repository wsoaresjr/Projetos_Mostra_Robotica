#include <Servo.h>

// --- 1. Definição dos Pinos e Constantes ---
const int PINO_SENSOR_CHUVA = 2; // Pino Digital para o Sensor de Chuva (OUT)
const int PINO_SERVO = 9;        // Pino PWM para o Servo Motor (Sinal)

// Posições do Servo (em graus)
const int POSICAO_FECHADO = 0;   // Posição inicial (sem chuva)
const int POSICAO_ABERTO = 90;   // Posição para 90 graus (com chuva)

// Variável para o objeto Servo
Servo meuServo; 


void setup() {
  Serial.begin(9600);
  Serial.println("Sistema de Sensor de Chuva e Servo Iniciado!");

  // Configura o pino do Sensor de Chuva como ENTRADA
  pinMode(PINO_SENSOR_CHUVA, INPUT);

  // Anexa o objeto Servo ao pino (liga o Servo)
  meuServo. 
    attach(PINO_SERVO);

  // Garante que o servo comece na posição "fechado" (sem chuva)
  meuServo.write(POSICAO_FECHADO);
  Serial.print("Servo na posicao inicial: ");
  Serial.println(POSICAO_FECHADO);

  // O Sensor de Chuva padrão retorna LOW quando há chuva (ÁGUA = LOW)
  // Se o seu sensor retornar HIGH com água, mude esta lógica no loop.
}


void loop() {
  
  // Lê o estado do Sensor de Chuva (LOW = Chuva, HIGH = Seco/Não detectado)
  int estadoSensor = digitalRead(PINO_SENSOR_CHUVA);

  // Se o sensor detectar chuva (LOW)
  if (estadoSensor == LOW) {
    
    // Gira o Servo para 90 graus (POSICAO_ABERTO)
    meuServo.write(POSICAO_ABERTO);
    Serial.println("ALERTA: CHUVA DETECTADA! Servo girado para 90 graus.");
    
    // Pequeno delay para estabilizar o servo e não inundar o Serial Monitor
    delay(500); 
  } 
  // Se o sensor não detectar chuva (HIGH)
  else {
    
    // Retorna o Servo para 0 graus (POSICAO_FECHADO)
    meuServo.write(POSICAO_FECHADO);
    Serial.println("SECO: Nenhuma chuva detectada. Servo na posicao 0 grau.");
    
    // Delay maior quando o sistema está inativo
    delay(1000);
  }
}
