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

    void SetDirection(bool forward) {
      digitalWrite(m_In1, forward);
      digitalWrite(m_In2, !forward);
    }
    void TurnOff() {
      digitalWrite(m_In1, LOW);
      digitalWrite(m_In2, LOW);
    }

    // 0 - 255
    void SetSpeed(int rotationSpeed) {
      analogWrite(m_GSM, rotationSpeed);
    }
    
  private:
    int m_GSM, m_In1, m_In2;
};
