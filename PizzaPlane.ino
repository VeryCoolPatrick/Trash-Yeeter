#include <IRremote.hpp>
#include <Stepper.h>
#include <Servo.h>

const int STEPS_PER_REV = 2038, STEPPER_SPEED = 20;
const int RECV_PIN = 2, SERVO_PIN = 6, TRIG_PIN = 3, ECHO_PIN = 5, BUZZER = 9, LED_R = 7, LED_L = 8 ;
const int CHALLENGE_RANGE = 50;

Stepper myStepper = Stepper(STEPS_PER_REV, 10, 12, 11, 13);
Servo myServo;

int challenge;

void readDistance();
void goodBuzz();
void badBuzz();
void setLeds(int challenge);

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN);
  myStepper.setSpeed(10);
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  randomSeed(analogRead(0));
  challenge = random(2);
  setLeds(challenge);
  Serial.println(challenge);
}

void loop() {
  if (IrReceiver.decode()) {
    switch (IrReceiver.decodedIRData.command) {
      case 0x43:
        myServo.write(160);
        readDistance();
        myServo.write(0);
        delay(200);
        challenge = random(2);
        setLeds(challenge);
        delay(200);
        Serial.println(challenge);
        break;
      case 0x44:
        myStepper.step(STEPPER_SPEED);
        break;
      case 0x40:
        myStepper.step(-STEPPER_SPEED);
        break;
      }
    IrReceiver.resume();
  }
  delay(10);
}

void readDistance() {
  int reading = 200;
  for (int i = 0; i < 200; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delay(2);
    digitalWrite(TRIG_PIN, HIGH);
    delay(10);
    digitalWrite(TRIG_PIN, LOW);
    int temp = pulseIn(ECHO_PIN, HIGH)*0.034/2;
    if (temp < reading) reading = temp;
  }
  if(reading >= CHALLENGE_RANGE * challenge && reading <= CHALLENGE_RANGE * (challenge + 1)) {
    Serial.println("Success!");
    goodBuzz();
  }
  else {
    Serial.println("Fail!");
    badBuzz();
  }
  Serial.println(reading);
}

void goodBuzz() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
  delay(100);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void badBuzz() {
  digitalWrite(BUZZER, HIGH);
  delay(800);
  digitalWrite(BUZZER, LOW);
}

void setLeds(int challenge) {
  digitalWrite(LED_L, !challenge);
  digitalWrite(LED_R, challenge);
}