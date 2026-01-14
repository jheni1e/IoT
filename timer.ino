hw_timer_t *timer = NULL; //seta um ponteiro nulo do tipo hw_timer_t
int tempo = 0; //tempo comeca como 0

void IRAM_ATTR onTimer() { //codigo de interrupcao rapida, sem atrasos de carregamento da memoria flash
  tempo++; //tempo incrementa
}

void setup() {
  Serial.begin(9600); //inicializa o monitor serial
  timer = timerBegin(1000000); //preenche a variavel timer com 1 segundo/1MHz
  timerAttachInterrupt(timer, &onTimer); //cria a interrupcao, implicitamente usando o ponteiro, usa espaço da memoria como timer
  timerAlarm(timer, 10000000, true, 0); //ativa o alarme do timer: a cada 1 segundos, o onTimer() vai ser executado
}

void loop() {
  Serial.println("Tempo: " + String(tempo));
}