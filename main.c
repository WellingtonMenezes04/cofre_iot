/*
 * ================================================================
 *  LOCKER INTELIGENTE — Cofre Eletrônico com Arduino
 * ================================================================
 *  Descrição: Sistema de controle de acesso por senha de 4 dígitos
 *             utilizando 4 botões. Ao acertar a senha, o servo motor
 *             abre o cofre. Após 3 erros consecutivos, o sistema
 *             bloqueia por 10 segundos.
 *
 *  Hardware:
 *    - Arduino Uno
 *    - LCD 16x2 (modo 4 bits, pinos analógicos A0–A5)
 *    - Servo Motor SG90 ou similar (pino D9)
 *    - 4 Botões de pressão (D2, D3, D4, D5) com INPUT_PULLUP
 *    - LED Verde  (D6) com resistor 330 Ω
 *    - LED Vermelho (D7) com resistor 330 Ω
 *    - Buzzer passivo (D8)
 *
 *  Bibliotecas: LiquidCrystal (built-in), Servo (built-in)
 * ================================================================
 */

#include <LiquidCrystal.h>  // Biblioteca para controle do LCD
#include <Servo.h>          // Biblioteca para controle do servo motor

// ----------------------------------------------------------------
//  DEFINIÇÃO DOS PINOS
// ----------------------------------------------------------------

// LCD em modo 4 bits: RS, Enable, D4, D5, D6, D7 nos pinos analógicos
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo servo;  // Objeto que representa o servo motor

#define BTN1 2            // Botão 1 → dígito 1 da senha
#define BTN2 3            // Botão 2 → dígito 2 da senha
#define BTN3 4            // Botão 3 → dígito 3 da senha
#define BTN4 5            // Botão 4 → dígito 4 da senha
#define LED_VERDE    6    // LED de acesso autorizado (verde)
#define LED_VERMELHO 7    // LED de acesso negado / standby (vermelho)
#define BUZZER       8    // Buzzer para feedback sonoro
#define SERVO_PIN    9    // Sinal PWM do servo motor

// ----------------------------------------------------------------
//  CONFIGURAÇÃO DA SENHA
// ----------------------------------------------------------------

// Senha correta: sequência de 4 números de 1 a 4
// Cada número representa o botão a ser pressionado.
// Exemplo: {2, 3, 2, 3} = BTN2, BTN3, BTN2, BTN3
int senhaCorreta[4] = {2, 3, 2, 3};

// Armazena os botões pressionados pelo usuário na tentativa atual
int tentativa[4];

// Índice da posição atual na tentativa (0 a 3)
int posicao = 0;

// Contador de tentativas erradas consecutivas
int erros = 0;

// ----------------------------------------------------------------
//  SETUP — executado uma vez ao ligar o Arduino
// ----------------------------------------------------------------
void setup() {
  // Configura os pinos dos botões como entrada com resistor pull-up interno.
  // Com INPUT_PULLUP: pino em HIGH quando solto, LOW quando pressionado.
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);

  // Configura LEDs e buzzer como saída
  pinMode(LED_VERDE,    OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER,       OUTPUT);

  // Associa o servo ao pino definido e o coloca na posição 0° (fechado)
  servo.attach(SERVO_PIN);
  servo.write(0);  // 0° = cofre fechado

  // Inicializa o LCD com 16 colunas e 2 linhas
  lcd.begin(16, 2);

  // Exibe a tela inicial de boas-vindas
  telaInicial();
}

// ----------------------------------------------------------------
//  LOOP PRINCIPAL — fica verificando os botões continuamente
// ----------------------------------------------------------------
void loop() {
  // digitalRead retorna LOW (0) quando o botão está pressionado (lógica invertida pelo pull-up)
  // O delay(300) evita múltiplos registros por um único toque (debounce simples)
  if (!digitalRead(BTN1)) { registrar(1); delay(300); }
  if (!digitalRead(BTN2)) { registrar(2); delay(300); }
  if (!digitalRead(BTN3)) { registrar(3); delay(300); }
  if (!digitalRead(BTN4)) { registrar(4); delay(300); }
}

// ----------------------------------------------------------------
//  registrar() — registra o botão pressionado na tentativa atual
// ----------------------------------------------------------------
void registrar(int btn) {
  // Emite um bip curto de confirmação de toque (1000 Hz, 80 ms)
  tone(BUZZER, 1000, 80);

  // Salva o número do botão na posição atual do vetor tentativa[]
  tentativa[posicao] = btn;
  posicao++;  // Avança para o próximo dígito

  // Atualiza o LCD: exibe um asterisco "*" para cada dígito digitado
  lcd.setCursor(0, 1);  // Linha 2, coluna 0
  for (int i = 0; i < posicao; i++) lcd.print("* ");

  // Quando todos os 4 dígitos foram inseridos, verifica a senha
  if (posicao == 4) {
    delay(300);   // Pequena pausa antes de verificar
    verificar();
  }
}

