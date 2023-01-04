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
#define PIN_ECHO_POWER 12
#define PIN_PARKING_ECHO_TRIGGER A4
#define PIN_PARKING_ECHO_ECHO A5
#define PIN_PARKING_ECHO_POWER 13

#define PIN_STEERING_LEFT A1
#define PIN_STEERING_CENTER A2
#define PIN_STEERING_RIGHT A3

#define PIN_MOTOR_L_GSM 5
#define PIN_MOTOR_L_IN1 6
#define PIN_MOTOR_L_IN2 7
#define PIN_MOTOR_R_GSM 10
#define PIN_MOTOR_R_IN1 9
#define PIN_MOTOR_R_IN2 8
#pragma endregion  // Pins

#pragma region Configuration
#define DEBUG false
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
#pragma endregion  // Configuration

#pragma region Sensors
LinienSensor SteeringSensor;
Motor LeftMotor, RightMotor;
EchoSensor DistanceSensor, ParkingSensor;
#pragma endregion  // Sensors

#pragma region Driving Commands
bool steerLeft = false, steerRight = false;
bool drive = true;
bool obsticalInTheWay = false;
#pragma endregion  // Driving Commands

int updateDistanceSensorCycles;

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif

  DistanceSensor.Init(PIN_ECHO_TRIGGER, PIN_ECHO_ECHO, 50);
  ParkingSensor.Init(PIN_PARKING_ECHO_TRIGGER, PIN_PARKING_ECHO_ECHO, 50);

  SteeringSensor.Init(PIN_STEERING_LEFT, PIN_STEERING_CENTER, PIN_STEERING_RIGHT);

  LeftMotor.Init(PIN_MOTOR_L_GSM, PIN_MOTOR_L_IN1, PIN_MOTOR_L_IN2);
  RightMotor.Init(PIN_MOTOR_R_GSM, PIN_MOTOR_R_IN1, PIN_MOTOR_R_IN2);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_ECHO_POWER, OUTPUT);
  digitalWrite(PIN_ECHO_POWER, HIGH);
  pinMode(PIN_PARKING_ECHO_POWER, OUTPUT);
  digitalWrite(PIN_PARKING_ECHO_POWER, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  drive = true;

  ReadButton();

  UpdateCycledPrograms();

  UpdateNightLight();

  ReadSteeringSensor();

  SetMotors();

  BlinkProgramStatusLED();
}

// Read if button was pressed -> parking mode
void ReadButton() {
  bool buttonState = digitalRead(PIN_BUTTON) == LOW;
  static bool buttonLastState;
  static bool stopCar;
  if (buttonState != buttonLastState) {
    buttonLastState = buttonState;
    if (buttonState) {
      delay(1000);
      digitalWrite(PIN_ECHO_POWER, LOW);
      digitalWrite(PIN_PARKING_ECHO_POWER, HIGH);
      Park(&LeftMotor, &RightMotor, &ParkingSensor);  //stopCar = !stopCar;      
      digitalWrite(PIN_ECHO_POWER, HIGH);
      digitalWrite(PIN_PARKING_ECHO_POWER, LOW);
    }
  }
  if (stopCar) drive = false;
}

// Update programs that should run every 3./10. loop()-function
void UpdateCycledPrograms() {
  updateDistanceSensorCycles++;
  if (updateDistanceSensorCycles > 3) updateDistanceSensorCycles = 0;
  if (updateDistanceSensorCycles == 0) MessureDistanceSensor();

  if (obsticalInTheWay) drive = false;
}

// Read SteeringSensor and decide in which direction to drive
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

// Apply driving direction to motors
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

// Read out the Front-DistanceSensor
void MessureDistanceSensor() {
  float distance = DistanceSensor.GetDistance(Metric::Centimeter);
  obsticalInTheWay = distance < 10.0f;
  PRINT("FrontDistance: ");
  PRINT(distance);
}

// Set Night-Light based on how bright it is outside
void UpdateNightLight() {
  int lightIntensity = analogRead(PIN_LDR);
  int ledLightIntensity = map(constrain(lightIntensity, 600, 750), 600, 750, 255, 0);
  analogWrite(PIN_LED, ledLightIntensity);
}

// Toggle the builtin-Led every time the loop()-function runs once (performance-profiling)
void BlinkProgramStatusLED() {
  static bool programCycleHigh;
  programCycleHigh = !programCycleHigh;
  digitalWrite(LED_BUILTIN, programCycleHigh);
}