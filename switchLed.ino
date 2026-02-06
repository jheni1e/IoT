hw_timer_t *timer = NULL;
hw_timer_t *timer2 = NULL;

volatile int tempo = 0;
int flag = 0;
int ledPin = 22;

void IRAM_ATTR onTimer() {
  tempo++;
}

void IRAM_ATTR switchLed() {
  digitalWrite(ledPin, !digitalRead(ledPin));
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 1);

  Serial.begin(115200);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 100000, true, 0);

  timer2 = timerBegin(1000000);
  timerAttachInterrupt(timer2, &switchLed);
  timerAlarm(timer2, 100000, true, 0);
}

void loop() {
  if (tempo >= 500) {
    tempo = 0;
    flag++;
    if (flag % 2 == 0) {
      timerAlarm(timer2, 1000000, true, 0);
    } else if (flag % 2 == 1) {
      timerAlarm(timer2, 100000, true, 0);
    }
  }
}
