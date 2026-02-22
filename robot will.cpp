//robot tour code
//pins
const int LEFT_MOTOR_PWM  = 5;
const int LEFT_MOTOR_DIR  = 4;
const int RIGHT_MOTOR_PWM = 6;
const int RIGHT_MOTOR_DIR = 7;
const int LEFT_ENCODER_PIN  = 2;  
const int RIGHT_ENCODER_PIN = 3;  // check for interupt capable

//encoder variables
volatile long leftEncoderCount  = 0;
volatile long rightEncoderCount = 0;

// TUNING
const int MOTOR_SPEED = 150;           // <255
const int TICKS_PER_REV = 360;          
const float WHEEL_DIAMETER_CM = 6.5;    // wheel diameter - determine with noah
const float WHEEL_BASE_CM = 14.0;       // distance between wheels - again check w/ noah

long ticksFor90Deg;

//ISRs  - interupt service routine
void leftEncoderISR() {
  leftEncoderCount++;
}

void rightEncoderISR() {
  rightEncoderCount++;
}

//Setup code
void setup() {
  Serial.begin(9600);

  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);

  pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), rightEncoderISR, RISING);

  //90-degree turn
  float turnCircumference = PI * WHEEL_BASE_CM;
  float distanceFor90 = turnCircumference / 4.0;
  float wheelCircumference = PI * WHEEL_DIAMETER_CM;
  ticksFor90Deg = (distanceFor90 / wheelCircumference) * TICKS_PER_REV;

  Serial.println("send commends");
}

//motor control
void stopMotors() {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
}

void moveForward() {
  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);
  analogWrite(LEFT_MOTOR_PWM, MOTOR_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, MOTOR_SPEED);
}

void moveBackward() {
  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);
  analogWrite(LEFT_MOTOR_PWM, MOTOR_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, MOTOR_SPEED);
}

void turnLeft90() {
  leftEncoderCount = 0;
  rightEncoderCount = 0;

  digitalWrite(LEFT_MOTOR_DIR, LOW);
  digitalWrite(RIGHT_MOTOR_DIR, HIGH);

  analogWrite(LEFT_MOTOR_PWM, MOTOR_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, MOTOR_SPEED);

  while (abs(leftEncoderCount) < ticksFor90Deg &&
         abs(rightEncoderCount) < ticksFor90Deg) {
  }

  stopMotors();
}

void turnRight90() {
  leftEncoderCount = 0;
  rightEncoderCount = 0;

  digitalWrite(LEFT_MOTOR_DIR, HIGH);
  digitalWrite(RIGHT_MOTOR_DIR, LOW);

  analogWrite(LEFT_MOTOR_PWM, MOTOR_SPEED);
  analogWrite(RIGHT_MOTOR_PWM, MOTOR_SPEED);

  while (abs(leftEncoderCount) < ticksFor90Deg &&
         abs(rightEncoderCount) < ticksFor90Deg) {
  }

  stopMotors();
}

// main code
void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 'F':
        moveForward();
        std::print("F");
        break;
      case 'B':
        moveBackward();
        std::print("B");
        break;
      case 'L':
        turnLeft90();
        std::print("L");
        break;
      case 'R':
        turnRight90();
        std::print("R");
        break;
      default:
        stopMotors();
        break;
    }
  }

}
