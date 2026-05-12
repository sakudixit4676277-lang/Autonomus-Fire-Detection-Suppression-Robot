#include <Servo.h>

#define FLAME_L A0
#define FLAME_C A1
#define FLAME_R A2

#define MOTOR_L1 5
#define MOTOR_L2 6
#define MOTOR_R1 9
#define MOTOR_R2 10

#define PUMP 7

#define SERVO_PIN 3
Servo nozzle;

#define FIRE_TH 500

#define MOVE_TIME 200   // 0.2 second
unsigned long startTime = 0;
bool moving = false;

int pos = 90;
int dir = 1;

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_L1, OUTPUT);
  pinMode(MOTOR_L2, OUTPUT);
  pinMode(MOTOR_R1, OUTPUT);
  pinMode(MOTOR_R2, OUTPUT);

  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);

  nozzle.attach(SERVO_PIN);
  nozzle.write(90);
}

void loop() {

  int L = analogRead(FLAME_L);
  int C = analogRead(FLAME_C);
  int R = analogRead(FLAME_R);

  bool fL = (L > FIRE_TH);
  bool fC = (C > FIRE_TH);
  bool fR = (R > FIRE_TH);

  bool fire = (fL || fC || fR);

  if (fire) {

    if (!moving) {
      moving = true;
      startTime = millis();
    }

   
    if (millis() - startTime < MOVE_TIME) {

      
      if (fL && fC && fR) {
        forward();
        delay(300);
        stopMotors();
      }

      else if (fL && fC) {
        left();
      }

      else if (fR && fC) {
        right();
      }

      else if (fC) {
        forward();
      }

      else if (fL) {
        left();
      }

      else if (fR) {
        right();
      }

      digitalWrite(PUMP, LOW);
      nozzle.write(90);
    }

    
    else {
      stopMotors();
      digitalWrite(PUMP, HIGH);
      servoLogic(fL, fC, fR);
    }

  } else {
    moving = false;
    stopAll();
  }

  delay(40);
}


void servoLogic(bool fL, bool fC, bool fR) {

  if (fL && fC && fR) {
    sweep(50, 130);
  }

  else if (fL && fC) {
    sweep(50, 90);
  }

  else if (fR && fC) {
    sweep(90, 130);
  }

  else if (fL) {
    nozzle.write(50);
  }

  else if (fR) {
    nozzle.write(130);
  }

  else if (fC) {
    nozzle.write(90);
  }
}

void sweep(int minA, int maxA) {
  pos += dir * 4;

  if (pos >= maxA) {
    pos = maxA;
    dir = -1;
  }

  if (pos <= minA) {
    pos = minA;
    dir = 1;
  }

  nozzle.write(pos);
}


void forward() {
  digitalWrite(MOTOR_L1, HIGH);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, HIGH);
  digitalWrite(MOTOR_R2, LOW);
}

void left() {
  digitalWrite(MOTOR_L1, LOW);
  digitalWrite(MOTOR_L2, HIGH);
  digitalWrite(MOTOR_R1, HIGH);
  digitalWrite(MOTOR_R2, LOW);
}

void right() {
  digitalWrite(MOTOR_L1, HIGH);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW);
  digitalWrite(MOTOR_R2, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_L1, LOW);
  digitalWrite(MOTOR_L2, LOW);
  digitalWrite(MOTOR_R1, LOW);
  digitalWrite(MOTOR_R2, LOW);
}

void stopAll() {
  stopMotors();
  digitalWrite(PUMP, LOW);
  nozzle.write(90);
}
