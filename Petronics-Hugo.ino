// Author: Peter Hennig

#include "LinienSensor.h"
#include "EchoSensor.h"
#include "Motor.h"
#include "CarParking.h"

#pragma region Pins
#define PIN_BUTTON 2
#define PIN_LDR A0
#define PIN_LED 11

#define PIN_ECHO_TRIGGER 3
#define PIN_ECHO_ECHO 4

#define PIN_STEERING_LEFT A1
#define PIN_STEERING_CENTER A2
#define PIN_STEERING_RIGHT A3

#define PIN_MOTOR_L_GSM 5
#define PIN_MOTOR_L_IN1 6
#define PIN_MOTOR_L_IN2 7
#define PIN_MOTOR_R_GSM 10
#define PIN_MOTOR_R_IN1 9
#define PIN_MOTOR_R_IN2 8
#pragma endregion // Pins

#pragma region Configuration
#define DEBUG true
#if DEBUG
#define PRINT(msg, ...) Serial.print(msg, ##__VA_ARGS__)
#define PRINTLN(msg, ...) Serial.println(msg, ##__VA_ARGS__)
#else
#define PRINT(msg, ...)
#define PRINTLN(msg, ...)
#endif

#define MOTOR_STEER_SPEED 0
#define MOTOR_DRIVE_SPEED_L 100
#define MOTOR_DRIVE_SPEED_R 85

#define IS_LINE_THRESHOLD 150
#pragma endregion // Configuration

#pragma region Sensors
LinienSensor SteeringSensor;
Motor LeftMotor, RightMotor;
EchoSensor DistanceSensor;
#pragma endregion // Sensors

#pragma region Driving Commands
bool steerLeft = false, steerRight = false;
bool drive = true;
bool obsticalInTheWay = false;
#pragma endregion // Driving Commands

int slowCycles, fastCycles; // loop cycle counters to do some things less often

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif

  DistanceSensor.Init(PIN_ECHO_TRIGGER, PIN_ECHO_ECHO, 50);

  SteeringSensor.Init(PIN_STEERING_LEFT, PIN_STEERING_CENTER, PIN_STEERING_RIGHT);

  LeftMotor.Init(PIN_MOTOR_L_GSM, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2);
  RightMotor.Init(PIN_MOTOR_R_GSM, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  drive = true;

  ReadButton();

  UpdateCycledPrograms();

  ReadSteeringSensor();

  SetMotors();

  BlinkProgramStatusLED();
}

void ReadButton() {
  bool buttonState = digitalRead(PIN_BUTTON) == LOW;
  static bool buttonLastState;
  static bool stopCar;
  if (buttonState != buttonLastState) {
    buttonLastState = buttonState;
    if (buttonState)
      Park(&LeftMotor, &RightMotor, &DistanceSensor);  //stopCar = !stopCar;
  }
  if (stopCar) drive = false;
}

void UpdateCycledPrograms() {
  slowCycles++;
  fastCycles++;
  if (fastCycles > 3) fastCycles = 0;
  if (slowCycles > 10) slowCycles = 0;
  if (fastCycles == 0) MessureDistanceSensor();
  if (slowCycles == 0) UpdateNightLight();

  if (obsticalInTheWay) drive = false;
}

void ReadSteeringSensor() {
  LinienSensorResult lenkungResult = SteeringSensor.Messure();
  PRINT("    Raw Sensor: [L: ");
  PRINT(lenkungResult.Left);
  PRINT(", C: ");
  PRINT(lenkungResult.Center);
  PRINT(", R: ");
  PRINT(lenkungResult.Right);
  PRINT(']');

  // Configure driving direction based on sensor values
  bool lineIsLeft = lenkungResult.Left > IS_LINE_THRESHOLD,
       lineIsRight = lenkungResult.Right > IS_LINE_THRESHOLD,
       lineIsCenter = lenkungResult.Center > IS_LINE_THRESHOLD;
  if (lineIsCenter) {
    steerLeft = false;
    steerRight = false;
  } else if (lineIsLeft) {
    steerLeft = true;
    steerRight = false;
  } else if (lineIsRight) {
    steerRight = true;
    steerLeft = false;
  }
  if (lineIsLeft && lineIsRight && lineIsCenter) {
    steerLeft = false;
    steerRight = false;
    drive = false;
  }
}

// Apply configured driving directions to motors
void SetMotors() {
  if (!drive) {
    PRINTLN(" standing still");
    LeftMotor.SetSpeed(0);
    RightMotor.SetSpeed(0);
  } else if (!steerLeft && !steerRight) {
    PRINTLN(" driving forward");
    LeftMotor.SetSpeed(MOTOR_DRIVE_SPEED_L);
    RightMotor.SetSpeed(MOTOR_DRIVE_SPEED_R);
  } else if (steerLeft) {
    PRINTLN(" driving left");
    LeftMotor.SetSpeed(MOTOR_STEER_SPEED);
    RightMotor.SetSpeed(MOTOR_DRIVE_SPEED_R);
  } else if (steerRight) {
    PRINTLN(" driving right");
    LeftMotor.SetSpeed(MOTOR_DRIVE_SPEED_L);
    RightMotor.SetSpeed(MOTOR_STEER_SPEED);
  }
}

void MessureDistanceSensor() {
  float distance = DistanceSensor.GetDistance(Metric::Centimeter);
  obsticalInTheWay = distance < 10.0f;
  PRINT("FrontDistance: ");
  PRINT(distance);
}

void UpdateNightLight() {
  int lightIntensity = analogRead(PIN_LDR);
  int ledLightIntensity = map(constrain(lightIntensity, 600, 750), 600, 750, 255, 0);
  analogWrite(PIN_LED, ledLightIntensity);
}

void BlinkProgramStatusLED() {
  static bool programCycleHigh;
  programCycleHigh = !programCycleHigh;
  digitalWrite(LED_BUILTIN, programCycleHigh);
}