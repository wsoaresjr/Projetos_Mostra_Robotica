// Definição dos pinos do Sensor Ultrasônico
const int pinoTrig = 7; // Trigger na porta Digital 7
const int pinoEcho = 6;  // Echo na porta Digital 6

// Definição do pino do Relé
const int pinoRele = 2; // Módulo Relé na porta Digital 2

// Limite de Distância em centímetros
const long DISTANCIA_LIMITE = 20; // A bomba liga se a distância for MENOR que 20 cm

void setup() {
  // Configuração dos pinos do sensor como entrada/saída
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  // Configuração do pino do Relé como saída
  pinMode(pinoRele, OUTPUT);

  // Garante que a bomba inicie desligada
  digitalWrite(pinoRele, HIGH); // HIGH geralmente DESLIGA o relé (depende do seu módulo)

  // Inicia a comunicação serial para monitoramento
  Serial.begin(9600);
  Serial.println("Sistema de Bomba por Aproximacao Iniciado");
}

void loop() {
  // 1. Gera um pulso no pino Trig para iniciar a medição
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);

  // 2. Mede a duração do pulso de retorno (Echo)
  long duracao = pulseIn(pinoEcho, HIGH);

  // 3. Calcula a distância em centímetros
  // (Velocidade do som no ar é 340 m/s ou 0.034 cm/µs. Como a onda vai e volta, dividimos por 2)
  long distanciaCm = duracao * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distanciaCm);
  Serial.println(" cm");

  // 4. Lógica de Aproximação
  if (distanciaCm < DISTANCIA_LIMITE) {
    // Objeto (ou pessoa) está PRÓXIMO.
    digitalWrite(pinoRele, LOW); // LOW geralmente LIGA o relé
    Serial.println("--> APROXIMACAO DETECTADA! BOMBA LIGADA! <--");
  } else {
    // Objeto (ou pessoa) está LONGE ou não foi detectado.
    digitalWrite(pinoRele, HIGH); // HIGH geralmente DESLIGA o relé
    Serial.println("Distancia Segura. Bomba Desligada.");
  }

  // Pequeno intervalo entre as leituras
  delay(500);
}
