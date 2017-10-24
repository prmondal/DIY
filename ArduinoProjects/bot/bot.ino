/******************************************************************
 Project     : Smart Robot - Robot can be controlled by remote control or push button and 
               can be moved autonomously or manually by remote control.
               
               For this project I used Tata Sky remote control.
               
               Remote Control Button Settings:
                Switch On/Off - Power On Button
                Move Forward - Up Button
                Move Backward - Down Button
                Move Left - Left Button
                Move Right - Right Button
                Mode Change (Manual/Autonomous) - TV Button

               In autonomous mode robot runs freely without colliding with obstacles.
               
 Libraries   : Ping sensor, Adafruit motor shield, IR remote
 Author      : Prasenjit Mondal
 Date        : 06/07/17
******************************************************************/
#define REMOTE_ENABLED true

#include "NewPing.h"
#include "AFMotor.h"

#if REMOTE_ENABLED
  #include "IRremote.h"
#endif

#define TRIGGER_PIN  A5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define OBSTACLE_DETECT_DISTANCE 24 //inch

#define LED_PIN A2
#define PUSH_BTN_PIN A3

#define DC_MOTOR_1 4
#define DC_MOTOR_2 3
#define SPEED 150

#if REMOTE_ENABLED
  #define IR_RECV_PIN A1
  #define IR_POWER 0xC0000C //POWER Btn
  #define IR_ROBOT_MODE_CHANGE 0xC00080 //TV Btn
  #define IR_MOVE_FORWARD 0xC00058
  #define IR_MOVE_BACKWARD 0xC00059
  #define IR_MOVE_LEFT 0xC0005A
  #define IR_MOVE_RIGHT 0xC0005B

  enum REMOTE_STATE {
    REMOTE_FORWARD,
    REMOTE_BACKWARD,
    REMOTE_LEFT,
    REMOTE_RIGHT,
    REMOTE_UNKNOWN
  };
#endif

enum ACTION {
  LEFT_ROTATION,
  RIGHT_ROTATION,
  MOVE_FORWARD,
  MOVE_BACKWARD,
  NO_ACTION
};

boolean DEBUG_MODE = false;

#if REMOTE_ENABLED
  REMOTE_STATE remoteState = REMOTE_UNKNOWN;
#endif

ACTION botAction = NO_ACTION;

unsigned int obstacleDistance;
unsigned long pingSensorDelay = 100; //ms
unsigned long pingSensorLastEpoch = 0;

unsigned long ledBlinkDelay = 100; //ms
unsigned long ledBlinkLastEpoch = 0;
int ledBlinkState = LOW;

unsigned long DEBOUNCE_DELAY = 200;
unsigned long pushBtnLastEpoch = 0;
int POWER_ON_STATE = LOW;

#if REMOTE_ENABLED
  unsigned long irReceiveDelay = 500; //ms
  unsigned long irReceiveLastEpoch = 0;
#endif

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

AF_DCMotor leftWheel(DC_MOTOR_1, MOTOR34_64KHZ);
AF_DCMotor rightWheel(DC_MOTOR_2, MOTOR34_64KHZ);

#if REMOTE_ENABLED
  IRrecv irrecv(IR_RECV_PIN);
  decode_results results;
#endif

boolean isAutonomous = true; //robot can be set to manual by remote control

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
  leftWheel.run(FORWARD);
  rightWheel.run(BACKWARD);
  botAction = LEFT_ROTATION;
}

void rotateRight() {
  leftWheel.run(BACKWARD);
  rightWheel.run(FORWARD);
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
    if(DEBUG_MODE) Serial.println("Current obstacle distance: " + obstacleDistance);
    pingSensorLastEpoch = currentTime;
  }
}

void runBotAutonomously(unsigned long currentTime) {
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

#if REMOTE_ENABLED
  void runBotManually(unsigned long currentTime) {
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
#endif

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

#if REMOTE_ENABLED
  void setRemoteStateForIRCommand() {
    switch(results.value) {
      case IR_MOVE_FORWARD:
        if(DEBUG_MODE) Serial.println("forward");
        remoteState = REMOTE_FORWARD;
      break;
  
      case IR_MOVE_BACKWARD:
        if(DEBUG_MODE) Serial.println("backward");
        remoteState = REMOTE_BACKWARD;
      break;
  
      case IR_MOVE_LEFT:
        if(DEBUG_MODE) Serial.println("left");
        remoteState = REMOTE_LEFT;
      break;
  
      case IR_MOVE_RIGHT:
        if(DEBUG_MODE) Serial.println("right");
        remoteState = REMOTE_RIGHT;
      break;
  
      case IR_ROBOT_MODE_CHANGE:
        if(DEBUG_MODE) Serial.println("mode change");
        isAutonomous = !isAutonomous;
        stopBot();
      break;
    }
  }

  void checkIRReceiver(unsigned long currentTime) {
    if(currentTime - irReceiveLastEpoch > irReceiveDelay) {
      irReceiveLastEpoch = currentTime;
  
      if(irrecv.decode(&results)) {
        if(DEBUG_MODE) Serial.println(results.value, HEX);
  
        //power on robot
        if(results.value == IR_POWER) {
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
#endif

void checkPushButtonState(unsigned long currentTime) {
  int pushBtnState = digitalRead(PUSH_BTN_PIN);
  
  if(currentTime - pushBtnLastEpoch > DEBOUNCE_DELAY) {
    //switch on at transition from LOW -> HIGH
    if(pushBtnState == HIGH) {
      togglePowerState();
    }
    
    pushBtnLastEpoch = currentTime;
  }
}

void togglePowerState() {
  if(POWER_ON_STATE == LOW) {
    if(DEBUG_MODE) Serial.println("Powered On");
    POWER_ON_STATE = HIGH;
  } else {
    if(DEBUG_MODE) Serial.println("Powered Off");
    POWER_ON_STATE = LOW;
    powerOff();
  }
}

void powerOff() {
  switchOffIndicator();
  stopBot();

  #if REMOTE_ENABLED
    remoteState = REMOTE_UNKNOWN;
  #endif
  
  botAction = NO_ACTION;
  isAutonomous = true;
}

void setup() {
  Serial.begin(9600);

  #if REMOTE_ENABLED
    irrecv.enableIRIn();
  #endif
  
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
  
  #if REMOTE_ENABLED
    checkIRReceiver(currentTime);
  #endif  
  
  if(POWER_ON_STATE == HIGH) {
    if(isAutonomous) {
      //check obstacles
      runPingSensor(currentTime);
    
      //run robot's motor
      runBotAutonomously(currentTime);
    } else {
      if(DEBUG_MODE) Serial.println("Manual Mode On");
      
      //keep LED on
      switchOnIndicator();

      //perform robot action depends on remote controller state
    #if REMOTE_ENABLED
        runBotManually(currentTime);
      #endif
    }
  }
}

