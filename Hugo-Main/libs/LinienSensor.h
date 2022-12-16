#ifndef LINIENSENSOR_H
#define LINIENSENSOR_H

struct LinienSensorResult {
  int Left, Right, Center;
  LinienSensorResult(int left = 0, int right = 0, int center = 0)
    : Left(left), Right(right), Center(center) {}
};

class LinienSensor {
public:
  LinienSensor() {}

  void Init(int pinA1, int pinA2, int pinA3) {
    m_PinA1 = pinA1;
    m_PinA2 = pinA2;
    m_PinA3 = pinA3;
  }

  LinienSensorResult Messure() {
    return LinienSensorResult{ analogRead(m_PinA1), analogRead(m_PinA3), analogRead(m_PinA2) };
  }

private:
  int m_PinA1, m_PinA2, m_PinA3;
};

#endif  // LINIENSENSOR_H
