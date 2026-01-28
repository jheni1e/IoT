int leds[6] = { 12, 14, 27, 23, 18, 5 };
int totalLeds = 6;
int botaoPin = 4;

volatile int ledEscolhido = 0;
volatile int tempoPassado = 0;

hw_timer_t *timer = NULL;

int tempo = 0;
int flagLed = 0;
int sentidoHorario = 0;

void IRAM_ATTR onTimer() {
	tempoPassado++;
	if (flagLed == 1) {
		if (tempo > 0) {
			tempo--;
		} else {
			flagLed = 0;
			if (sentidoHorario == 0) {
				sentidoHorario = 1;
			} else {
				sentidoHorario = 0;
			}
		}
	}
}

void IRAM_ATTR ledFunction() {
	if (flagLed != 1) {
		flagLed = 1;
		tempo = 200;
	}
}

void setup() {
	for (int i = 0; i < totalLeds; i++) {
		pinMode(leds[i], OUTPUT);
		digitalWrite(leds[i], 0);
	}

	pinMode(botaoPin, INPUT_PULLUP);

	attachInterrupt(
	  digitalPinToInterrupt(botaoPin),
	  ledFunction,
	  FALLING);

	Serial.begin(115200);

	timer = timerBegin(1000000);
	timerAttachInterrupt(timer, &onTimer);
	timerAlarm(timer, 1000, true, 0);
}

void loop() {
	Serial.print("Sentido horário: ");
	Serial.println(sentidoHorario);

	if (sentidoHorario) {
		digitalWrite(leds[ledEscolhido], 1);
		if(tempoPassado > 500) {
			tempoPassado = 0;
			digitalWrite(leds[ledEscolhido], 0);
			if (ledEscolhido < totalLeds - 1) {
				ledEscolhido++;
			} else {
				ledEscolhido = 0;
			}
		}
	} else {
		digitalWrite(leds[ledEscolhido], 1);
		if(tempoPassado > 500) {
			tempoPassado = 0;
			digitalWrite(leds[ledEscolhido], 0);
			if (ledEscolhido > 0) {
				ledEscolhido--;
			} else {
				ledEscolhido = 5;
			}
		}
	}
}