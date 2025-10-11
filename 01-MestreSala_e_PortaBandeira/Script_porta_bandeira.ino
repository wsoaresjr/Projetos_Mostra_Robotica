// — parâmetros de hardware —
const int PWM_ESQUERDO = 5;    // PWM do motor esquerdo
const int IN_ESQ1     = 2;     // entrada 1 do H-bridge esquerdo
const int IN_ESQ2     = 3;     // entrada 2 do H-bridge esquerdo
const int PWM_DIREITO = 6;     // PWM do motor direito
const int IN_DIR1     = 4;     // entrada 1 do H-bridge direito
const int IN_DIR2     = 7;     // entrada 2 do H-bridge direito

// — parâmetros de trajetória —
const float RAIO        = 100.0;       // raio do círculo em mm (ajuste conforme necessário)
const float DIST_EIXO   = 90.0;        // distância entre as rodas (L) em mm
const int VELOCIDADE_BASE = 180;       // velocidade máxima de referência (0–255)
const unsigned long TEMPO_FASE = 5000; // duração de cada fase em ms

// SENTIDO: +1 para horário, -1 para anti-horário
// Defina  1 no mestre-sala, -1 no porta-bandeira
const int SENTIDO = 1;

unsigned long ultimaTroca = 0;  // armazena o instante da última mudança de fase
int etapa = 0;                  // fase atual: 0, 1 ou 2

void setup() {
  // configura pinos como saída
  pinMode(PWM_ESQUERDO, OUTPUT);
  pinMode(IN_ESQ1, OUTPUT);
  pinMode(IN_ESQ2, OUTPUT);
  pinMode(PWM_DIREITO, OUTPUT);
  pinMode(IN_DIR1, OUTPUT);
  pinMode(IN_DIR2, OUTPUT);

  ultimaTroca = millis();  // inicializa o contador de tempo
}

void loop() {
  unsigned long agora = millis();
  // verifica se já passou TEMPO_FASE para mudar de fase
  if (agora - ultimaTroca > TEMPO_FASE) {
    etapa = (etapa + 1) % 3;
    ultimaTroca = agora;
  }

  switch (etapa) {
    case 0:  // fase 1: mesmo sentido, velocidade reduzida
      orbitar(SENTIDO, VELOCIDADE_BASE * 0.6);
      break;
    case 1:  // fase 2: sentidos opostos, velocidade média
      orbitar(SENTIDO, VELOCIDADE_BASE);
      break;
    case 2:  // fase 3: mesmo sentido, velocidade máxima
      orbitar(-SENTIDO, VELOCIDADE_BASE);
      break;
  }
}

// orbita em um círculo de raio RAIO (mm)
// sentido: +1 = horário, -1 = anti-horário
// velocidade: valor de 0 a 255
void orbitar(int sentido, int velocidade) {
  // cálculo das velocidades das rodas:
  // velEsq = v*(RAIO - DIST_EIXO/2)/RAIO
  // velDir = v*(RAIO + DIST_EIXO/2)/RAIO
  float velEsq = velocidade * (RAIO - DIST_EIXO/2) / RAIO;
  float velDir = velocidade * (RAIO + DIST_EIXO/2) / RAIO;

  // aplica o sentido (+1 ou -1)
  ajustarMotores(velEsq * sentido, velDir * sentido);
}

// configura e aciona os dois motores
// velEsq e velDir podem ser positivos (para frente) ou negativos (para trás)
void ajustarMotores(float velEsq, float velDir) {
  // motor esquerdo
  if (velEsq >= 0) {
    digitalWrite(IN_ESQ1, HIGH);
    digitalWrite(IN_ESQ2, LOW);
  } else {
    digitalWrite(IN_ESQ1, LOW);
    digitalWrite(IN_ESQ2, HIGH);
    velEsq = -velEsq;
  }
  analogWrite(PWM_ESQUERDO, constrain((int)velEsq, 0, 255));

  // motor direito
  if (velDir >= 0) {
    digitalWrite(IN_DIR1, HIGH);
    digitalWrite(IN_DIR2, LOW);
  } else {
    digitalWrite(IN_DIR1, LOW);
    digitalWrite(IN_DIR2, HIGH);
    velDir = -velDir;
  }
  analogWrite(PWM_DIREITO, constrain((int)velDir, 0, 255));
}
