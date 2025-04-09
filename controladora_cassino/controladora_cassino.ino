#include <Adafruit_NeoPixel.h>

#define LED_PIN_1 10  // Fita da esquerda
#define LED_PIN_2 11  // Fita horizontal do meio
#define LED_PIN_3 12  // Fita da direita

#define LED_COUNT_1 24  // Número de LEDs da Fita 1
#define LED_COUNT_2 34  // Número de LEDs da Fita 2
#define LED_COUNT_3 86  // Número de LEDs da Fita 3

#define TOTAL_LEDS (LED_COUNT_1 + LED_COUNT_2 + LED_COUNT_3)

Adafruit_NeoPixel strip_1(LED_COUNT_1, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2(LED_COUNT_2, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_3(LED_COUNT_3, LED_PIN_3, NEO_GRB + NEO_KHZ800);

int readSerial = 0;
int efeitoAtual = 0;
byte brilho = 130;
bool esperaSerial = true;

unsigned long ultimaTrocaRainbow = 0;
int posicaoCor = 0;
bool efeitoRainbowAtivo = false;
bool efeitoDegrade = false;

bool efeitoPiscarAtivo = false;
bool estadoPiscar = false;
unsigned long ultimaTrocaPiscar = 0;

bool efeitoRainbowStroboAtivo = false;

bool stroboAtivo = false;

bool modoEfeitos = false;
unsigned long lastEffectTime = 0;

int estado = 0;
int efeitoEtapa = 0;

void setup() {
  Serial.begin(9600);
  strip_1.begin();  //-- Inicia a fita saida 1
  strip_2.begin();  //-- Inicia a f0ita saida 2
  strip_3.begin();  //-- Inicia a fita saida 3
}

void loop() {
  if (Serial.available() > 0) {
    readSerial = Serial.read();
    esperaSerial = false;
  }

  if (esperaSerial) {
    orangeEffect();  // Mostra efeito laranja fixo
    return;          // Sai do loop para não fazer mais nada
  }

  serialRead();

  if (efeitoRainbowAtivo) {
    efeitoRainbow();
  }
  if (efeitoRainbowStroboAtivo) {
    efeitoRainbowStrobo();
  }
  if (efeitoPiscarAtivo) {
    efeitoPiscar();
  }
  if (efeitoAtual == 9) {
    efeitoRespiracaoDegrade();
  }
  if (efeitoAtual == 10) {
    efeitoAleatorio();
  }
  if (efeitoAtual == 11) {
    efeitoRadar();
  }

  if (modoEfeitos) {
    unsigned long currentTime = millis();
    if (currentTime - lastEffectTime >= 10000) {  // Muda a cada 10 segundos
      lastEffectTime = currentTime;
      efeitoAtual = (efeitoAtual + 1) % 10;  // Agora só alterna entre 3 efeitos
    }
    switch (efeitoAtual) {
      case 0: greenEffect(); break;
      case 1: efeitoAleatorio(); break;
      case 2: blueEffect(); break;
      case 3: efeitoRespiracaoDegrade(); break;
      case 4: redEffect(); break;
      case 5: efeitoRainbow(); break;
      case 6: pinkEffect(); break;
      case 7: orangeEffect(); break;
      case 8: cianEffect(); break;
      case 9: efeitoRadar(); break;
    }
  }
}

void serialRead() {

  if (readSerial == 'w') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    whiteEffect();
  }

  if (readSerial == 'a') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    redEffect();
  }
  if (readSerial == 'b') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    greenEffect();
  }
  if (readSerial == 'c') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    blueEffect();
  }
  if (readSerial == 'd') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    cianEffect();
  }
  if (readSerial == 'e') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    pinkEffect();
  }
  if (readSerial == 'f') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
    orangeEffect();
  }
  if (readSerial == 'g') {
    efeitoRainbow();
    efeitoRainbowAtivo = true;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    setBrilho(brilho);
  }
  if (readSerial == 'h') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    stroboAtivo = false;
    efeitoAtual = 9;
  }
  if (readSerial == 'i') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    stroboAtivo = false;
    modoEfeitos = false;
    efeitoAtual = 10;
  }
  if (readSerial == 'j') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    stroboAtivo = false;
    modoEfeitos = false;
    efeitoAtual = 11;
  }
  if (readSerial == 'l') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    stroboAtivo = false;
    modoEfeitos = true;
  }
  if (efeitoAtual == 7 && readSerial == 's') {
    efeitoRainbowStrobo();
    efeitoRainbowStroboAtivo = true;
    efeitoRainbowAtivo = false;
  }
  if (readSerial == 's') {
    efeitoPiscarAtivo = true;
  }
  if (readSerial == '1') {
    brilho = 25;
    setBrilho(brilho);
  }
  if (readSerial == '2') {
    brilho = 40;
    setBrilho(brilho);
  }
  if (readSerial == '3') {
    brilho = 55;
    setBrilho(brilho);
  }
  if (readSerial == '4') {
    brilho = 70;
    setBrilho(brilho);
  }
  if (readSerial == '5') {
    brilho = 85;
    setBrilho(brilho);
  }
  if (readSerial == '6') {
    brilho = 90;
    setBrilho(brilho);
  }
  if (readSerial == '7') {
    brilho = 105;
    setBrilho(brilho);
  }
  if (readSerial == '8') {
    brilho = 120;
    setBrilho(brilho);
  }
  if (efeitoAtual != 6 && readSerial == '9') {
    brilho = 130;
    setBrilho(brilho);
  }

  if (readSerial == '0') {
    efeitoRainbowAtivo = false;
    efeitoRainbowStroboAtivo = false;
    efeitoPiscarAtivo = false;
    modoEfeitos = false;
    efeitoAtual = 0;
    offEffect();
  }
}
void greenEffect() {
  efeitoAtual = 1;
  preencherLed(0, 0, brilho);
}
void blueEffect() {
  efeitoAtual = 2;
  preencherLed(0, brilho, 0);
}
void redEffect() {
  efeitoAtual = 3;
  preencherLed(brilho, 0, 0);
}
void cianEffect() {
  efeitoAtual = 4;
  preencherLed(0, brilho, brilho);
}
void pinkEffect() {
  efeitoAtual = 5;
  preencherLed(brilho, brilho, 0);
}
void orangeEffect() {
  efeitoAtual = 6;
  preencherLed(brilho, 0, brilho / 10);
}
void whiteEffect() {
  efeitoAtual = 16;
  preencherLed(255, 255, 255);
}
void offEffect() {
  preencherLed(0, 0, 0);
}

