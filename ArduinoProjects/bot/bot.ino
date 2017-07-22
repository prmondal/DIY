/******************************************************************
 Project     : Obstacle Avoidance Robot
 Libraries   : Ping sensor, Adafruit motor shield
 Author      : Prasenjit Mondal
 Date        : 06/07/17
******************************************************************/

#include <NewPing.h>
#include <AFMotor.h>
#include <IRremote.h>

#define TRIGGER_PIN  A5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define OBSTACLE_DETECT_DISTANCE 5

#define LED_PIN A2

#define DC_MOTOR_1 4
#define DC_MOTOR_2 3
#define SPEED 200

#define IR_RECV_PIN A1
#define IR_POWER 0xC0000C
#define IR_MOVE_FORWARD 0xC00058
#define IR_MOVE_BACKWARD 0xC00059
#define IR_MOVE_LEFT 0xC0005A
#define IR_MOVE_RIGHT 0xC0005B

enum REMOTE_STATE {
  REMOTE_FORWARD,
  REMOTE_BACKWARD,
  REMOTE_LEFT,
  REMOTE_RIGHT
};

REMOTE_STATE remoteState;

#define PUSH_BTN_PIN A3

boolean isRemoteControlled = false;

enum ACTION {
  LEFT_ROTATION,
  RIGHT_ROTATION,
  MOVE_FORWARD,
  MOVE_BACKWARD
};

ACTION botAction;

unsigned int obstacleDistance;
unsigned long pingSensorDelay = 100; //ms
unsigned long pingSensorLastEpoch = 0;

unsigned long ledBlinkDelay = 100; //ms
unsigned long ledBlinkLastEpoch = 0;
int ledBlinkState = LOW;

unsigned long DEBOUNCE_DELAY = 200;
unsigned long pushBtnLastEpoch = 0;
int POWER_ON_STATE = LOW;

unsigned long irReceiveDelay = 1000; //ms
unsigned long irReceiveLastEpoch = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
AF_DCMotor leftWheel(DC_MOTOR_1, MOTOR34_64KHZ);
AF_DCMotor rightWheel(DC_MOTOR_2, MOTOR34_64KHZ);

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

void moveForward() {
  leftWheel.run(BACKWARD);
  rightWheel.run(BACKWARD);
  botAction = MOVE_FORWARD;
}

void moveBackward() {
  leftWheel.run(FORWARD);
  rightWheel.run(FORWARD);
  botAction = MOVE_BACKWARD;
}

void rotateLeft() {
  leftWheel.run(BACKWARD);
  rightWheel.run(FORWARD);
  botAction = LEFT_ROTATION;
}

void rotateRight() {
  leftWheel.run(FORWARD);
  rightWheel.run(BACKWARD);
  botAction = RIGHT_ROTATION;
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
  } else if(rand == 2) {
    rotateRight();
  }
}

void runPingSensor(unsigned long currentTime) {
  if(currentTime - pingSensorLastEpoch > pingSensorDelay) {
    obstacleDistance = sonar.ping_in();
    //Serial.println(obstacleDistance);
    pingSensorLastEpoch = currentTime;
  }
}

void stopPingSensor() {
 // sonar.timer_stop(); 
}

void runBot(unsigned long currentTime) {
  //if obstacle not detected (if ping sensor is not connected value is measured as 0 which allows to move the bot forward)
  if(obstacleDistance == 0 || obstacleDistance > OBSTACLE_DETECT_DISTANCE) {
    moveForward();
    
    //keep LED on
    switchOnIndicator();
  } else {
    rotate();
    blinkLED(currentTime);
  }
}

void checkBotState(unsigned long currentTime) {
  switch(remoteState) {
    case REMOTE_FORWARD:
      moveForward();
    break;

    case REMOTE_BACKWARD:
      moveBackward();
    break;

    case REMOTE_LEFT:
      rotateLeft();
    break;

    case REMOTE_RIGHT:
      rotateRight();
    break;
  }
}

void stopBot() {
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

void switchOnIndicator() {
  digitalWrite(LED_PIN, HIGH);
}

void switchOffIndicator() {
  digitalWrite(LED_PIN, LOW);
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

void setRemoteStateForIRCommand() {
  if(results.value == IR_MOVE_FORWARD) {
    remoteState = REMOTE_FORWARD;
  } else if(results.value == IR_MOVE_BACKWARD) {
    remoteState = REMOTE_BACKWARD;
  } else if(results.value == IR_MOVE_LEFT) {
    remoteState = REMOTE_LEFT;
  } else if(results.value == IR_MOVE_RIGHT) {
    remoteState = REMOTE_RIGHT;
  }
}

void checkIRReceiver(unsigned long currentTime) {
  if(currentTime - irReceiveLastEpoch > irReceiveDelay) {
    irReceiveLastEpoch = currentTime;

    if(irrecv.decode(&results)) {
      //Serial.println(results.value, HEX);

      //power on robot
      if(results.value == IR_POWER) {
        isRemoteControlled = true;
        togglePowerState();
      }

      //check IR command if the robot is powered on
      if(POWER_ON_STATE == HIGH) {
        setRemoteStateForIRCommand();
      }
    
      irrecv.resume();
    }
  }
}

void checkPushButtonState(unsigned long currentTime) {
  int pushBtnState = digitalRead(PUSH_BTN_PIN);
  
  if(currentTime - pushBtnLastEpoch > DEBOUNCE_DELAY) {
    //switch on at transition from LOW -> HIGH
    if(pushBtnState == HIGH) {
      isRemoteControlled = false;
      togglePowerState();
    }
    
    pushBtnLastEpoch = currentTime;
  }
}

void togglePowerState() {
  if(POWER_ON_STATE == LOW) {
    POWER_ON_STATE = HIGH;
  } else {
    POWER_ON_STATE = LOW;
    powerOff();
  }
}

void powerOff() {
  switchOffIndicator();
  stopPingSensor();
  stopBot();
}

void setup() {
  Serial.begin(9600);

  irrecv.enableIRIn();
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_BTN_PIN, INPUT);
  
  leftWheel.setSpeed(SPEED);
  rightWheel.setSpeed(SPEED);
  
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

void loop() {
  unsigned long currentTime = millis();
  
  checkPushButtonState(currentTime);
  checkIRReceiver(currentTime);
    
  if(POWER_ON_STATE == HIGH) {
    if(!isRemoteControlled) {
      //check obstacles
      runPingSensor(currentTime);
    
      //run robot's motor
      runBot(currentTime);
    } else {
      //keep LED on
      switchOnIndicator();

      //perform robot action depens on remote controller state
      checkBotState(currentTime);
    }
  }
}
