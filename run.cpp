#include <Arduino.h>
//MAIN COMMAND
String cmd="____";
// Motor A/LEFT
#define A_DIR 12
#define A_PWM 3
#define A_BRAKE 9
#define ENC_LEFT 2
// Motor B/RIGHT
#define B_DIR 13
#define B_PWM 11
#define B_BRAKE 8
#define ENC_RIGHT 4
//define command constants in order f/b, F/B, l/r, speed
const int COUNTS_SHORT = 306;   // 12.5 cm
const int COUNTS_LONG = 612;   // 25 cm
const int COUNTS_TURN = 251;   // 90 degrees in-place
const int MOTOR_SPEED = 150;   // PWM 0–255
//define encoders
volatile long leftCount  = 0;
volatile long rightCount = 0;
// Left encoder on D2 (external interrupt)
void leftEncoderISR() {
  leftCount++;
}
// Right encoder on D4 (pin-change interrupt)
ISR(PCINT2_vect) {
  if (PIND & (1 << PIND4)) {
    rightCount++;
  }
}
//set motors
void setLeftMotor(int speed) {
  if (speed == 0) {
    digitalWrite(A_BRAKE, HIGH);
    analogWrite(A_PWM, 0);
    return;
  }
  digitalWrite(A_BRAKE, LOW);
  digitalWrite(A_DIR, speed > 0 ? HIGH : LOW);
  analogWrite(A_PWM, abs(speed));
}
void setRightMotor(int speed) {
  if (speed == 0) {
    digitalWrite(B_BRAKE, HIGH);
    analogWrite(B_PWM, 0);
    return;
  }
  digitalWrite(B_BRAKE, LOW);
  digitalWrite(B_DIR, speed > 0 ? HIGH : LOW);
  analogWrite(B_PWM, abs(speed));
}
// reset encoders
void resetEncoders() {
  noInterrupts();
  leftCount = 0;
  rightCount = 0;
  interrupts();
}
//set basis of all
void driveCounts(long leftTarget, long rightTarget) {
  resetEncoders();
  int leftDir = (leftTarget >= 0) ? 1 : -1;
  int rightDir = (rightTarget >= 0) ? 1 : -1;
  setLeftMotor(leftDir * MOTOR_SPEED);
  setRightMotor(rightDir * MOTOR_SPEED);
  while (true) {
    if (abs(leftCount) >= abs(leftTarget)) setLeftMotor(0);
    if (abs(rightCount) >= abs(rightTarget)) setRightMotor(0);
    if (abs(leftCount) >= abs(leftTarget) &&
        abs(rightCount) >= abs(rightTarget)) {
      break;
    }
  }
  setLeftMotor(0);
  setRightMotor(0);
  delay(100);
}

//run letter command
void executeCommand(char c) {
  switch (c) {
    case 'f': driveCounts(COUNTS_SHORT, COUNTS_SHORT); break;
    case 'F': driveCounts(COUNTS_LONG, COUNTS_LONG ); break;
    case 'b': driveCounts(-COUNTS_SHORT, -COUNTS_SHORT); break;
    case 'B': driveCounts(-COUNTS_LONG, -COUNTS_LONG ); break;
    case 'l': driveCounts(-COUNTS_TURN, COUNTS_TURN ); break;
    case 'L': driveCounts(-COUNTS_TURN, COUNTS_TURN ); break;
    case 'r': driveCounts(COUNTS_TURN, -COUNTS_TURN ); break;
    case 'R': driveCounts(COUNTS_TURN, -COUNTS_TURN ); break;
    default:
      break;
  }
}
//run str on execute command
void executeString(String cmd) {
  for (int i = 0; i < cmd.length(); i++) {
    executeCommand(cmd[i]);
  }
}
//set up pins as __
void setup() {
  pinMode(A_DIR, OUTPUT);
  pinMode(A_PWM, OUTPUT);
  pinMode(A_BRAKE, OUTPUT);
  pinMode(B_DIR, OUTPUT);
  pinMode(B_PWM, OUTPUT);
  pinMode(B_BRAKE, OUTPUT);
  pinMode(ENC_LEFT, INPUT_PULLUP);
  pinMode(ENC_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_LEFT), leftEncoderISR, RISING);
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT20);
  Serial.begin(9600);
}
//run all
void loop() {
    executeString(cmd);
    while (true){}
  }