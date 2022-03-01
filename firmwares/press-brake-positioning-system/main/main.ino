#include <ezButton.h>
#include <TM1637.h>
#include <Keypad.h>
#define leftMotorDirPin 12
#define leftMotorStepPin 13
#define rightMotorDirPin A4
#define rightMotorStepPin A5

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

const char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte pin_rows[ROW_NUM] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
const byte pin_column[COLUMN_NUM] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const int CLK = 2;
const int DIO = 3;

TM1637 tm(CLK, DIO);

int digit1 = 0;
int digit2 = 0;
int digit3 = 0;
int digit4 = 0;
int MOTORS_SPEED = 4000;
boolean SHOULD_STOP = false;

const ezButton leftLimitSwitch(A0);  // create ezButton object for left motor that's attached to pin A0;
const ezButton rightLimitSwitch(A1);  // create ezButton object for right motor that's attached to pin A1;

const float DISTANCE_PER_STEP = 0.39269908169872414;

const int LEFT_MOTOR_FORWARD = LOW;
const int LEFT_MOTOR_BACKWARD = HIGH;

const int RIGHT_MOTOR_FORWARD = HIGH;
const int RIGHT_MOTOR_BACKWARD = LOW;

void setup() {
  pinMode(leftMotorStepPin, OUTPUT);
  pinMode(leftMotorDirPin, OUTPUT);
  pinMode(rightMotorStepPin, OUTPUT);
  pinMode(rightMotorDirPin, OUTPUT);

  tm.init();

  // set brightness; 0-7
  tm.set(1);

  Serial.begin(9600);

  leftLimitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  rightLimitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void moveMotorsWithOneStepForward() {
  // These four lines result in 1 step:
  digitalWrite(leftMotorStepPin, HIGH);
  digitalWrite(rightMotorStepPin, HIGH);
  delayMicroseconds(MOTORS_SPEED);

  digitalWrite(leftMotorStepPin, LOW);
  digitalWrite(rightMotorStepPin, LOW);
  delayMicroseconds(MOTORS_SPEED);
}

void moveMotorsForward(int steps) {
  Serial.println("Moving both motors forward...");

  digitalWrite(leftMotorDirPin, LEFT_MOTOR_FORWARD);
  digitalWrite(rightMotorDirPin, RIGHT_MOTOR_FORWARD);

  for (int i = 0; i < steps; i++) {
    if (keypad.getKey()) {
      SHOULD_STOP = true;
    }
    if (SHOULD_STOP) {
      return;
    }
    moveMotorsWithOneStepForward();
  }
}

float calcDistance(int steps) {
  return steps * DISTANCE_PER_STEP;
}

int calculateSteps(int milimeters) {
  int steps =  round(milimeters / DISTANCE_PER_STEP);

  Serial.println("Moving " + (String)steps + " steps for " + (String)calcDistance(steps) + " mm");

  return steps;
}

void resetMotors() {
  Serial.println("Resetting motors...");

  digitalWrite(leftMotorDirPin, LEFT_MOTOR_BACKWARD);
  digitalWrite(rightMotorDirPin, RIGHT_MOTOR_BACKWARD);

  boolean leftMotorAtZero = leftLimitSwitch.getState() == LOW;
  boolean rightMotorAtZero = rightLimitSwitch.getState() == LOW;

  while (true) {
    if (keypad.getKey()) {
      SHOULD_STOP = true;
    }
    if (SHOULD_STOP) {
      return;
    }

    leftLimitSwitch.loop(); // MUST call the loop() function first
    rightLimitSwitch.loop(); // MUST call the loop() function first

    if (leftMotorAtZero && rightMotorAtZero) {
      // Both motors are at zero point.
      Serial.println("Both motors at zero point");
      return;
    }

    boolean shouldMoveLeftMotor = !leftLimitSwitch.isPressed();
    boolean shouldMoveRightMotor = !rightLimitSwitch.isPressed();

    if(shouldMoveLeftMotor && !leftMotorAtZero) {
      digitalWrite(leftMotorStepPin, HIGH);
    } else {
      Serial.println("Left motor at zero point");
      leftMotorAtZero = true;
    }


    if(shouldMoveRightMotor && !rightMotorAtZero) {
      digitalWrite(rightMotorStepPin, HIGH);
    } else {
      Serial.println("Right motor at zero point");
      rightMotorAtZero = true;
    }

    delayMicroseconds(MOTORS_SPEED);

    digitalWrite(leftMotorStepPin, LOW);
    digitalWrite(rightMotorStepPin, LOW);
    delayMicroseconds(MOTORS_SPEED);
  }
}

void displayValue() {
  // Display value
  // example: "12:ab"
  tm.display(0, digit1);
  tm.display(1, digit2);
  tm.point(1);
  tm.display(2, digit3);
  tm.display(3, digit4);
}

void setDigits(int newDigit1, int newDigit2, int newDigit3, int newDigit4) {
  digit1 = newDigit1;
  digit2 = newDigit2;
  digit3 = newDigit3;
  digit4 = newDigit4;
}

void loop() {
  SHOULD_STOP = false;

  leftLimitSwitch.loop(); // MUST call the loop() function first
  rightLimitSwitch.loop(); // MUST call the loop() function first

  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    String keyStr = String(key);

    boolean shouldRun = (key == '#');
    boolean shouldReset = (key == '*');

    if (shouldRun) {
      int totalMilimeters = digit1 * 1000 + digit2 * 100 + digit3 * 10 + digit4;
      int totalSteps = calculateSteps(totalMilimeters);
      
      resetMotors();
      moveMotorsForward(totalSteps);

    } else if (shouldReset) {
      // Reset to 00:00;
      setDigits(0, 0, 0, 0);
    } else {
      // Set the new digit
      // Example:
      // New keyStr is `3` and the current value is 01:23
      // The result will be 12:33
      setDigits(digit2, digit3, digit4, keyStr.toInt());
    }
  }

  displayValue();
}
