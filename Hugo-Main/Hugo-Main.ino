#include "EchoSensor.h"
#include "LinienSensor.h"
#include "Motor.h"

#define MOTOR_STEER_SPEED 0
#define MOTOR_DRIVE_SPEED_L 85
#define MOTOR_DRIVE_SPEED_R 92

#define DEBUG false
#define ECHO_ENABLED false

#if ECHO_ENABLED
EchoSensor DistanceSensor;
#endif
LinienSensor SteeringSensor;
Motor LeftMotor, RightMotor;

bool steerLeft = false, steerRight = false;
bool drive = true;
LinienSensorResult lenkungResult;

#define IS_LINE_THRESHOLD 150

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif

#if ECHO_ENABLED
  DistanceSensor.Init(3, 4);
#endif
  SteeringSensor.Init(A1, A2, A3);

  LeftMotor.Init(10, 9, 8, true);
  RightMotor.Init(5, 6, 7, false);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  drive = true;

#if ECHO_ENABLED
  // Print out sensor raw values for debugging
  long frontDistance = DistanceSensor.GetDistance(Metric::Centimeter);
  bool obsticalInTheWay = frontDistance < 5;  // if front distance is less than 5cm away
#endif
#if DEBUG && ECHO_ENABLED
  Serial.print("FrontDistance: ");
  Serial.print(frontDistance);
#endif

  lenkungResult = SteeringSensor.Messure();
#if DEBUG
  Serial.print("    Raw Sensor: [L: ");
  Serial.print(lenkungResult.Left);
  Serial.print(", C: ");
  Serial.print(lenkungResult.Center);
  Serial.print(", R: ");
  Serial.print(lenkungResult.Right);
  Serial.print(']');
#endif

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
    steerLeft = steerRight = false;
    drive = false;
  }

#if ECHO_ENABLED
  if (obsticalInTheWay) drive = false;
#endif

  // Apply configured driving directions to motors
  if (!drive) {
#if DEBUG
    Serial.println(" standing still");
#endif
    LeftMotor.SetSpeed(0);
    RightMotor.SetSpeed(0);
  } else if (!steerLeft && !steerRight) {
#if DEBUG
    Serial.println(" driving forward");
#endif
    LeftMotor.SetSpeed(MOTOR_DRIVE_SPEED_L);
    RightMotor.SetSpeed(MOTOR_DRIVE_SPEED_R);
  } else if (steerLeft) {
#if DEBUG
    Serial.println(" driving left");
#endif
    LeftMotor.SetSpeed(MOTOR_DRIVE_SPEED_L);
    RightMotor.SetSpeed(MOTOR_STEER_SPEED);
  } else if (steerRight) {
#if DEBUG
    Serial.println(" driving right");
#endif
    LeftMotor.SetSpeed(MOTOR_STEER_SPEED);
    RightMotor.SetSpeed(MOTOR_DRIVE_SPEED_R);
  }

  static bool high = true;
  high = !high;
  digitalWrite(LED_BUILTIN, high);
}
