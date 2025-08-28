#define MAX_ANGLE 180
#define STD_ANGLE 90
#define MIN_ANGLE 0

#include <Servo.h>

Servo mg995;
/*
mg995 모터는 별도의 power supply(5V 이상 - 7.5V 미만) 필요
파랑선은 3번 pin, 주황선은 4번 pin에 연결
동작상태를 serial monitor를 통해서 확인 가능
*/

const int servoPin = 9;
const int pin = 3;   //주황선 버튼 누르면 동작
const int STOP = 4;  //파랑선 버튼 누르면 중지
bool isMoving = false;

const int USER_DELAY = 1000;// 대기 시간 1000 = 1초, 숫자 1당 milisecond 할당
const int user_repeat = 10;  // 0이면 무한반복

void setup() {
  Serial.begin(9600);
  mg995.attach(servoPin);
  pinMode(pin, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);

  mg995.write(STD_ANGLE);
}

void one_cycle(int DELAY){
  mg995.write(MAX_ANGLE);
  delay(DELAY);
  mg995.write(STD_ANGLE);
  delay(DELAY);
  mg995.write(MIN_ANGLE);
  delay(DELAY);
  mg995.write(STD_ANGLE);
  delay(DELAY);
}

void multiple_cycle(int repeator){
  if (repeator != 0){
    for (int i = 0; i < repeator; i++){
      if(digitalRead(STOP) == HIGH){
        isMoving = false;
        Serial.write("Stopping...\n");
        break;
      }
      one_cycle(USER_DELAY);
    }
  } else {
    while(true){
      if(digitalRead(STOP) == HIGH){
        isMoving = false;
        Serial.write("Stopping...\n");
        break;
      }
      one_cycle(USER_DELAY);
    }
  }
}

void loop() {
  isMoving = false;
  if(digitalRead(pin) == HIGH && !isMoving){
    Serial.write("Operating...\n");
    isMoving = true;
    multiple_cycle(user_repeat);
  }else{
    Serial.write("waiting...\n");
    delay(1000);
  }
}
