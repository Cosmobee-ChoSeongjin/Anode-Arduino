#include <Arduino.h>

const int pwmPinA = 9;  // 게이트 드라이버 HO 핀
const int sdPin = 2;    // SD 핀

void setup() {
  pinMode(pwmPinA, OUTPUT);
  pinMode(sdPin, OUTPUT);
  digitalWrite(sdPin, LOW);  // 초기 상태: 드라이버 비활성화

  // 타이머 1 설정
  TCCR1A = _BV(COM1A1) | _BV(WGM11);         // 고속 PWM 모드, 비반전 출력
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // 분주값 없음
  ICR1 = 319;  // 50kHz 주파수

  OCR1A = 0;   // 초기 듀티 0%

  Serial.begin(9600);  // 시리얼 통신 초기화
  Serial.println("Enter 'on' to turn ON, 'off' to turn OFF");
  Serial.println("Enter duty cycle (0-160) to adjust");
}

void loop() {
  // 시리얼 통신을 통해 On/Off 및 듀티비 조정 명령 수신
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.equalsIgnoreCase("on")) {
      digitalWrite(sdPin, HIGH); // 드라이버 활성화
      Serial.println("Driver ON");

      // 소프트 스타트: 듀티비 점진적 증가
      for (int duty = 0; duty <= 160; duty++) { // 0%에서 50%까지 증가
        OCR1A = duty;
        delay(10);  // 증가 속도 조정 (10ms)
      }
    } else if (input.equalsIgnoreCase("off")) {
      digitalWrite(sdPin, LOW); // 드라이버 비활성화
      OCR1A = 0;  // 듀티비 초기화
      Serial.println("Driver OFF");
    } else {
      int duty = input.toInt();
      if (duty >= 0 && duty <= 160) {
        OCR1A = duty; // 듀티비 조정
        Serial.print("Duty cycle set to: ");
        Serial.println(duty);
      } else {
        Serial.println("Invalid duty cycle. Enter a value between 0 and 160.");
      }
    }
  }
}
