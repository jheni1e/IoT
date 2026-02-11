int soma = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(soma);
  delay(10000);
  Serial.println("Foi 10s");
  soma += 1;
}