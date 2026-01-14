const int trigger = 4;
const int echo = 16;

float duracao, distancia;

void setup() {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duracao = pulseIn(echo, HIGH);
  distancia = (duracao * 0.0343) / 2;
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println("cm");
  delay(500);
}