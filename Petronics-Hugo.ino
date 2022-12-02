#include "EchoSensor.h"
#include "LinienSensor.h"
#include "Motor.h"

EchoSensor Echo;
LinienSensor LenkungSensor;
Motor LeftMotor, RightMotor;

void setup() {
    Serial.begin(9600);

    Echo.Init(3, 4);
    //LenkungSensor.Init(1, 2, 3);

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

    /*auto lenkungResult = LenkungSensor.Messure();
    Serial.print("LenkungResult(LCR): ");
    Serial.print(lenkungResult.Left);
    Serial.print(", ");
    Serial.print(lenkungResult.Center);
    Serial.print(", ");
    Serial.println(lenkungResult.Right);*/

    LeftMotor.SetSpeed(200);
    RightMotor.SetSpeed(50);
}
