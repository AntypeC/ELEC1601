#include <Servo.h>
const int irLedPin = 6;
const int irLedPinLEFT = 10;
const int irLedPinRIGHT = 2;

const int irReceiver = 7;
const int irReceiverLEFT = 11;
const int irReceiverRIGHT = 3;

const int LedPin = A1;
const int LedPinLEFT = A2;
const int LedPinRIGHT = A0;

Servo leftWheel;
Servo rightWheel;

// Ctrl + Shift + M

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(irLedPin, OUTPUT);
  pinMode(irReceiver, INPUT);
  pinMode(irReceiverLEFT, INPUT);
  pinMode(irReceiverRIGHT, INPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(LedPinLEFT, OUTPUT);
  pinMode(LedPinRIGHT, OUTPUT);

  leftWheel.attach(13); // right is 12, left is 13
  rightWheel.attach(12);
  
}

volatile int time = 20;

void run_forward(int velocity, int tilt, int time) {
  // turns forward
  leftWheel.writeMicroseconds(1500 + velocity);
  rightWheel.writeMicroseconds(1500 - (velocity + 15) + tilt);
  delay(time);
}
void pause(int time) {
  // pause
  leftWheel.writeMicroseconds(1495);
  rightWheel.writeMicroseconds(1498);
  delay(time);
}
void run_backward(int velocity, int time) {
  // turns backward
  leftWheel.writeMicroseconds(1500 - velocity);
  rightWheel.writeMicroseconds(1500 + velocity);
  delay(time);
}
void turn_right(int angle, int time) {
  // turns left
  leftWheel.writeMicroseconds(1500 + angle);
  rightWheel.writeMicroseconds(1500 + (angle));
  delay(time);
}
void turn_left(int angle, int time) {
  // turns right
  leftWheel.writeMicroseconds(1500 - angle);
  rightWheel.writeMicroseconds(1500 - (angle));
  delay(time);
}

int irDistance(int irLed, int irReceiver, int led) {
    int distance = 0;

    for (long f = 38000; f <= 45000; f += 1000) { 
      tone(irLed, f); 
      delay(5); 
      
      distance += digitalRead(irReceiver); 
      noTone(irLed); 

    }
    if (distance != 8) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
    return distance;
}

int commitDirection = 0;
// 0 = no commitments
// -1 = left
// 1 = right
int clearCount = 0;
const int TURN_DURATION = 250;
const int FRONT_THRESHOLD = 5;
const int CLEAR_REQUIRED = 3;

void loop() {
  int left = irDistance(irLedPinLEFT, irReceiverLEFT, LedPinLEFT);
  int front = irDistance(irLedPin, irReceiver, LedPin);
  int right = irDistance(irLedPinRIGHT, irReceiverRIGHT, LedPinRIGHT);
  Serial.print("LEFT OBSTACLE: ");
  Serial.print(left);
  Serial.print(" | FRONT OBSTACLE: ");
  Serial.print(front);
  Serial.print(" | RIGHT OBSTACLE: ");
  Serial.print(right);
  Serial.println("");
  int velocity = 30;
  if (front <= FRONT_THRESHOLD) { // 0 means immediate obstacle, !=0 means no immediate obstacle, >= 5 means no obstacle in long proximity, 8 means no obstacle
    pause(20);
    if (commitDirection == 0) {
      if (left > right) {
        pause(20);
        Serial.println("MOVE LEFT");
        commitDirection = -1;
      } else if (right > left) {
        pause(20);
        Serial.println("MOVE RIGHT");
        commitDirection = 1;
      } else {
        pause(20);
        run_backward(velocity, 10);
        Serial.println("MOVE LEFT");
        commitDirection = -1;
      }
    }
    if (commitDirection == -1) {
      turn_left(120, TURN_DURATION);
    } else {
      turn_right(120, TURN_DURATION);
    }
  } else { // 0 means obstacle
    Serial.println("MOVE FORWARD");
    commitDirection = 0;
    if ((left == 8) && (right == 8) && (front == 8)) {
      pause(20);
    } else {
      if (left > right) {
        run_forward(velocity, -10, 10);
      } else if (right > left) {
        run_forward(velocity, 10, 10);
      } else {
        run_forward(velocity, 0, 10);
      }
    }
  }
  delay(time);
}