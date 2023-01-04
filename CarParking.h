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

#if !defined(PRINT) && !defined(PRINTLN)
#define PRINT(msg, ...)
#define PRINTLN(msg, ...)
#endif

void Park(Motor* leftMotor, Motor* rightMotor, EchoSensor* rightEchoSensor) {
  leftMotor->SetSpeed(CAR_DRIVE_L);
  rightMotor->SetSpeed(CAR_DRIVE_R);
  bool nextToPotentialSpot = false;
  unsigned long start;
  while (true) {
    // Calculate diffrence between last and new Distance parallel to the car
    static float lastDistance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float distance = rightEchoSensor->GetDistance(Metric::Centimeter);
    float difference = abs(distance - lastDistance);
    PRINTLN(difference);
    // Wait until we drive past a parking spot
    if (difference < CAR_LENGHT_X_CM) {
      lastDistance = distance;
      delay(150);
      continue;
    }

    if (nextToPotentialSpot) {
      // Found potential parking slot
      float timeItTookToDrivePastSlot = millis() - start;
      int distanceCm = (timeItTookToDrivePastSlot / 1000) * DISTANCE_PER_SECOND_IN_CM_100_85_POWER;
      nextToPotentialSpot = false;

      // Check if we would fit in the parking slot
      if (distanceCm > CAR_LENGHT_Y_CM + SAFTY_MESSURES_CM) {
        PRINT("Found Parking Slot! It took ");
        PRINT(timeItTookToDrivePastSlot / 1000.0f);
        PRINT("s to drive past it and it's ");
        PRINT(distanceCm);
        PRINTLN("cm");

        // 90° right backwards
        leftMotor->SetDirection(MotorDirection::Backward);
        leftMotor->SetSpeed(100);
        rightMotor->SetSpeed(0);
        delay(800);
        // some bit backwards
        rightMotor->SetDirection(MotorDirection::Backward);
        rightMotor->SetSpeed(85);
        delay(550);
        // 90° left backwards
        leftMotor->SetSpeed(0);
        delay(680);
        // tiny bit backwards
        leftMotor->SetSpeed(100);
        delay(100);
        // Finished parking
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