const int trigPin = A5;
const int potentiometerPin = A4;
const int echoPin = 11;
const int motor_pin = 10;
const int waitTime = 7000;
const int close_gate = 230;
const int open_gate = 110;
const int distance_to_open = 4;
char *PASSWORD = "1346";
const int PASSWORD_LENGTH = 4;

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


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  int potentiometerValue = analogRead(potentiometerPin);
  int potentiometerFlag = 0;
  
  while (potentiometerValue <= 1000) { // stay forever until the potentiometer is at default
    if (!potentiometerFlag) { // if it's the first time entering the loop, restart everything
      analogWrite(motor_pin, close_gate);
      potentiometerFlag = 1;
      delay(500);
    }
    analogWrite(motor_pin, map(potentiometerValue, 0, 1023, 0, 240)); // note that 240 is not the max but it's the best value with respect to the angle and speed specifics
    delay(50);
    potentiometerValue = analogRead(potentiometerPin);
  }

  // restart the trigger, then start it again then restart it, (neg_edge, pos_edge, neg_edge)
  digitalWrite(trigPin, 0);
  delayMicroseconds(2);
  digitalWrite(trigPin, 1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0);
 
  duration = pulseIn(echoPin, 1); // duration: The time (in microseconds) that the echo signal remains HIGH on the echoPin
  distance = (duration*.0343)/2; // (343 meters/second) speed of the light in the air, (2) measure the distance for one way
   Serial.print("Distance: ");
   Serial.println(distance);
  delay(10);

  if (distance <= distance_to_open) { 
    analogWrite(motor_pin, open_gate);
    delay(waitTime);
  }
  else {
    analogWrite(motor_pin, close_gate);
  }

  char key = keypad.getKey(); // get the key the user entered
  if (userIndex == 5)
    userIndex = 0;

  if (key) {
    if (key == '#') { // # is stand for confirm
      if(userIndex == PASSWORD_LENGTH && comapreStrings(userPass, PASSWORD, PASSWORD_LENGTH)) {
        analogWrite(motor_pin, open_gate);
        delay(waitTime);
      }
      userIndex = 0;
    }
    else { // if not confirmed yet
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

  for (int i = 0; i < length; i++) {
    str1[0] = '\0'; // restart the string each time
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
