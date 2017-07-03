#include <NewPing.h>
#include <AFMotor.h>

#define TRIGGER_PIN  A5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define LED_PIN A2

#define OBSTACLE_DETECT_DISTANCE 5
unsigned int obstacleDistance;

#define DC_MOTOR_1 4
#define DC_MOTOR_2 3
#define SPEED 200

#define PUSH_BTN_PIN A3
#define DEBOUNCE_DELAY 200
long lastTime = 0;
int POWER_ON_STATE = LOW;

enum ACTION {
  LEFT_ROTATION, RIGHT_ROTATION, MOVE_FORWARD, MOVE_BACKWARD
};

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
AF_DCMotor leftWheel(DC_MOTOR_1, MOTOR34_64KHZ);
AF_DCMotor rightWheel(DC_MOTOR_2, MOTOR34_64KHZ);
ACTION botAction;

void moveBackward() {
  leftWheel.run(FORWARD);
  rightWheel.run(FORWARD);
}

void moveForward() {
  leftWheel.run(BACKWARD);
  rightWheel.run(BACKWARD);
}

void rotateLeft() {
  leftWheel.run(BACKWARD);
  rightWheel.run(FORWARD);
}

void rotateRight() {
  leftWheel.run(FORWARD);
  rightWheel.run(BACKWARD);
}

void rotate() {
  //check last rotation status and keep continue doing that
  if(botAction == LEFT_ROTATION) {
    rotateLeft();
    return;
  }
  
  if(botAction == RIGHT_ROTATION) {
    rotateRight();
    return;
  }
  
  //rotate left or right (randomly) to avoid obstacle
  int rand = random(1,3);

  //rand=1 -> rotate left
  //rand=2 -> rotate right
  if(rand == 1) {
    rotateLeft();
    botAction = LEFT_ROTATION;
  } else if(rand == 2) {
    rotateRight();
    botAction = RIGHT_ROTATION;
  }
}

void runPingSensor() {
  //Serial.println("run ping sensor");
  obstacleDistance = sonar.ping_in();
  //Serial.println(obstacleDistance);
}

void stopPingSensor() {
  //Serial.println("stop ping sensor");
  sonar.timer_stop(); 
}

void runBot() {
  //Serial.println("run bot");
  //if obstacle not detected (if ping sensor is not connected value is measured 0 which allows to move the bot forward)
  if(obstacleDistance > OBSTACLE_DETECT_DISTANCE) {
    //digitalWrite(LED_PIN, LOW);
    moveForward();
    botAction = MOVE_FORWARD;
  } else {
    //digitalWrite(LED_PIN, HIGH);
    rotate();
  }
}

void stopBot() {
  //Serial.println("stop bot");
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_BTN_PIN, INPUT);
  
  leftWheel.setSpeed(SPEED);
  rightWheel.setSpeed(SPEED);
  
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

void loop() {
  delay(100); //delay for ping sensor, min delay for ping sensor is 29ms
  
  int pushBtnState = digitalRead(PUSH_BTN_PIN);
  long currentTime = millis();
  
  if(currentTime - lastTime > DEBOUNCE_DELAY) {
    //switch on at transition from LOW -> HIGH
    if(pushBtnState == HIGH) {
      if(POWER_ON_STATE == LOW) {
        POWER_ON_STATE = HIGH;
      } else {
        POWER_ON_STATE = LOW;
        
        digitalWrite(LED_PIN, LOW);
        stopPingSensor();
        stopBot();
      }
    }

    lastTime = currentTime;
  }

  if(POWER_ON_STATE == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    runPingSensor();
    runBot();
  }
}