void efeitoPiscar() {
  unsigned long intervalo = 25;  // tempo de piscada (ms)

  if (millis() - ultimaTrocaPiscar >= intervalo) {
    ultimaTrocaPiscar = millis();
    estadoPiscar = !estadoPiscar;

    if (estadoPiscar) {
      setBrilho(10);  // Apaga os LEDs (brilho zero)
    } else {
      setBrilho(brilho);  // Volta ao brilho original
    }
  }
}

void efeitoRespiracaoDegrade() {
  efeitoAtual = 9;

  static unsigned long tempoAnterior = 0;
  static float brilhoF = 0;
  static int direcao = 1;

  static uint8_t corAtual[3] = { 130, 0, 0 };    // vermelho
  static uint8_t corProxima[3] = { 0, 0, 130 };  // azul
  static float tCor = 0.0;                       // interpolador de cor (0.0 a 1.0)

  const float passoBrilho = 1;
  const int brilhoMin = 10;
  const int brilhoMax = brilho;

  if (millis() - tempoAnterior > 5) {
    tempoAnterior = millis();

    brilhoF += direcao * passoBrilho;
    if (brilhoF >= brilhoMax) {
      brilhoF = brilhoMax;
      direcao = -1;
    } else if (brilhoF <= brilhoMin) {
      brilhoF = brilhoMin;
      direcao = 1;

      // Avança para a próxima cor ao fim de um ciclo
      corAtual[0] = corProxima[0];
      corAtual[1] = corProxima[1];
      corAtual[2] = corProxima[2];

      // Escolhe nova cor aleatória (ou da lista se quiser)
      uint8_t novasCores[6][3] = {
        { 130, 0, 0 },    // vermelho
        { 0, 130, 0 },    // verde
        { 0, 0, 130 },    // azul
        { 130, 130, 0 },  // amarelo
        { 0, 130, 130 },  // ciano
        { 130, 0, 130 }   // magenta
      };
      int index = random(0, 6);
      corProxima[0] = novasCores[index][0];
      corProxima[1] = novasCores[index][1];
      corProxima[2] = novasCores[index][2];
      tCor = 0.0;
    }

    // Atualiza interpolador de cor apenas ao subir o brilho
    if (direcao == 1 && brilhoMax != brilhoMin) {
      tCor = (brilhoF - brilhoMin) / (brilhoMax - brilhoMin);
    }

    // Interpola as cores
    uint8_t r = (1 - tCor) * corAtual[0] + tCor * corProxima[0];
    uint8_t g = (1 - tCor) * corAtual[1] + tCor * corProxima[1];
    uint8_t b = (1 - tCor) * corAtual[2] + tCor * corProxima[2];

    // Atualiza brilho e cor
    byte brilhoAtual = (byte)brilhoF;
    strip_1.setBrightness(brilhoAtual);
    strip_2.setBrightness(brilhoAtual);
    strip_3.setBrightness(brilhoAtual);

    for (int i = 0; i < strip_1.numPixels(); i++) strip_1.setPixelColor(i, r, g, b);
    for (int i = 0; i < strip_2.numPixels(); i++) strip_2.setPixelColor(i, r, g, b);
    for (int i = 0; i < strip_3.numPixels(); i++) strip_3.setPixelColor(i, r, g, b);

    strip_1.show();
    strip_2.show();
    strip_3.show();
  }
}

