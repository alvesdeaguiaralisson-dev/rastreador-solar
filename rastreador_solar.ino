#include <Servo.h>

#define PINO_SERVO_H  5 // Servo horizontal
#define PINO_SERVO_V  6 // Servo vertical

bool flag_operacao = true; // Flag de controle de operação

int tolerancia = 100;   // Faixa de resposta da iluminância (10~100)
int tempo_atraso = 100; // Parâmetro de atraso em milissegundos (10~100)

// Configurações do Servo Horizontal
Servo horizontal;
int servoh = 90;             // Ângulo de inicialização
int servohLimitHigh = 180;   // Ângulo máximo de rotação na direção horizontal
int servohLimitLow = 0;      // Ângulo mínimo de rotação na direção horizontal

// Configurações do Servo Vertical
Servo vertical;
int servov = 90;             // Ângulo de inicialização ajustado
int servovLimitHigh = 180;   // Ângulo máximo de rotação na direção vertical
int servovLimitLow = 90;     // Ângulo mínimo de rotação na direção vertical

// 4 portas de conexão para os módulos fotorresistores (LDR)
const int ldrlt = A0;   // Superior esquerdo (Left Top)
const int ldrrt = A1;   // Superior direito (Right Top)
const int ldrld = A2;   // Inferior esquerdo (Left Down)
const int ldrrd = A3;   // Inferior direito (Right Down)

void setup() {
  horizontal.attach(PINO_SERVO_H);
  vertical.attach(PINO_SERVO_V);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);
  Serial.begin(9600);
}

void loop() {
  // Se a flag for falsa, interrompe a execução do rastreamento
  if (!flag_operacao) {
    return;
  }

  // Lê os valores de iluminância dos 4 módulos fotorresistores
  int lt = analogRead(ldrlt); // Superior esquerdo
  int rt = analogRead(ldrrt); // Superior direito
  int ld = analogRead(ldrld); // Inferior esquerdo
  int rd = analogRead(ldrrd); // Inferior direito

  Serial.print("lt:"); Serial.print(lt); Serial.print(" ");
  Serial.print("rt:"); Serial.print(rt); Serial.print(" ");
  Serial.print("ld:"); Serial.print(ld); Serial.print(" ");
  Serial.print("rd:"); Serial.println(rd);

  // Calcula as médias das leituras dos módulos adjacentes
  int avt = (lt + rt) / 2; // Média superior
  int avd = (ld + rd) / 2; // Média inferior
  int avl = (lt + ld) / 2; // Média esquerda
  int avr = (rt + rd) / 2; // Média direita

  int veg = (avt + avd + avl + avr) / 4; // Média global

  // Ajusta a sensibilidade e o atraso conforme a intensidade da luz
  if (veg > 0 && veg < 300) {
    tolerancia = map(veg, 10, 300, 5, 100);
    tempo_atraso = map(veg, 10, 300, 100, 50);
  } else {
    tolerancia = 50;
    tempo_atraso = 50;
  }

  Serial.print("veg= "); Serial.println(veg);
  Serial.print("tol= "); Serial.println(tolerancia);
  Serial.print("dtime= "); Serial.println(tempo_atraso);

  // Calcula a diferença entre as linhas (superior/inferior e esquerda/direita)
  int dvert = avt - avd;    // Diferença vertical
  int dhoriz = avl - avr;   // Diferença horizontal

  // Verifica se a diferença vertical excede a tolerância para alterar o ângulo
  if (abs(dvert) > tolerancia) {
    if (avt > avd) {
      servov++;
      if (servov > servovLimitHigh) {
        servov = servovLimitHigh;
      }
    } else if (avt < avd) {
      servov--;
      if (servov < servovLimitLow) {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov);
  }

  // Verifica se a diferença horizontal excede a tolerância para alterar o ângulo
  if (abs(dhoriz) > tolerancia) {
    if (avl < avr) {
      servoh--;
      if (servoh < servohLimitLow) {
        servoh = servohLimitLow;
      }
    } else if (avl > avr) {
      servoh++;
      if (servoh > servohLimitHigh) {
        servoh = servohLimitHigh;
      }
    }
    horizontal.write(servoh);
  }

  delay(tempo_atraso);
}
