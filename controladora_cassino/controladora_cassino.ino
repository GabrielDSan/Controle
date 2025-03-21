#include <Adafruit_NeoPixel.h>

#define LED_PIN_1 10  // Fita da esquerda
#define LED_PIN_2 11  // Fita horizontal do meio
#define LED_PIN_3 12  // Fita da direita

#define LED_COUNT_1 24  // Número de LEDs da Fita 1
#define LED_COUNT_2 32  // Número de LEDs da Fita 2
#define LED_COUNT_3 86  // Número de LEDs da Fita 3

#define TOTAL_LEDS (LED_COUNT_1 + LED_COUNT_2 + LED_COUNT_3)

#define Data_1 5  //-- Entrada digital
#define Data_2 6  //-- Entrada digital
#define Data_3 7  //-- Entrada digital
#define Data_4 4  //-- Entrada digital

int dataState_1 = 0;      //-- Estado da entrada 1 (LOW/HIGH)
int dataState_2 = 0;      //-- Estado da entrada 2 (LOW/HIGH)
int dataState_3 = 0;      //-- Estado da entrada 3 (LOW/HIGH)
int dataState_4 = 0;      //-- Estado da entrada 4 (LOW/HIGH)
int dataPrevState_1 = 0;  //-- Estado anterior da entrada 1
int dataPrevState_2 = 0;  //-- Estado anterior da entrada 2
int dataPrevState_3 = 0;  //-- Estado anterior da entrada 3
int dataPrevState_4 = 0;  //-- Estado anterior da entrada 4

Adafruit_NeoPixel strip_1(LED_COUNT_1, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2(LED_COUNT_2, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_3(LED_COUNT_3, LED_PIN_3, NEO_GRB + NEO_KHZ800);

int estado = 0;
int efeitoEtapa = 0;
bool efeitoAtivo = false;
unsigned long ultimaAtualizacao = 0;
const int intervalo = 25;
int ondaOffset = 0;

void setup() {
  strip_1.begin();  //-- Inicia a fita saida 1
  strip_2.begin();  //-- Inicia a fita saida 2
  strip_3.begin();  //-- Inicia a fita saida 3

  pinMode(Data_1, INPUT);  //-- Define o pino 1 digital como entrada (Usar ressitores de 10K para pull-down)
  pinMode(Data_2, INPUT);  //-- Define o pino 2 digital como entrada (Usar ressitores de 10K para pull-down)
  pinMode(Data_3, INPUT);  //-- Define o pino 3 digital como entrada (Usar ressitores de 10K para pull-down)
  pinMode(Data_4, INPUT);  //-- Define o pino 4 digital como entrada (Usar ressitores de 10K para pull-down)
}

void loop() {
  Actions();
  ReadData();
}

void Actions() {
  if (dataState_2 == HIGH) {
    efeitoRadar();
  } else if (dataState_3 == HIGH) {
    efeitoRainbowOnda();
  } else if (dataState_4 == HIGH) {
    efeitoRandom();
  } else {
    estado = 0;  // Reinicia o efeito do começo
    efeitoEtapa = 0;
    efeitoAtivo = false;
    clearAll();  // Reseta a variável após o efeito ser executado
  }
  if (dataState_1 == HIGH) {
    efeitoStrobo();
  } else {
    strip_1.setBrightness(255);
    strip_2.setBrightness(255);
    strip_3.setBrightness(255);
  }
}

void ReadData() {
  dataState_1 = digitalRead(Data_1);
  dataState_2 = digitalRead(Data_2);
  dataState_3 = digitalRead(Data_3);
  dataState_4 = digitalRead(Data_4);

  if (dataState_1 != dataPrevState_1) {
    dataPrevState_1 = dataState_1;
  }
  if (dataState_2 != dataPrevState_2) {
    dataPrevState_2 = dataState_2;
  }
  if (dataState_3 != dataPrevState_3) {
    dataPrevState_3 = dataState_3;
  }
  if (dataState_4 != dataPrevState_4) {
    dataPrevState_4 = dataState_4;
  }
}

void efeitoRadar() {
  if (millis() - ultimaAtualizacao >= intervalo) {
    ultimaAtualizacao = millis();

    switch (estado) {
      case 0:  // Fita 1 e Fita 3 sobem até metade (24 pixels)
        if (efeitoEtapa < 24) {
          // Acende em verde
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(0, 255, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(0, 255, 0));

          strip_1.show();
          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;  // Próxima etapa
        }
        break;

      case 1:  // Fita 2 fecha dos dois lados
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          // Acende em verde
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(0, 255, 0));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(0, 255, 0));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 2:  // Fita 3 sobe mais 11 pixels e desce invertido
        if (efeitoEtapa < 11) {
          // Acende em verde
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(0, 255, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(0, 255, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 3:  // Fita 3 fecha do pixel 11 até o 50 pelos dois lados
        if (efeitoEtapa <= (50 - 11) / 2) {
          // Acende em verde
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(0, 255, 0));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(0, 255, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 10;  // Muda para a fase 10 para acender em vermelho
          efeitoEtapa = 0;
        }
        break;

      case 10:  // Fase de acender em vermelho
        if (efeitoEtapa < 24) {
          // Acende em vermelho
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(0, 0, 255));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(0, 0, 255));

          strip_1.show();
          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;  // Próxima etapa
        }
        break;

      case 11:  // Fita 2 fecha dos dois lados em vermelho
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          // Acende em vermelho
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(0, 0, 255));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(0, 0, 255));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 12:  // Fita 3 sobe mais 11 pixels e desce invertido em vermelho
        if (efeitoEtapa < 11) {
          // Acende em vermelho
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(0, 0, 255));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(0, 0, 255));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 13:  // Fita 3 fecha do pixel 11 até o 50 pelos dois lados em vermelho
        if (efeitoEtapa <= (50 - 11) / 2) {
          // Acende em vermelho
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(0, 0, 255));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(0, 0, 255));

          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 20;  // Volta ao início
          efeitoEtapa = 0;
        }
        break;

        case 20:  // Fase de acender em vermelho
        if (efeitoEtapa < 24) {
          // Acende em vermelho
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(255, 0, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(255, 0, 0));

          strip_1.show();
          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;  // Próxima etapa
        }
        break;

      case 21:  // Fita 2 fecha dos dois lados em vermelho
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          // Acende em vermelho
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(255, 0, 0));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(255, 0, 0));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 22:  // Fita 3 sobe mais 11 pixels e desce invertido em vermelho
        if (efeitoEtapa < 11) {
          // Acende em vermelho
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(255, 0, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(255, 0, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 23:  // Fita 3 fecha do pixel 11 até o 50 pelos dois lados em vermelho
        if (efeitoEtapa <= (50 - 11) / 2) {
          // Acende em vermelho
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(255, 0, 0));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(255, 0, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 0;  // Volta ao início
          efeitoEtapa = 0;
        }
        break;
    }
  }
}

