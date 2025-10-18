# Quiz V/F com LCD I2C e Rolagem de Texto

## Descrição do Projeto

Este projeto implementa um **quiz de perguntas Verdadeiro ou Falso (V/F)** utilizando um **Arduino Uno**, um **display LCD 16x2 com interface I2C** e **dois botões** (um para Verdadeiro e outro para Falso). As perguntas são exibidas no display, com **rolagem automática** quando o texto ultrapassa 16 caracteres.

O jogo percorre uma lista de perguntas pré-definidas, contabiliza os acertos e exibe o placar final. É possível **reiniciar o quiz** segurando os dois botões simultaneamente por alguns segundos.

---

## Componentes Necessários

* Arduino Uno R3
* Display LCD 16x2 com interface I2C
* 2 Botões (push buttons)
* 2 Resistores de 10kΩ (opcional, pois usamos `INPUT_PULLUP`)
* Jumpers e Protoboard

---

## Ligações

| Componente | Pino Arduino | Observações                     |
| ---------- | ------------ | ------------------------------- |
| LCD I2C    | SDA (A4)     | Comunicação I2C                 |
| LCD I2C    | SCL (A5)     | Comunicação I2C                 |
| Botão V    | D2           | Usa INPUT_PULLUP (ativo em LOW) |
| Botão F    | D3           | Usa INPUT_PULLUP (ativo em LOW) |

---

## Funcionamento do Código

### 1. Inicialização

* O código utiliza a biblioteca `LiquidCrystal_I2C` para controlar o display.
* O endereço I2C (`LCD_ADDR`) deve ser configurado conforme o resultado do **scanner I2C** (por exemplo, `0x27` ou `0x3F`).
* Ao iniciar, o LCD exibe a mensagem *"Quiz V/F - Iniciando..."* e mostra a primeira pergunta.

### 2. Estrutura das Perguntas

As perguntas estão armazenadas em um vetor de estruturas:

```cpp
struct Questao {
  const char* texto;
  bool resposta; // true = Verdadeiro, false = Falso
};
```

Cada elemento contém o texto da pergunta e a resposta correta.

### 3. Rolagem Automática

Quando o texto da pergunta excede 16 caracteres, o código ativa uma **rolagem automática** que percorre toda a linha e deixa o display em branco antes de reiniciar a exibição da pergunta. A velocidade é ajustável por meio da constante:

```cpp
const unsigned long SCROLL_INTERVAL = 250; // ms
```

### 4. Leitura dos Botões

Os botões utilizam o modo `INPUT_PULLUP`, o que significa que o estado normal é **HIGH** e o pressionamento gera um **LOW**. O código aplica **debounce** e reconhece a resposta do aluno:

* Botão Verdadeiro → `PIN_V`
* Botão Falso → `PIN_F`

### 5. Verificação e Feedback

Após responder, o LCD exibe:

* *"Correto! :)"* ou *"Errado :("*
* Em seguida, mostra o gabarito (V ou F) e avança para a próxima pergunta.

### 6. Reinício do Quiz

Se o aluno pressionar e segurar **ambos os botões por 2 segundos**, o quiz é reiniciado.

---

## Principais Funções

| Função                              | Descrição                                                         |
| ----------------------------------- | ----------------------------------------------------------------- |
| `mostrarPergunta()`                 | Exibe a pergunta atual e inicia o texto de rolagem se necessário. |
| `atualizarRolagem()`                | Atualiza a posição da rolagem do texto.                           |
| `processarResposta(bool respAluno)` | Compara a resposta do aluno com o gabarito e exibe o resultado.   |
| `reiniciarQuiz()`                   | Reinicia o índice e o contador de acertos.                        |
| `aguardarSoltar(int pin)`           | Aguarda o botão ser solto antes de continuar.                     |

---

## Possíveis Melhorias

* Adicionar **buzzer** para feedback sonoro (acerto/erro).
* Armazenar pontuação em **EEPROM**.
* Exibir **mensagens aleatórias de incentivo**.
* Implementar **modo de seleção de dificuldade**.

---


