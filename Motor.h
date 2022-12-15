#ifndef MOTOR_H
#define MOTOR_H

enum class MotorDirection {
  Forward,
  Backward
};

class Motor {
  public:
    Motor() {}

    void Init(int gsm, int in1, int in2, bool builtinReverse = false) {
      m_GSM = gsm;
      m_In1 = in1;
      m_In2 = in2;
      m_BuiltinReverse = builtinReverse;

      pinMode(m_GSM, OUTPUT);
      pinMode(m_In1, OUTPUT);
      pinMode(m_In2, OUTPUT);

      SetDirection(MotorDirection::Forward);
    }

    void SetDirection(MotorDirection motorDirection) {
      bool forward = motorDirection == MotorDirection::Forward;
      if (m_BuiltinReverse) forward = !forward;
      digitalWrite(m_In1, forward);
      digitalWrite(m_In2, !forward);
    }

    void TurnOn() {
      SetDirection(MotorDirection::Forward);
    }

    void TurnOff() {
      digitalWrite(m_In1, LOW);
      digitalWrite(m_In2, LOW);
    }

#define MAX_MOTOR_SPEED 255
#define MIN_MOTOR_SPEED 85
    void SetSpeed(int rotationSpeed) {
      if (rotationSpeed < MIN_MOTOR_SPEED) {
        if (rotationSpeed < 42) rotationSpeed = 0;
        else if (rotationSpeed > 42) rotationSpeed = 42;
      }
      rotationSpeed = constrain(rotationSpeed, 0, MAX_MOTOR_SPEED);
      analogWrite(m_GSM, rotationSpeed);
    }

  private:
    int m_GSM, m_In1, m_In2;
    bool m_BuiltinReverse;
};

#endif // MOTOR_H