void efeitoRandom() {
  if (millis() - ultimaAtualizacao >= 2) {
    ultimaAtualizacao = millis();
    if (efeitoEtapa < TOTAL_LEDS) {
      strip_1.setPixelColor(random(0, 24), strip_1.Color(0, 0, random(0, 255)));
      strip_2.setPixelColor(random(0, 32), strip_3.Color(0, 0, random(0, 255)));
      strip_3.setPixelColor(random(0, 86), strip_3.Color(0, 0, random(0, 255)));
      strip_1.setPixelColor(random(0, 24), strip_1.Color(255, 255, 255));
      strip_2.setPixelColor(random(0, 32), strip_3.Color(255, 255, 255));
      strip_3.setPixelColor(random(0, 86), strip_3.Color(255, 255, 255));
    }
    strip_1.show();
    strip_2.show();
    strip_3.show();
  }
}

unsigned long ultimaTroca3 = 0;
bool ledsAcesos = true;

void efeitoStrobo() {
  // Strobo: alterna o brilho entre 0 e 255 de todos os LEDs
  if (millis() - ultimaTroca3 >= 50) {
    ultimaTroca3 = millis();

    if (ledsAcesos) {
      // Apaga todos os LEDs (brilho 0)
      strip_1.setBrightness(10);
      strip_2.setBrightness(10);
      strip_3.setBrightness(10);
    } else {
      // Acende todos os LEDs (brilho 255)
      strip_1.setBrightness(255);
      strip_2.setBrightness(255);
      strip_3.setBrightness(255);
    }
    strip_1.show();
    strip_2.show();
    strip_3.show();

    // Alterna o estado de brilho (acende ou apaga)
    ledsAcesos = !ledsAcesos;
  }
}

void efeitoRainbowOnda() {
  if (millis() - ultimaAtualizacao >= intervalo) {
    ultimaAtualizacao = millis();

    // Cria a onda de cor para cada fita
    ondaOffset++;  // Desloca a onda

    // Fita 1: aplica o efeito de onda
    for (int i = 0; i < LED_COUNT_1; i++) {
      uint32_t cor = Wheel((i + ondaOffset) % 255);  // Cor baseada na posição com offset
      strip_1.setPixelColor(i, cor);
    }

    // Fita 2: aplica o efeito de onda
    for (int i = 0; i < LED_COUNT_2; i++) {
      uint32_t cor = Wheel((i + ondaOffset) % 255);  // Cor baseada na posição com offset
      strip_2.setPixelColor(i, cor);
    }

    // Fita 3: aplica o efeito de onda
    for (int i = 0; i < LED_COUNT_3; i++) {
      uint32_t cor = Wheel((i + ondaOffset) % 255);  // Cor baseada na posição com offset
      strip_3.setPixelColor(i, cor);
    }

    // Exibe as mudanças nas fitas de LED
    strip_1.show();
    strip_2.show();
    strip_3.show();
  }
}

uint32_t Wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return strip_1.Color(255 - pos * 3, 0, pos * 3);
  }
  if (pos < 170) {
    pos -= 85;
    return strip_1.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return strip_1.Color(pos * 3, 255 - pos * 3, 0);
}

void clearAll() {
  strip_1.clear();
  strip_2.clear();
  strip_3.clear();

  strip_1.show();
  strip_2.show();
  strip_3.show();
}