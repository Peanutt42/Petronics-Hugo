// Author: Peter Hennig

#pragma once

struct LinienSensorResult {
  int Left, Right, Center;
  LinienSensorResult(int left = 0, int right = 0, int center = 0)
    : Left(left), Right(right), Center(center) {}
};

class LinienSensor {
public:
  LinienSensor() {}

  void Init(int pinLeft, int pinCenter, int pinRight) {
    m_PinLeft = pinLeft;
    m_PinCenter = pinCenter;
    m_PinRight = pinRight;
  }

  // Read the sensor data
  const LinienSensorResult Messure() {
    return LinienSensorResult{ analogRead(m_PinLeft), analogRead(m_PinRight), analogRead(m_PinCenter) };
  }

private:
  int m_PinLeft, m_PinCenter, m_PinRight;
};