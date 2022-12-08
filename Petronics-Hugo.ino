#include "EchoSensor.h"
#include "LinienSensor.h"
#include "Motor.h"
#include "Car.h"

#define MOTOR_STEER_SPEED 100 // The speed at which the side responsible for steering should move

Car::Steering SteeringWheel;
Car::Gas GasPedal;

EchoSensor DistanceSensor;
LinienSensor SteeringSensor;
Motor LeftMotor, RightMotor;

void setup() {
  Serial.begin(9600);

  DistanceSensor.Init(3, 4);
  SteeringSensor.Init(A1, A2, A3);

  LeftMotor.Init(10, 9, 8);
  RightMotor.Init(5, 6, 7);
  LeftMotor.SetDirection(MotorDirection::Forward);
  RightMotor.SetDirection(MotorDirection::Forward);
}


void loop() {
  // Print out sensor raw values for debugging
  long frontDistance = DistanceSensor.GetDistance(Metric::Centimeter);
  bool obsticalInTheWay = frontDistance < 5; // if front distance is less than 5cm away
  Serial.print("FrontDistance: ");
  Serial.print(frontDistance);

  auto lenkungResult = SteeringSensor.Messure();
  Serial.print("    Raw Sensor: [L: ");
  Serial.print(lenkungResult.Left);
  Serial.print(", C: ");
  Serial.print(lenkungResult.Center);
  Serial.print(", R: ");
  Serial.print(lenkungResult.Right);
  Serial.print(']');

  // Configure the Gaspedal and SteeringWheel based on sensor values
  if (IsLine(lenkungResult.Center))
    SteeringWheel = Car::Steering::Neutral;
  else if  (IsLine(lenkungResult.Left))
    SteeringWheel = Car::Steering::Left;
  else if (IsLine(lenkungResult.Right))
    SteeringWheel = Car::Steering::Right;

  if (obsticalInTheWay) {
    SteeringWheel = Car::Steering::Neutral;
    GasPedal = Car::Gas::Still;
  }

  // Set Motor speeds based on GasPedal and SteeringWheel
  int drivingDirectionMultiplier = 0;
  if (GasPedal == Car::Gas::Forward) drivingDirectionMultiplier = 1;
  else if (GasPedal == Car::Gas::Backward) drivingDirectionMultiplier = -1;

  switch (SteeringWheel) {
    default:
    case Car::Steering::Neutral:
      if (GasPedal == Car::Gas::Forward) Serial.println(" driving forward");
      else if (GasPedal == Car::Gas::Backward) Serial.println(" driving backward");
      else Serial.println(" standing still");

      LeftMotor.SetSpeed(MAX_MOTOR_SPEED * drivingDirectionMultiplier);
      RightMotor.SetSpeed(MAX_MOTOR_SPEED * drivingDirectionMultiplier);
      break;
    case Car::Steering::Left:
      Serial.println(" driving left");
      LeftMotor.SetSpeed(MOTOR_STEER_SPEED * drivingDirectionMultiplier);
      RightMotor.SetSpeed(MAX_MOTOR_SPEED * drivingDirectionMultiplier);
      break;
    case Car::Steering::Right:
      Serial.println(" driving right");
      LeftMotor.SetSpeed(MAX_MOTOR_SPEED * drivingDirectionMultiplier);
      RightMotor.SetSpeed(MOTOR_STEER_SPEED * drivingDirectionMultiplier);
      break;
  }

  delay(50);  // only for debug!
}

inline bool IsLine(int messuredValue) {
  return messuredValue > 50; // needs to be tested and adjusted for every situation!
}
