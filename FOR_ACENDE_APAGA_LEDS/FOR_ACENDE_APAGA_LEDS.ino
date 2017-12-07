//int pinled = 12
int pinled[9] = {12, 11, 10, 9, 8, 7, 6, 5, 4,};
int x;
int numeroLed;

void setup() {
  //pinMode(12, OUTPUT);
  for (x = 0 ; x <= 8; x = x++) {
    pinMode(pinled[x], OUTPUT);

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (numeroLed = 0 ; numeroLed <= 8 ; numeroLed = numeroLed++) {
    digitalWrite(pinled[numeroLed], HIGH);
    delay(200);
  }
  for (numeroLed = 8 ; numeroLed >= 0 ; numeroLed = numeroLed--) {
    digitalWrite(pinled[numeroLed], LOW);
    delay(200);
  }
}
