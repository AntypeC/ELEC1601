#include <Servo.h>
const int irLedPin = 6;
const int irLedPinLEFT = 10;
const int irLedPinRIGHT = 2;

const int irReceiverPin = 7;
const int redLedPin = A1;

Servo leftWheel;
Servo rightWheel;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(irLedPin, OUTPUT);
  pinMode(irReceiverPin, INPUT);
  pinMode(redLedPin, OUTPUT);

  leftWheel.attach(13); // right is 12, left is 13
  rightWheel.attach(12);
  
}

volatile int time = 20;

void run_forward(int velocity, int time) {
  // turns forward
  leftWheel.writeMicroseconds(1500 + velocity);
  rightWheel.writeMicroseconds(1500 - (velocity + 5));
  delay(time);
}
void pause(int time) {
  // pause
  leftWheel.writeMicroseconds(1500);
  rightWheel.writeMicroseconds(1500);
  delay(time);
}
void run_backward(int velocity, int time) {
  // turns backward
  leftWheel.writeMicroseconds(1500 - velocity);
  rightWheel.writeMicroseconds(1500 + velocity);
  delay(time);
}
void turn_left(int angle, int time) {
  // turns left
  leftWheel.writeMicroseconds(1500 + angle);
  rightWheel.writeMicroseconds(1500 + (angle + 5));
  delay(time);
}
void turn_right(int angle, int time) {
  // turns right
  leftWheel.writeMicroseconds(1500 - angle);
  rightWheel.writeMicroseconds(1500 - (angle + 5));
  delay(time);
}

int irDistance(int sensor) {
    int distance = 0;

    for (long f = 38000; f <= 45000; f += 1000) { 
      tone(sensor, f); 
      delay(time); 
      
      distance += digitalRead(irReceiverPin); 
      noTone(sensor); 
    }
    return distance;
}

void loop() {
  Serial.print("FRONT OBSTACLE: ");
  Serial.println(irDistance(irLedPin));
  int front = irDistance(irLedPin);
  Serial.println(front);
  Serial.print("LEFT OBSTACLE: ");
  int left = irDistance(irLedPinLEFT);
  Serial.println(left);
  Serial.print("RIGHT OBSTACLE: ");
  int right = irDistance(irLedPinRIGHT);
  Serial.println(right);
  int velocity = 30;
  if (front <= 5) { // !=0 means no obstacle, >= 4 means no obstacle in long proximity
    leftWheel.writeMicroseconds(1495);
    rightWheel.writeMicroseconds(1495);
    delay(time);
    if (left >= 5) {
      turn_left(30, 15);
    } else if (right >= 5) {
      turn_right(30, 15);
    }
  } else { // 0 means obstecle 
    run_forward(velocity, time);
  }
  delay(time);
}