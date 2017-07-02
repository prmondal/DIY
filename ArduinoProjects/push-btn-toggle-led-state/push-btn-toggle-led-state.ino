int ledPin = 13;
int pushBtnInp = 2;
boolean state = LOW;

void setup() {
  pinMode(pushBtnInp, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if(digitalRead(pushBtnInp) == LOW) {
    delay(200); //delay is added to delay the transition to avoid push button bouncing effect
    state = !state;
  }
  
  digitalWrite(ledPin, state);
}
