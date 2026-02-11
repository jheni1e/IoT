int botaoPin = 4;

volatile int botao = 0;
hw_timer_t *timer = NULL;

int flagBotao = 0;
int tempo = 0;

void IRAM_ATTR onTimer() {
	if(flagBotao == 1) {
		if (tempo > 0) {
			tempo--;
		} else {
			flagBotao = 0;
			if (digitalRead(botaoPin)) {
				botao++;
				Serial.print("Valor do botão: ");
				Serial.println(botao);
			}
		}
	}
}

// boa prática de função de interrupção é ser simples
void IRAM_ATTR buttonFunction() {
	if (flagBotao != 1) {
		flagBotao = 1;
		tempo = 200;
	}
}

void setup() {
	pinMode(botaoPin, INPUT_PULLUP);      

	attachInterrupt(
		digitalPinToInterrupt(botaoPin), // pin escolhido
		buttonFunction, // funcao IRAM_ATTR
		FALLING); // falling, rising, change

  Serial.begin(115200);

	timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);                          
  timerAlarm(timer, 1000, true, 0);
}

void loop() {
}