
#include <TM1637.h>
#include <Keypad.h>
#define leftMotorDirPin 12
#define leftMotorStepPin 13

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int CLK = 2;
int DIO = 3;

TM1637 tm(CLK, DIO);

int digit1 = 0;
int digit2 = 0;
int digit3 = 0;
int digit4 = 0;

boolean isLeftMotorRunning = false;

void setup() {
  pinMode(leftMotorStepPin, OUTPUT);
  pinMode(leftMotorDirPin, OUTPUT);
  // put your setup code here, to run once:
  tm.init();

  // set brightness; 0-7
  tm.set(1);

  Serial.begin(9600);
}

void moveMotorsWithOneStep() {
    // These four lines result in 1 step:
    digitalWrite(leftMotorStepPin, HIGH);
    delayMicroseconds(6000);
    digitalWrite(leftMotorStepPin, LOW);
    delayMicroseconds(6000);
}

void moveMotorsForward(int steps) {
      digitalWrite(leftMotorDirPin, LOW);

      for (int i = 0; i < steps; i++) {
        moveMotorsWithOneStep();
      }

      delay(1000);
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
  char key = keypad.getKey();

  if (key) {
    isLeftMotorRunning = true;

    Serial.println(key);

    String keyStr = String(key);

    boolean shouldRun = (key == '#');
    boolean shouldReset = (key == '*');

    if (shouldRun) {
      int totalSteps = digit1 * 1000 + digit2 * 100 + digit3 * 10 + digit4;
      
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
