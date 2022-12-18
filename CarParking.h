// Author: Peter Hennig

#pragma once

#include "EchoSensor.h"
#include "Motor.h"

#define DISTANCE_PER_SECOND_IN_CM_128_POWER 38
#define DISTANCE_PER_SECOND_IN_CM_100_85_POWER 27

#define CAR_LENGHT_Y_CM 30
#define CAR_LENGHT_X_CM 18

#define SAFTY_MESSURES_CM 5

void Park(Motor* leftMotor, Motor* rightMotor, EchoSensor* rightEchoSensor) {
  leftMotor->SetSpeed(100);
  rightMotor->SetSpeed(85);
  bool nextToPotentialSpot = false;
  unsigned long start;
  while (true) {
    // Calculate diffrence between last and new Distance parallel to the car
    static float lastDistance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float distance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float diffrence = abs(distance - lastDistance);
    Serial.println(diffrence);
    if (diffrence > CAR_LENGHT_X_CM + SAFTY_MESSURES_CM) {
      if (!nextToPotentialSpot) {
        nextToPotentialSpot = true;
        start = millis();
      } else {
        // Found potential parking slot
        float timeItTookToDrivePastSlot = millis() - start;
        int distanceCm = (timeItTookToDrivePastSlot / 1000) * DISTANCE_PER_SECOND_IN_CM_100_85_POWER;
        if (distanceCm > CAR_LENGHT_Y_CM + SAFTY_MESSURES_CM) {
          Serial.print("Found Parking Slot! It took ");
          Serial.print(timeItTookToDrivePastSlot / 1000.0f);
          Serial.print("s to drive past it and it's ");
          Serial.print(distanceCm);
          Serial.println("cm");

          // Drive back to the beginning of the parking slot
          leftMotor->SetDirection(MotorDirection::Backward);
          rightMotor->SetDirection(MotorDirection::Backward);
          leftMotor->SetSpeed(100);
          rightMotor->SetSpeed(85);
          delay(timeItTookToDrivePastSlot);
          leftMotor->SetDirection(MotorDirection::Forward);
          rightMotor->SetDirection(MotorDirection::Forward);
        }
        nextToPotentialSpot = false;
      }
    }
    lastDistance = distance;
  }
}