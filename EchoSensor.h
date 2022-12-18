// Author: Peter Hennig

#pragma once

enum class Metric {
  Centimeter,
  Meter,
  Millimeter
};

class EchoSensor {
public:
  EchoSensor() {}

  void Init(int triggerPin, int echoPin, int maxDistance) {
    m_TriggerPin = triggerPin;
    m_EchoPin = echoPin;
    m_MaxDistance = maxDistance;
    pinMode(m_EchoPin, INPUT);
    pinMode(m_TriggerPin, OUTPUT);
  }

#define ECHO_SPEED_MM_IN_MS 0.34
#define ECHO_SPEED_CM_IN_MS 0.034
#define ECHO_SPEED_M_IN_MS 0.00034
  float GetDistance(Metric metric) {
    digitalWrite(m_TriggerPin, LOW);
    delay(5);
    digitalWrite(m_TriggerPin, HIGH);
    delay(10);
    digitalWrite(m_TriggerPin, LOW);

    long duration = pulseIn(m_EchoPin, HIGH);
    if (duration == 0) return m_MaxDistance;
    float distance = (float)(duration / 2);  // RTT
    switch (metric) {
      default:
      case Metric::Centimeter: return distance * ECHO_SPEED_CM_IN_MS;
      case Metric::Meter: return distance * ECHO_SPEED_M_IN_MS;
      case Metric::Millimeter: return distance * ECHO_SPEED_MM_IN_MS;
    }
  }

  int GetMaxDistance() const {
    return m_MaxDistance;
  }
  void SetMaxDistance(int newMaxDistance) {
    m_MaxDistance = newMaxDistance;
  }

private:
  int m_TriggerPin, m_EchoPin;
  int m_MaxDistance;
};