// ----------------------------------------------------------------
//  verificar() — compara a tentativa com a senha correta
// ----------------------------------------------------------------
void verificar() {
  bool certa = true;  // Assume correto e testa cada dígito

  for (int i = 0; i < 4; i++) {
    if (tentativa[i] != senhaCorreta[i]) {
      certa = false;  // Qualquer dígito errado invalida a tentativa
      break;
    }
  }

  posicao = 0;  // Reseta o índice para a próxima tentativa

  if (certa) {
    abrirCofre();       // Senha correta: abre
  } else {
    erros++;            // Incrementa o contador de erros
    if (erros >= 3) {
      bloquear();       // 3 erros consecutivos: bloqueia o sistema
    } else {
      senhaErrada();    // Ainda tem tentativas: avisa e recomeça
    }
  }
}

// ----------------------------------------------------------------
//  abrirCofre() — ativa o servo, LEDs e mensagem de acesso liberado
// ----------------------------------------------------------------
void abrirCofre() {
  // Gira o servo para 90° — posição de aberto
  servo.write(90);

  // Feedback visual: verde acende, vermelho apaga
  digitalWrite(LED_VERDE,    HIGH);
  digitalWrite(LED_VERMELHO, LOW);

  // Feedback sonoro: dois bips ascendentes (tom de vitória)
  tone(BUZZER, 1500, 150);
  delay(200);
  tone(BUZZER, 2000, 150);

  // Exibe mensagem de cofre aberto e instrução para fechar
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  COFRE ABERTO!");
  lcd.setCursor(0, 1);
  lcd.print("B1 = fechar");

  // Aguarda o usuário pressionar BTN1 para fechar o cofre
  // (loop bloqueante — programa fica preso aqui até o botão ser pressionado)
  while (digitalRead(BTN1)) { }

  // Fecha o servo (retorna a 0°)
  servo.write(0);

  // Feedback de fechamento
  digitalWrite(LED_VERDE,    LOW);
  digitalWrite(LED_VERMELHO, HIGH);
  tone(BUZZER, 800, 200);

  erros = 0;     // Zera os erros após abertura bem-sucedida
  telaInicial(); // Volta à tela inicial
}

// ----------------------------------------------------------------
//  senhaErrada() — avisa sobre erro e exibe tentativas restantes
// ----------------------------------------------------------------
void senhaErrada() {
  // Feedback visual e sonoro de erro
  digitalWrite(LED_VERMELHO, HIGH);
  tone(BUZZER, 300, 400);   // Tom grave por 400 ms
  delay(400);
  digitalWrite(LED_VERMELHO, LOW);

  // Exibe mensagem com quantas tentativas ainda restam
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SENHA ERRADA!");
  lcd.setCursor(0, 1);
  lcd.print("Restam: ");
  lcd.print(3 - erros);    // Ex: erros=1 → "Restam: 2"

  delay(1500);   // Mantém a mensagem visível por 1,5 s
  telaInicial(); // Volta à tela inicial para nova tentativa
}

// ----------------------------------------------------------------
//  bloquear() — bloqueia o sistema por 10 segundos após 3 erros
// ----------------------------------------------------------------
void bloquear() {
  digitalWrite(LED_VERMELHO, HIGH);

  // Alarme sonoro: 6 bips rápidos para alertar sobre bloqueio
  for (int i = 0; i < 6; i++) {
    tone(BUZZER, 500, 200);
    delay(300);
  }

  // Countdown de 10 segundos no LCD
  for (int t = 10; t > 0; t--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("!! BLOQUEADO !!");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde: ");
    lcd.print(t);     // Exibe o tempo restante
    lcd.print("s");
    delay(1000);      // Aguarda 1 segundo antes de decrementar
  }

  // Fim do bloqueio: reseta erros e retorna ao normal
  erros = 0;
  digitalWrite(LED_VERMELHO, LOW);
  telaInicial();
}

// ----------------------------------------------------------------
//  telaInicial() — exibe a tela de boas-vindas / aguardando senha
// ----------------------------------------------------------------
void telaInicial() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOCKER INTELIG.");   // Título do sistema
  lcd.setCursor(0, 1);
  lcd.print("Digite a senha:");   // Instrução ao usuário

  // Estado padrão: LED verde apagado, LED vermelho aceso (aguardando)
  digitalWrite(LED_VERDE,    LOW);
  digitalWrite(LED_VERMELHO, HIGH);
}
