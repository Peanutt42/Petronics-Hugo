#define PIN_LDR A0
#define PIN_LED 2

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  int intensity = analogRead(PIN_LDR);
  digitalWrite(PIN_LED, intensity < 500);
  Serial.println(intensity);
}
