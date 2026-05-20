# 🔐 Locker Inteligente - Cofre IoT

Este projeto consiste em um sistema de **Cofre Eletrônico Inteligente** controlado por microcontrolador (compatível com a plataforma Arduino). Ele foi desenvolvido como parte prática da matéria de **Internet das Coisas (IoT)**.

O sistema utiliza autenticação por senha numérica via botões físicos, fornece feedback visual através de um display LCD e LEDs, emite alertas sonoros com um buzzer e realiza a tranca mecânica automatizada por meio de um servo motor.

---

## 👥 Integrantes do Grupo
* **Wellington de Menezes Paim**
* **Joao Pedro Martins**
* **Yuri Lucas Oishi**

---

## 🛠️ Componentes Utilizados

| Componente | Quantidade | Descrição |
| :--- | :---: | :--- |
| **Microcontrolador** | 1 | Placa compatível com Arduino (Ex: Uno, Nano) |
| **Display LCD 16x2** | 1 | Interface visual para instruções e status do sistema |
| **Servo Motor** | 1 | Atuador responsável por trancar e destrancar o cofre |
| **Push Buttons** | 4 | Botões para inserção da senha (B1, B2, B3 e B4) |
| **LED Verde** | 1 | Indicador visual de cofre aberto / sucesso |
| **LED Vermelho** | 1 | Indicador visual de cofre trancado / erro / bloqueio |
| **Buzzer** | 1 | Feedback sonoro para cliques, erros e alarmes |
| **Resistores** | - | Conforme necessário para proteção dos LEDs (ex: 220Ω) |

---

## 📌 Mapeamento de Pinos (Hardware)

| Componente | Pino no Arduino | Configuração no Código |
| :--- | :---: | :--- |
| **LCD RS** | A0 | `LiquidCrystal lcd(...)` |
| **LCD Enable** | A1 | `LiquidCrystal lcd(...)` |
| **LCD D4** | A2 | `LiquidCrystal lcd(...)` |
| **LCD D5** | A3 | `LiquidCrystal lcd(...)` |
| **LCD D6** | A4 | `LiquidCrystal lcd(...)` |
| **LCD D7** | A5 | `LiquidCrystal lcd(...)` |
| **Botão 1 (BTN1)** | D2 | `INPUT_PULLUP` |
| **Botão 2 (BTN2)** | D3 | `INPUT_PULLUP` |
| **Botão 3 (BTN3)** | D4 | `INPUT_PULLUP` |
| **Botão 4 (BTN4)** | D5 | `INPUT_PULLUP` |
| **LED Verde** | D6 | `OUTPUT` |
| **LED Vermelho** | D7 | `OUTPUT` |
| **Buzzer** | D8 | `OUTPUT` |
| **Servo Motor** | D9 | `servo.attach(9)` |

*Nota: Os botões utilizam os resistores internos do Arduino via configuração `INPUT_PULLUP`, o que significa que o circuito fecha em nível lógico Baixo (LOW) quando pressionados.*

---

## 🎮 Funcionamento do Sistema

1. **Estado Inicial (Trancado):**
   * O display exibe `LOCKER INTELIG. / Digite a senha:`.
   * O LED Vermelho fica aceso indicando que está trancado.
   * O Servo Motor se posiciona em 0° (fechado).

2. **Inserção da Senha:**
   * A senha correta padrão definida no código é uma sequência de 4 dígitos: **`[B1, B2, B1, B2]`** (mapeada no array de botões correspondentes aos pinos físicos 2 e 3).
   * A cada botão pressionado, um bipe curto soa e um asterisco `*` é impresso no display para proteger a privacidade da senha.

3. **Abertura (Sucesso):**
   * Caso a senha inserida esteja correta, o display exibe `COFRE ABERTO!`.
   * O LED Verde acende e o Vermelho se apaga.
   * O Servo Motor gira para 90° (aberto).
   * O sistema aguarda o pressionamento do **Botão 1 (BTN1)** para fechar novamente o cofre.

4. **Tratamento de Erros e Bloqueio de Segurança:**
   * Se a senha estiver incorreta, o LED Vermelho pisca, o buzzer emite um som grave e o display informa a quantidade de tentativas restantes.
   * O usuário tem direito a **3 tentativas**.
   * Caso erre as 3 vezes consecutivas, o sistema entra em **Modo de Bloqueio** por **10 segundos**. Durante este período, o alarme soa repetidamente e uma contagem regressiva é exibida no LCD, impedindo novas tentativas até o término do tempo.

---

## 🚀 Como Executar o Projeto

1. **Instalação de Bibliotecas:** Certifique-se de ter as bibliotecas nativas `LiquidCrystal.h` e `Servo.h` instaladas na sua Arduino IDE.
2. **Montagem do Circuito:** Conecte os componentes seguindo a tabela de [Mapeamento de Pinos](#-mapeamento-de-pinos-hardware).
3. **Upload do Código:** Abra o código do projeto na Arduino IDE, selecione a placa correta e a porta COM, e clique em carregar (Upload).

---
Desenvolvido para fins acadêmicos na disciplina de Internet das Coisas (IoT). 🛡️
