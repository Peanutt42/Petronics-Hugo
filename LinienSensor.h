#ifndef LINIENSENSOR_H
#define LINIENSENSOR_H

struct LinienSensorResult {
  int Left, Right, Center;
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
      LinienSensorResult result;
      result.Left = analogRead(m_PinA1);
      result.Center = analogRead(m_PinA2);
      result.Right = analogRead(m_PinA3);
      return result;
    }

  private:
    int m_PinA1, m_PinA2, m_PinA3;
};

#endif // LINIENSENSOR_H
