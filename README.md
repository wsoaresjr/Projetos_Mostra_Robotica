# Mostra de Robótica — Projeto Educacional Interativo

## 🧩 Visão Geral

A **Mostra de Robótica** é um projeto educacional desenvolvido para integrar a aprendizagem de **ciência, tecnologia e engenharia** por meio da **robótica aplicada**. O repositório reúne diversos experimentos, códigos e protótipos voltados para exposições, feiras e demonstrações didáticas em escolas e eventos de tecnologia.

Cada subpasta do projeto contém um conjunto de **experimentos temáticos** com Arduino e sensores diversos, pensados para que os alunos possam **aprender, testar e apresentar** projetos de forma lúdica e colaborativa.

---

## ⚙️ Estrutura do Repositório

A estrutura geral do projeto está organizada da seguinte forma:

```
Projetos_Mostra_Robotica-main/
│
├── quiz_lcd_scroll/           → Quiz interativo V/F com LCD I2C e botões
├── sensor_ultrassonico/       → Medição de distância e alerta de obstáculos
├── seguidor_de_linha/         → Robô que segue trilhas pretas utilizando sensores TCRT5000
├── estacionador_automatico/   → Protótipo de estacionamento autônomo com buzzer e LEDs
├── robo_explorador/           → Carrinho com sensores e desvio automático de obstáculos
├── painel_solar/              → Experimento com painel fotovoltaico e medição de tensão
└── ... outros módulos temáticos
```

Cada pasta contém:

* **Código-fonte Arduino (.ino)**
* **README específico** com explicações e diagramas
* **Imagens e esquemas de ligação**
* (Opcionalmente) vídeos e apresentações para demonstração

---

## 🧠 Objetivos Educacionais

O projeto foi concebido com foco em:

* Desenvolver o **pensamento computacional** e a **resolução de problemas**;
* Promover o **aprendizado ativo e prático** em robótica e eletrônica;
* Estimular o trabalho em equipe e a apresentação de projetos;
* Possibilitar o uso interdisciplinar nas áreas de **física, matemática, tecnologia e engenharia**.

---

## 🔌 Componentes Comuns

Os projetos utilizam, em diferentes combinações, os seguintes componentes:

| Componente                         | Descrição                         |
| ---------------------------------- | --------------------------------- |
| Arduino Uno R3                     | Placa microcontroladora principal |
| Display LCD 16x2 (I2C)             | Exibição de dados e mensagens     |
| Módulo Sensor Ultrassônico HC-SR04 | Medição de distância              |
| Módulo Ponte H L298N               | Controle de motores DC            |
| Sensores TCRT5000                  | Detecção de linha preta/branca    |
| Módulo DHT11                       | Leitura de temperatura e umidade  |
| Módulo de chuva                    | Detecção de umidade superficial   |
| Painel Solar 12V / 150mA           | Experimento de energia solar      |
| Buzzer e LEDs                      | Sinalização sonora e visual       |

---

## 🚀 Exemplos de Projetos Incluídos

### 🔹 Quiz V/F com LCD I2C

Um jogo interativo de Verdadeiro ou Falso, controlado por dois botões. As perguntas rolam automaticamente no display LCD.

### 🔹 Robô Explorador

Carrinho com sensores ultrassônicos que detecta obstáculos e faz desvios automáticos aleatórios.

### 🔹 Estacionador Automático

Simula um sistema de estacionamento inteligente que mede distância e emite alertas visuais e sonoros.

### 🔹 Painel Solar Fotovoltaico

Demonstra a conversão de energia solar em elétrica, com medição de tensão e corrente em tempo real.

### 🔹 Sensor de Linha

Protótipo que segue uma linha preta sobre fundo branco, utilizando dois sensores TCRT5000 e controle diferencial de motores.

---

## 🧰 Ferramentas e Bibliotecas Utilizadas

| Biblioteca            | Função                                                   |
| --------------------- | -------------------------------------------------------- |
| `Wire.h`              | Comunicação I2C                                          |
| `LiquidCrystal_I2C.h` | Controle de display LCD I2C                              |
| `NewPing.h`           | Controle do sensor ultrassônico HC-SR04                  |
| `Servo.h`             | Controle de servo motores                                |
| `DHT.h`               | Leitura de sensores de temperatura e umidade DHT11/DHT22 |

---

## 🧑‍💻 Como Usar

1. **Abra o Arduino IDE**.
2. **Instale as bibliotecas** necessárias (Menu: *Sketch > Incluir Biblioteca > Gerenciar Bibliotecas*).
3. **Conecte o Arduino Uno** ao computador via USB.
4. **Carregue o código do projeto desejado** (arquivo `.ino`).
5. **Monte o circuito** conforme o esquema fornecido em cada pasta.
6. **Envie o código** para o Arduino (botão ▶️ *Upload*).
7. **Teste e demonstre** o funcionamento!

---

## 🏫 Aplicação Didática

Este conjunto de projetos foi idealizado para **feiras de ciências, aulas práticas de robótica e mostras escolares**. Pode ser utilizado em:

* Clubes de robótica;
* Oficinas maker;
* Aulas interdisciplinares (Matemática, Física, Tecnologia);
* Projetos sociais e de iniciação científica.

Cada projeto favorece a **aprendizagem ativa** e o **trabalho colaborativo**, permitindo que estudantes compreendam conceitos de programação, eletrônica e automação de maneira envolvente.

---

## 🧩 Créditos e Autoria

**Autor:** Walter Soares
**Instituição:** Exploratória Digital — Projeto Social de Robótica
**Versão:** 1.0
**Linguagem:** Arduino C++
**Licença:** MIT

---

## 💡 Próximos Passos

* Criação de um **painel de controle** via Wi-Fi usando ESP8266/ESP32.
* Integração com **plataformas de telemetria** (ThingSpeak / Blynk).
* Desenvolvimento de um **kit didático modular** com base nos experimentos.

---
