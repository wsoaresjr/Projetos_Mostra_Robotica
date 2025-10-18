#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ====== AJUSTE AQUI O ENDEREÇO ACHADO NO SCANNER ======
#define LCD_ADDR 0x27  // ex.: 0x27 ou 0x3F
// ======================================================

LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

// Pinos dos botões (usarão INPUT_PULLUP)
const int PIN_V = 2;  // Verdadeiro
const int PIN_F = 3;  // Falso

// Debounce
const unsigned long DEBOUNCE_MS = 50;
unsigned long lastChangeV = 0, lastChangeF = 0;
bool lastStateV = HIGH, lastStateF = HIGH; // INPUT_PULLUP -> repouso = HIGH

// Reinício (segurando os dois botões)
const unsigned long RESET_HOLD_MS = 2000;

// Banco de perguntas
struct Questao {
  const char* texto;
  bool resposta; // true=Verdadeiro, false=Falso
};

Questao quiz[] = {
  {"A agua ferve a 100C", true},
  {"O zero eh primo", false},
  {"A Terra orbita o Sol", true},
  {"Pi = 3 exato", false},
  {"Brasil fica na America do Sul", true},
  {"LED precisa de resistor", true},
  {"DHT11 mede pressao", false},
  {"UNO usa ATmega328P", true}
};
const int TOTAL = sizeof(quiz) / sizeof(quiz[0]);

int idx = 0;         // índice da questão atual
int acertos = 0;     // placar

// ======= CONTROLE DE ROLAGEM =======
String textoPergunta = "";   // "Qx: <texto>"
String scrollBuffer = "";    // textoPergunta + 16 espaços (para linha em branco)
int scrollPos = 0;
unsigned long lastScroll = 0;
const unsigned long SCROLL_INTERVAL = 250; // velocidade da rolagem (ms)

void mostrarPergunta();
void processarResposta(bool respAluno);
void reiniciarQuiz();
void aguardarSoltar(int pin);
void atualizarRolagem();

void setup() {
  Wire.begin();                 // <<< importante para I2C no UNO
  Serial.begin(9600);
  Serial.println("Iniciando Quiz V/F...");

  pinMode(PIN_V, INPUT_PULLUP);
  pinMode(PIN_F, INPUT_PULLUP);

  // Inicialização do LCD (tente init(); se sua lib exigir begin(), troque)
  lcd.init();
  // lcd.begin(16, 2);
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Quiz V / F");
  lcd.setCursor(0,1); lcd.print("Iniciando...");
  delay(800);

  Serial.println("LCD inicializado. Mostrando primeira pergunta.");
  mostrarPergunta();
}

void loop() {
  // Atualiza rolagem não-bloqueante
  atualizarRolagem();

  // Leitura com debounce (V)
  bool readingV = digitalRead(PIN_V);
  if (readingV != lastStateV) {
    lastChangeV = millis();
    lastStateV = readingV;
  } else {
    if (readingV == LOW && (millis() - lastChangeV) > DEBOUNCE_MS) {
      Serial.println("Botao V pressionado");
      processarResposta(true);
      aguardarSoltar(PIN_V);
    }
  }

  // Leitura com debounce (F)
  bool readingF = digitalRead(PIN_F);
  if (readingF != lastStateF) {
    lastChangeF = millis();
    lastStateF = readingF;
  } else {
    if (readingF == LOW && (millis() - lastChangeF) > DEBOUNCE_MS) {
      Serial.println("Botao F pressionado");
      processarResposta(false);
      aguardarSoltar(PIN_F);
    }
  }

  // Verifica reset segurando os dois botões
  if (digitalRead(PIN_V) == LOW && digitalRead(PIN_F) == LOW) {
    unsigned long t0 = millis();
    while (digitalRead(PIN_V) == LOW && digitalRead(PIN_F) == LOW) {
      if (millis() - t0 >= RESET_HOLD_MS) {
        Serial.println("Reset por ambos os botoes");
        reiniciarQuiz();
        break;
      }
      delay(10);
    }
  }
}

void mostrarPergunta() {
  lcd.clear();
  if (idx >= TOTAL) {
    lcd.setCursor(0,0); lcd.print("Fim! Acertos:");
    lcd.setCursor(0,1); lcd.print(acertos); lcd.print("/"); lcd.print(TOTAL);
    Serial.print("Fim do quiz. Acertos: ");
    Serial.print(acertos); Serial.print("/"); Serial.println(TOTAL);
    return;
  }

  String linha1 = "Q"; linha1 += (idx + 1); linha1 += ": ";
  String texto = String(quiz[idx].texto);
  textoPergunta = linha1 + texto;

  // Monta buffer com 16 espaços ao final para "varrer" e zerar a linha
  scrollBuffer = textoPergunta + "                "; // 16 espaços
  scrollPos = 0;
  lastScroll = millis();

  // Mostra primeiro frame
  lcd.setCursor(0,0);
  if (textoPergunta.length() <= 16) lcd.print(textoPergunta);
  else lcd.print(scrollBuffer.substring(0,16));

  lcd.setCursor(0,1);
  lcd.print("[V] ou [F]?   ");

  Serial.print("Pergunta "); Serial.print(idx+1); Serial.print(": ");
  Serial.println(texto);
}

void atualizarRolagem() {
  // Se couber na linha, não rola
  if (textoPergunta.length() <= 16) return;

  if (millis() - lastScroll >= SCROLL_INTERVAL) {
    lastScroll = millis();

    // janela de 16 chars dentro do buffer (texto + 16 espaços)
    int maxStart = scrollBuffer.length() - 16; // último início possível
    String janela = scrollBuffer.substring(scrollPos, scrollPos + 16);

    lcd.setCursor(0,0);
    lcd.print(janela);

    // avança; quando passar do último início, reinicia (linha ficará vazia antes)
    scrollPos++;
    if (scrollPos > maxStart) {
      scrollPos = 0;
    }
  }
}

void processarResposta(bool respAluno) {
  if (idx >= TOTAL) return;

  bool correta = (respAluno == quiz[idx].resposta);
  if (correta) acertos++;
  Serial.print("Resposta aluno: "); Serial.print(respAluno ? "V" : "F");
  Serial.print(" | Gabarito: "); Serial.print(quiz[idx].resposta ? "V" : "F");
  Serial.print(" | "); Serial.println(correta ? "CORRETO" : "ERRADO");

  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Sua resposta:");
  lcd.setCursor(0,1); lcd.print(respAluno ? "Verdadeiro" : "Falso");
  delay(700);

  lcd.clear();
  lcd.setCursor(0,0); lcd.print(correta ? "Correto! :)" : "Errado :(");
  lcd.setCursor(0,1); lcd.print("Gabarito: ");
  lcd.print(quiz[idx].resposta ? "V" : "F");
  delay(900);

  idx++;
  mostrarPergunta();
}

void reiniciarQuiz() {
  idx = 0;
  acertos = 0;
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Reiniciando...");
  delay(500);
  mostrarPergunta();
}

void aguardarSoltar(int pin) {
  while (digitalRead(pin) == LOW) { delay(5); }
}
