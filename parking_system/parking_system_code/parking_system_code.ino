const int trigPin = A5;
const int potentiometerPin = A4;
const int echoPin = 11;
const int motor_pin = 10;
const int waitTime = 7000;

float duration, distance, gate_angle;

#include <Keypad.h>

const int ROWS = 4; //four rows
const int COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char userPass[5];
int userIndex = 0;

char *PASSWORD = "1346";

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  int potentiometerValue = analogRead(potentiometerPin);
  int potentiometerFlag = 0;
  
  while (potentiometerValue <= 950) {
    if (!potentiometerFlag) {
      analogWrite(motor_pin, 55);
      potentiometerFlag = 1;
      delay(500);
    }
    analogWrite(motor_pin, map(potentiometerValue, 0, 1023, 180, 0));
    delay(50);
    potentiometerValue = analogRead(potentiometerPin);
  }

  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
 
  duration = pulseIn(echoPin, 1);
  distance = (duration*.0343)/2;
  // Serial.print("Distance: ");
  // Serial.println(distance);
  delay(10);

  if (distance <= 4) {
    analogWrite(motor_pin, 160);
    delay(waitTime);
  }
  else {
    analogWrite(motor_pin, 55);
  }

  char key = keypad.getKey();
  if (userIndex == 5)
    userIndex = 0;

  if (key) {
    if (key == '#') {
      if(comapreStrings(userPass, PASSWORD, userIndex)) {
        analogWrite(motor_pin, 160);
        delay(waitTime);
      }
      userIndex = 0;
    }
    else {
      userPass[userIndex] = key;
      Serial.print("KEY: ");
      Serial.println(userPass[userIndex]);
      Serial.print("current pass: ");
      userIndex += 1;
      printString(userPass, userIndex);
    }
  }

}

int comapreStrings(char *str1, char *str2, int length) {
  for (int i = 0; i < length; i++) {
    if (str1[i] != str2[i])
      return 0;
  }
  return 1;
}

void printString(char *str, int length) {
  for (int i = 0; i < length; i++) {
    Serial.print(str[i]);
    Serial.print("-->");
  }
  Serial.println();
}
