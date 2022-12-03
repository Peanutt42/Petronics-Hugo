#include "EchoSensor.h"
#include "LinienSensor.h"
#include "Motor.h"

EchoSensor Echo;
LinienSensor LenkungSensor;
Motor LeftMotor, RightMotor;
#define MAX_SPEED 256
#define STEER_SPEED 100

bool steerLeft, steerRight;

void setup() {
  Serial.begin(9600);

  Echo.Init(3, 4);
  LenkungSensor.Init(1, 2, 3);

  LeftMotor.Init(10, 9, 8);
  RightMotor.Init(5, 6, 7);
  LeftMotor.SetDirection(true);
  RightMotor.SetDirection(true);
}


void loop() {
  long frontDistance = Echo.GetDistance(Metric::Centimeter);
  Serial.print("FrontDistance: ");
  Serial.print(frontDistance);
  Serial.print("                       ");

  auto lenkungResult = LenkungSensor.Messure();
  Serial.print("LenkungResult(L, C, R): ");
  Serial.print(lenkungResult.Left);
  Serial.print(", ");
  Serial.print(lenkungResult.Center);
  Serial.print(", ");
  Serial.println(lenkungResult.Right);

  if (IsLine(lenkungResult.Center)) {
    steerLeft = false;
    steerRight = false;
  }
  else if  (IsLine(lenkungResult.Left)) {
    steerLeft = true;
    steerRight = false;
  }
  else if (IsLine(lenkungResult.Right)) {
    steerLeft = false;
    steerRight = true;
  }

  LeftMotor.SetSpeed(steerLeft ? STEER_SPEED : MAX_SPEED);
  RightMotor.SetSpeed(steerLeft ? STEER_SPEED : MAX_SPEED);

  if (!steerLeft && !steerRight)
    Serial.println("Now going forward");
  else if (steerLeft)
    Serial.println("Now steering left");
  else if (steerRight)
    Serial.println("Now steering right");

    delay(150);  // Only for Serial output reading, remove on release
}

inline bool IsLine(int messuredValue) {
#define IS_BLACK_THRESHOLD 60 // Testen!!!
  return messuredValue < IS_BLACK_THRESHOLD;
}
