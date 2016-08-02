int ledPin = 13;
int pushBtnInp = 2;

void setup() {
  pinMode(pushBtnInp, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //button released
  if (digitalRead(pushBtnInp) == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
