#ifndef ECHOSENSOR_H
#define ECHOSENSOR_H

enum class Metric {
  Centimeter,
  Meter,
  Millimeter
};

class EchoSensor {
  public:
    EchoSensor() {}

    void Init(int triggerPin, int echoPin) {
      m_TriggerPin = triggerPin;
      m_EchoPin = echoPin;
      pinMode(m_EchoPin, INPUT);
      pinMode(m_TriggerPin, OUTPUT);
    }

#define ECHO_SPEED_MM_IN_MS 0.34
#define ECHO_SPEED_CM_IN_MS 0.034
#define ECHO_SPEED_M_IN_MS 0.00034
    long GetDistance(Metric metric)  {
      digitalWrite(m_TriggerPin, LOW);
      delay(5);
      digitalWrite(m_TriggerPin, HIGH);
      delay(10);
      digitalWrite(m_TriggerPin, LOW);

      long duration = pulseIn(m_EchoPin, HIGH);
      long distance = duration / 2; // RTT
      switch (metric) {
      default:
      case Metric::Centimeter:          return distance * ECHO_SPEED_CM_IN_MS;
      case Metric::Meter:               return distance * ECHO_SPEED_M_IN_MS;
      case Metric::Millimeter:          return distance * ECHO_SPEED_MM_IN_MS;
      }
    }

  private:
    int m_TriggerPin, m_EchoPin;
};
#endif // ECHOSENSOR_H
