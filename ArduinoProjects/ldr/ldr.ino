int analogInp = A0;
int ledPin = 13;
int ldrLevel = 25;

void setup() {
  pinMode(analogInp, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(analogInp);
  Serial.println(value);

  if(value < ldrLevel) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
