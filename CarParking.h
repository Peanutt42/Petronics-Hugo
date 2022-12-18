// Author: Peter Hennig

#pragma once

#include "EchoSensor.h"
#include "Motor.h"

#define DISTANCE_PER_SECOND_IN_CM_128_POWER 38
#define DISTANCE_PER_SECOND_IN_CM_100_85_POWER 27

#define CAR_LENGHT_Y_CM 30
#define CAR_LENGHT_X_CM 15

#define SAFTY_MESSURES_CM 5

#define CAR_DRIVE_L 100
#define CAR_DRIVE_R 85

#define Stand(time) \
  leftMotor->SetSpeed(0); \
  rightMotor->SetSpeed(0); \
  delay(time)

void Park(Motor* leftMotor, Motor* rightMotor, EchoSensor* rightEchoSensor) {
  leftMotor->SetSpeed(CAR_DRIVE_L);
  rightMotor->SetSpeed(CAR_DRIVE_R);
  bool nextToPotentialSpot = false;
  unsigned long start;
  while (true) {
    // Calculate diffrence between last and new Distance parallel to the car
    static float lastDistance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float distance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float diffrence = abs(distance - lastDistance);
    Serial.println(diffrence);
    if (diffrence < CAR_LENGHT_X_CM) {
      lastDistance = distance;
      delay(150);
      continue;
    }

    if (nextToPotentialSpot) {
      // Found potential parking slot
      float timeItTookToDrivePastSlot = millis() - start;
      int distanceCm = (timeItTookToDrivePastSlot / 1000) * DISTANCE_PER_SECOND_IN_CM_100_85_POWER;
      nextToPotentialSpot = false;
      if (distanceCm > CAR_LENGHT_Y_CM + SAFTY_MESSURES_CM) {
        Serial.print("Found Parking Slot! It took ");
        Serial.print(timeItTookToDrivePastSlot / 1000.0f);
        Serial.print("s to drive past it and it's ");
        Serial.print(distanceCm);
        Serial.println("cm");

        leftMotor->SetSpeed(0);
        rightMotor->SetSpeed(0);

        leftMotor->SetDirection(MotorDirection::Backward);
        leftMotor->SetSpeed(100);
        delay(800);
        rightMotor->SetDirection(MotorDirection::Backward);
        rightMotor->SetSpeed(85);
        delay(550);
        leftMotor->SetSpeed(0);
        delay(680);
        leftMotor->SetSpeed(100);
        delay(100);
        leftMotor->SetSpeed(0);
        leftMotor->SetDirection(MotorDirection::Forward);
        rightMotor->SetSpeed(0);
        rightMotor->SetDirection(MotorDirection::Forward);
        delay(5000);
        return;
      }
    } else {
      nextToPotentialSpot = true;
      start = millis();
    }
    lastDistance = distance;
    delay(200);
  }
}