void efeitoAleatorio() {
  unsigned long tempoAnterior = 0;
  efeitoAtual = 10;
  for (int i = 0; i < strip_1.numPixels(); i++) strip_1.setPixelColor(i, 10, 10, 10);
  for (int i = 0; i < strip_2.numPixels(); i++) strip_2.setPixelColor(i, 10, 10, 10);
  for (int i = 0; i < strip_3.numPixels(); i++) strip_3.setPixelColor(i, 10, 10, 10);

  if (millis() - tempoAnterior > 20) {
    tempoAnterior = millis();

    strip_1.setPixelColor(random(0, 24), 0, 0, 130);
    strip_2.setPixelColor(random(0, 32), 0, 0, 130);
    strip_3.setPixelColor(random(0, 86), 0, 0, 130);
    strip_1.setPixelColor(random(0, 24), 0, 130, 0);
    strip_2.setPixelColor(random(0, 32), 0, 130, 0);
    strip_3.setPixelColor(random(0, 86), 0, 130, 0);
    delay(50);

    strip_1.show();
    strip_2.show();
    strip_3.show();
  }
}

unsigned long ultimaTroca3 = 0;
bool ledsAcesos = true;

void efeitoStrobo() {
  if (millis() - ultimaTroca3 > 20) {
    ultimaTroca3 = millis();

    if (ledsAcesos) {
      strip_1.setBrightness(10);
      strip_2.setBrightness(10);
      strip_3.setBrightness(10);
      strip_1.show();
      strip_2.show();
      strip_3.show();
    } else {
      strip_1.setBrightness(brilho);
      strip_2.setBrightness(brilho);
      strip_3.setBrightness(brilho);
      strip_1.show();
      strip_2.show();
      strip_3.show();
    }

    ledsAcesos = !ledsAcesos;
  }
}

void setBrilho(byte valor) {
  strip_1.setBrightness(valor);
  strip_2.setBrightness(valor);
  strip_3.setBrightness(valor);
  strip_1.show();
  strip_2.show();
  strip_3.show();
}

void efeitoRainbow() {
  efeitoAtual = 7;
  unsigned long intervalo = 5;
  if (millis() - ultimaTrocaRainbow >= intervalo) {
    ultimaTrocaRainbow = millis();

    for (int i = 0; i < strip_1.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_1.numPixels())) & 130;
      strip_1.setPixelColor(i, rodaCores(cor));
    }
    for (int i = 0; i < strip_2.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_2.numPixels())) & 130;
      strip_2.setPixelColor(i, rodaCores(cor));
    }
    for (int i = 0; i < strip_3.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_3.numPixels())) & 130;
      strip_3.setPixelColor(i, rodaCores(cor));
    }

    strip_1.show();
    strip_2.show();
    strip_3.show();

    posicaoCor = (posicaoCor + 1) % 256;
  }
}

