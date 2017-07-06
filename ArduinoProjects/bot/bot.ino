/******************************************************************
 Project     : Obstacle Avoidance Robot
 Libraries   : Ping sensor, Adafruit motor shield
 Author      : Prasenjit Mondal
 Date        : 06/07/17
******************************************************************/

#include <NewPing.h>
#include <AFMotor.h>

#define TRIGGER_PIN  A5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define OBSTACLE_DETECT_DISTANCE 5

#define LED_PIN A2

#define DC_MOTOR_1 4
#define DC_MOTOR_2 3
#define SPEED 200

#define PUSH_BTN_PIN A3

enum ACTION {
  LEFT_ROTATION, RIGHT_ROTATION, MOVE_FORWARD, MOVE_BACKWARD
};

unsigned int obstacleDistance;

unsigned long pingSensorDelay = 100; //ms
unsigned long pingSensorLastEpoch = 0;

unsigned long ledBlinkDelay = 100; //ms
unsigned long ledBlinkLastEpoch = 0;
int ledBlinkState = LOW;

unsigned long DEBOUNCE_DELAY = 200;
unsigned long pushBtnLastEpoch = 0;
int POWER_ON_STATE = LOW;

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
  //check last rotation status and continue rotation
  if(botAction == LEFT_ROTATION) {
    rotateLeft();
    return;
  }
  
  if(botAction == RIGHT_ROTATION) {
    rotateRight();
    return;
  }
  
  //rotate left or right (randomly) to avoid obstacle
  int rand = random(1, 3);

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
  obstacleDistance = sonar.ping_in();
  //Serial.println(obstacleDistance);
}

void stopPingSensor() {
  sonar.timer_stop(); 
}

void runBot(unsigned long currentTime) {
  //if obstacle not detected (if ping sensor is not connected value is measured as 0 which allows to move the bot forward)
  if(obstacleDistance == 0 || obstacleDistance > OBSTACLE_DETECT_DISTANCE) {
    moveForward();
    botAction = MOVE_FORWARD;
    
    //keep LED on
    digitalWrite(LED_PIN, HIGH);
  } else {
    rotate();
    blinkLED(currentTime);
  }
}

void stopBot() {
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

void blinkLED(unsigned long currentTime) {
  if(currentTime - ledBlinkLastEpoch > ledBlinkDelay) {
    if(ledBlinkState == HIGH) {
      ledBlinkState = LOW;
    } else {
      ledBlinkState = HIGH;
    }
    
    digitalWrite(LED_PIN, ledBlinkState);
    ledBlinkLastEpoch = currentTime;
  }
}

void powerOff() {
  digitalWrite(LED_PIN, LOW);
  stopPingSensor();
  stopBot();
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
  int pushBtnState = digitalRead(PUSH_BTN_PIN);
  unsigned long currentTime = millis();
  
  if(currentTime - pushBtnLastEpoch > DEBOUNCE_DELAY) {
    //switch on at transition from LOW -> HIGH
    if(pushBtnState == HIGH) {
      if(POWER_ON_STATE == LOW) {
        POWER_ON_STATE = HIGH;
      } else {
        POWER_ON_STATE = LOW;
        powerOff();
      }
    }

    pushBtnLastEpoch = currentTime;
  }

  if(POWER_ON_STATE == HIGH) {
    //check obstacles
    if(currentTime - pingSensorLastEpoch > pingSensorDelay) {
      runPingSensor();
      pingSensorLastEpoch = currentTime;
    }
    
    //run robot's motor
    runBot(currentTime);
  }
}
