#ifndef MOTOR_H
#define MOTOR_H

enum class MotorDirection {
  Forward,
  Backward
};

class Motor {
  public:
    Motor() {}

    void Init(int gsm, int in1, int in2) {
      m_GSM = gsm;
      m_In1 = in1;
      m_In2 = in2;

      pinMode(gsm, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
    }

    void SetDirection(MotorDirection motorDirection) {
      digitalWrite(m_In1, motorDirection == MotorDirection::Forward);
      digitalWrite(m_In2, motorDirection == MotorDirection::Backward);
    }
    void TurnOff() {
      digitalWrite(m_In1, LOW);
      digitalWrite(m_In2, LOW);
    }

    // 0 - 255
#define MAX_MOTOR_SPEED 255
#define MIN_MOTOR_SPEED 0
    void SetSpeed(int rotationSpeed) {
      if (rotationSpeed < MIN_MOTOR_SPEED) rotationSpeed = MIN_MOTOR_SPEED;
      else if (rotationSpeed > MAX_MOTOR_SPEED) rotationSpeed = MAX_MOTOR_SPEED;
      analogWrite(m_GSM, rotationSpeed);
    }

  private:
    int m_GSM, m_In1, m_In2;
};

#endif // MOTOR_H
