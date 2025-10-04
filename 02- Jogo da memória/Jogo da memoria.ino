// Pinos dos LEDs (saída)
const int ledPins[] = {2, 3, 4, 5};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// Pinos dos Botões (entrada)
const int buttonPins[] = {6, 7, 8, 9};
const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

// Variáveis do Jogo
int gameSequence[100]; // Armazena a sequência do jogo, máximo de 100 passos
int sequenceLength = 0; // Comprimento atual da sequência
int roundNumber = 0; // Número da rodada

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Inicializa o gerador de números aleatórios com ruído do pino analógico

  // Configura os pinos dos LEDs como SAÍDA
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Desliga todos os LEDs inicialmente
  }

  // Configura os pinos dos botões como ENTRADA com PULLUP interno
  // Isso simplifica a fiação, você só precisa conectar o botão ao pino e ao GND
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  Serial.println("Jogo da Memoria - Pronto!");
  delay(1000); // Pequena pausa antes de começar
}

void loop() {
  // Inicia uma nova rodada
  roundNumber++;
  Serial.print("Rodada: ");
  Serial.println(roundNumber);

  // Gera um novo passo na sequência
  generateNextStep();

  // Mostra a sequência atual para o jogador
  showSequence();

  // Obtém a entrada do jogador
  if (!getPlayerInput()) {
    gameOver();
    while (true); // Trava o programa após o Game Over
  }

  delay(1500); // Pequena pausa entre as rodadas
}

void generateNextStep() {
  int newColor = random(numLeds); // Gera um número aleatório entre 0 e numLeds-1
  gameSequence[sequenceLength] = newColor;
  sequenceLength++;
  Serial.print("Sequencia gerada (apenas para debug): ");
  for (int i = 0; i < sequenceLength; i++) {
    Serial.print(gameSequence[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void showSequence() {
  Serial.println("Preste atencao na sequencia:");
  for (int i = 0; i < sequenceLength; i++) {
    int colorToShow = gameSequence[i];
    digitalWrite(ledPins[colorToShow], HIGH); // Acende o LED
    delay(500); // Mantém aceso por meio segundo
    digitalWrite(ledPins[colorToShow], LOW); // Apaga o LED
    delay(200); // Pequena pausa entre os LEDs
  }
  Serial.println("Sua vez!");
}

boolean getPlayerInput() {
  int playerInput[sequenceLength]; // Array para armazenar a entrada do jogador
  int inputIndex = 0;

  unsigned long lastButtonPressTime = millis(); // Para timeout da entrada

  while (inputIndex < sequenceLength) {
    // Timeout para o jogador não demorar demais
    if (millis() - lastButtonPressTime > 10000) { // 10 segundos para cada passo
      Serial.println("Tempo esgotado!");
      return false;
    }

    for (int i = 0; i < numButtons; i++) {
      // Quando um botão é pressionado (INPUT_PULLUP significa LOW quando pressionado)
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(50); // Debounce
        if (digitalRead(buttonPins[i]) == LOW) { // Confirma o pressionamento

          // Acende o LED correspondente ao botão pressionado para feedback
          digitalWrite(ledPins[i], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);

          playerInput[inputIndex] = i; // Armazena a "cor" do botão pressionado
          Serial.print("Voce pressionou: ");
          Serial.println(i);

          // Verifica se o passo está correto imediatamente
          if (playerInput[inputIndex] != gameSequence[inputIndex]) {
            Serial.println("Sequencia incorreta!");
            return false; // Errou a sequência
          }

          inputIndex++;
          lastButtonPressTime = millis(); // Reseta o timer de timeout
          // Espera o botão ser solto antes de continuar para evitar múltiplas leituras
          while (digitalRead(buttonPins[i]) == LOW);
          delay(50); // Pequeno debounce após soltar
        }
      }
    }
  }
  Serial.println("Sequencia correta!");
  return true; // Acertou a sequência
}

void gameOver() {
  Serial.println("GAME OVER!");
  Serial.print("Voce chegou na rodada: ");
  Serial.println(roundNumber);
  // Piscar todos os LEDs rapidamente para indicar Game Over
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(150);
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(150);
  }
}
