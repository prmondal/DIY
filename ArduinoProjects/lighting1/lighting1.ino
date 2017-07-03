int redLeds = 9;
int greenLeds = 10;
int blueLeds = 11;

int seq1_time = 100;
int seq2_time = 100;
int seq3_time = 150;
int seq4_time = 30;
int seq5_time = 500;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(redLeds, OUTPUT);
  pinMode(greenLeds, OUTPUT);
  pinMode(blueLeds, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //seq1
  digitalWrite(redLeds, HIGH);   
  delay(seq1_time);               
  digitalWrite(redLeds, LOW);    
  delay(seq1_time);
  
  digitalWrite(greenLeds, HIGH);   
  delay(seq1_time);               
  digitalWrite(greenLeds, LOW);    
  delay(seq1_time);
  
  digitalWrite(blueLeds, HIGH);   
  delay(seq1_time);               
  digitalWrite(blueLeds, LOW);    
  delay(seq1_time);
  
  //seq2
  digitalWrite(redLeds, HIGH);
  digitalWrite(blueLeds, HIGH);
  delay(seq2_time);               
  digitalWrite(redLeds, LOW);
  digitalWrite(blueLeds, LOW);
  delay(seq2_time);
  
  digitalWrite(greenLeds, HIGH);   
  delay(seq2_time);               
  digitalWrite(greenLeds, LOW);    
  delay(seq2_time);
  
  //seq3
  digitalWrite(blueLeds, HIGH);   
  delay(seq3_time);               
  digitalWrite(blueLeds, LOW);    
  delay(seq3_time);
  
  digitalWrite(greenLeds, HIGH);   
  delay(seq3_time);               
  digitalWrite(greenLeds, LOW);    
  delay(seq3_time);
  
  digitalWrite(redLeds, HIGH);   
  delay(seq3_time);               
  digitalWrite(redLeds, LOW);    
  delay(seq3_time);

  //seq4
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(redLeds, fadeValue);
    analogWrite(greenLeds, fadeValue);
    analogWrite(blueLeds, fadeValue);
    delay(seq4_time);
  }

  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(redLeds, fadeValue);
    analogWrite(greenLeds, fadeValue);
    analogWrite(blueLeds, fadeValue);
    delay(seq4_time);
  }

  //seq 5
  digitalWrite(blueLeds, HIGH);
  digitalWrite(greenLeds, HIGH);
  digitalWrite(redLeds, HIGH);
  delay(seq5_time);               
  digitalWrite(blueLeds, LOW);
  digitalWrite(greenLeds, LOW);
  digitalWrite(redLeds, LOW);    
  delay(seq5_time);

  digitalWrite(blueLeds, HIGH);
  digitalWrite(greenLeds, HIGH);
  digitalWrite(redLeds, HIGH);
  delay(seq5_time);               
  digitalWrite(blueLeds, LOW);
  digitalWrite(greenLeds, LOW);
  digitalWrite(redLeds, LOW);    
  delay(seq5_time);
}
