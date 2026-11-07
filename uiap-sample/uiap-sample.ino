#define LED_BUILTIN 2

void flashLed(unsigned long duration) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(duration);
  digitalWrite(LED_BUILTIN, LOW);
  delay(150);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // Blinkとは違うことが分かるように、短く2回点滅します。
  flashLed(120);
  flashLed(120);
  delay(1000);
}
