#include "EchoSensor.h"
#include "LinienSensor.h"
#include "Motor.h"

#define MOTOR_STEER_SPEED 50

EchoSensor DistanceSensor;
LinienSensor SteeringSensor;
Motor LeftMotor, RightMotor;

bool steerLeft, steerRight;
bool drive;

void setup() {
  Serial.begin(9600);

  DistanceSensor.Init(3, 4);
  SteeringSensor.Init(A1, A2, A3);

  LeftMotor.Init(10, 9, 8, false);
  RightMotor.Init(5, 6, 7, true);
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

  // Configure driving direction based on sensor values
  if (IsLine(lenkungResult.Center))
    steerLeft = steerRight = false;
  else if (IsLine(lenkungResult.Left)) {
    steerLeft = true;
    steerRight = false;
  }
  else if (IsLine(lenkungResult.Right)) {
    steerRight = true;
    steerLeft = false;
  }

  drive = !obsticalInTheWay;

  // Apply configured driving directions to motors
  if (!drive) {
    LeftMotor.SetSpeed(0);
    RightMotor.SetSpeed(0);
  }
  else if (!steerLeft && !steerRight) {
    if (drive) Serial.println(" driving forward");
    else Serial.println(" standing still");

    LeftMotor.SetSpeed(MAX_MOTOR_SPEED);
    RightMotor.SetSpeed(MAX_MOTOR_SPEED);
  }
  else if (steerLeft) {
    Serial.println(" driving left");
    LeftMotor.SetSpeed(MOTOR_STEER_SPEED);
    RightMotor.SetSpeed(MAX_MOTOR_SPEED);
  }
  else if (steerRight) {
    Serial.println(" driving right");
    LeftMotor.SetSpeed(MAX_MOTOR_SPEED);
    RightMotor.SetSpeed(MOTOR_STEER_SPEED);
  }
}

inline bool IsLine(int messuredValue) {
  return messuredValue > 50; // needs to be tested and adjusted for every situation!
}