void efeitoRainbowStrobo() {
  unsigned long intervalo = 5;
  if (millis() - ultimaTrocaRainbow >= intervalo) {
    ultimaTrocaRainbow = millis();

    for (int i = 0; i < strip_1.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_1.numPixels())) & 130;
      strip_1.setPixelColor(i, rodaCores(cor));
    }
    for (int i = 0; i < strip_2.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_2.numPixels())) & 130;
      strip_2.setPixelColor(i, rodaCores(cor));
    }
    for (int i = 0; i < strip_3.numPixels(); i++) {
      int cor = (posicaoCor + (i * 256 / strip_3.numPixels())) & 130;
      strip_3.setPixelColor(i, rodaCores(cor));
    }

    efeitoStrobo();

    posicaoCor = (posicaoCor + 1) % 256;
  }
}

uint32_t rodaCores(byte pos) {
  if (pos < 85) {
    return strip_1.Color(pos * 3, 255 - pos * 3, 0);
  } else if (pos < 170) {
    pos -= 85;
    return strip_1.Color(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return strip_1.Color(0, pos * 3, 255 - pos * 3);
  }
}

void preencherLed(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 24; i++) {
    strip_1.setPixelColor(i, r, g, b);
  }
  for (int i = 0; i < 32; i++) {
    strip_2.setPixelColor(i, r, g, b);
  }
  for (int i = 0; i < 86; i++) {
    strip_3.setPixelColor(i, r, g, b);
  }

  strip_1.show();
  strip_2.show();
  strip_3.show();
}

unsigned long ultimaAtualizacao = 0;

void efeitoRadar() {
  if (millis() - ultimaAtualizacao >= 25) {
    ultimaAtualizacao = millis();

    switch (estado) {
      case 0:
        if (efeitoEtapa < 24) {
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(0, 130, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(0, 130, 0));

          strip_1.show();
          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 1:
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(0, 130, 0));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(0, 130, 0));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 2:
        if (efeitoEtapa < 11) {
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(0, 130, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(0, 130, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 3:
        if (efeitoEtapa <= (50 - 11) / 2) {
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(0, 130, 0));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(0, 130, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 10;
          efeitoEtapa = 0;
        }
        break;

      case 10:
        if (efeitoEtapa < 24) {
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(0, 0, 130));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(0, 0, 130));

          strip_1.show();
          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 11:
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(0, 0, 130));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(0, 0, 130));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 12:
        if (efeitoEtapa < 11) {
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(0, 0, 130));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(0, 0, 130));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 13:
        if (efeitoEtapa <= (50 - 11) / 2) {
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(0, 0, 130));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(0, 0, 130));


          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 20;
          efeitoEtapa = 0;
        }
        break;

      case 20:
        if (efeitoEtapa < 24) {
          strip_1.setPixelColor(LED_COUNT_1 - 1 - efeitoEtapa, strip_1.Color(130, 0, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - efeitoEtapa, strip_3.Color(130, 0, 0));

          strip_1.show();
          strip_3.show();

          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 21:
        if (efeitoEtapa < LED_COUNT_2 / 2) {
          strip_2.setPixelColor(efeitoEtapa, strip_2.Color(130, 0, 0));
          strip_2.setPixelColor(LED_COUNT_2 - 1 - efeitoEtapa, strip_2.Color(130, 0, 0));

          strip_2.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 22:
        if (efeitoEtapa < 11) {
          strip_3.setPixelColor(efeitoEtapa, strip_3.Color(130, 0, 0));
          strip_3.setPixelColor(LED_COUNT_3 - 1 - (24 + efeitoEtapa), strip_3.Color(130, 0, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          efeitoEtapa = 0;
          estado++;
        }
        break;

      case 23:
        if (efeitoEtapa <= (50 - 11) / 2) {
          strip_3.setPixelColor(11 + efeitoEtapa, strip_3.Color(130, 0, 0));
          strip_3.setPixelColor(50 - efeitoEtapa, strip_3.Color(130, 0, 0));

          strip_3.show();
          efeitoEtapa++;
        } else {
          estado = 0;
          efeitoEtapa = 0;
        }
        break;
    }
  }
}
