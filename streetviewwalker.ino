#include <DigiKeyboard.h>

/*************************************
StreetView walker
v003
**************************************/

#define CHATTERING_WAIT_PERIOD 200
#define SENSOR_PIN 0
#define LED_PIN 1
#define BUTTON_PIN 2

bool sensorSideIsRight;
unsigned long upTime;
int stepTotal;
bool hallIcPast;
bool buttonPast;

void flashled(int times, int flashWait) {

  for (int i=1; i <= times; i++){
    digitalWrite(LED_PIN, HIGH);
    DigiKeyboard.delay(flashWait);
    digitalWrite(LED_PIN, LOW);
    DigiKeyboard.delay(flashWait);
  }
}


void setup() {

  sensorSideIsRight = true;
  buttonPast = false;

  pinMode(SENSOR_PIN, INPUT_PULLUP); //HOLE IC Sensor
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Button
  pinMode(LED_PIN, OUTPUT); //LED on Model A 
 
  hallIcPast = digitalRead(SENSOR_PIN);
  stepTotal = 0;

  flashled(1,1000);

}

// the loop routine runs over and over again forever:
void loop() {

  byte hallIcRead;
  byte buttonStat;
  buttonStat = 0;

  upTime = millis();
  hallIcRead = digitalRead(SENSOR_PIN);

  if(hallIcRead != hallIcPast && hallIcRead == HIGH) { //ホールICが磁気反応に転じた瞬間
    stepTotal++;
 
    digitalWrite(LED_PIN, HIGH);
    DigiKeyboard.delay(CHATTERING_WAIT_PERIOD); //チャタリング防止の不感時間
  }

  if(hallIcRead != hallIcPast && hallIcRead == LOW) { //ホールICが磁気無反応に転じた瞬間(w送信)
    DigiKeyboard.sendKeyStroke(KEY_W);
    
    digitalWrite(LED_PIN, LOW);
    DigiKeyboard.delay(CHATTERING_WAIT_PERIOD); //チャタリング防止の不感時間
  }

  buttonStat = digitalRead(BUTTON_PIN);

  if(buttonStat == LOW && buttonPast == false) { //ボタンが押された瞬間
    buttonPast = true;
    if(sensorSideIsRight == true ) { //磁気センサをステッパーの右側に設置した場合
      if(hallIcRead == HIGH){ //磁気センサ反応
        DigiKeyboard.sendKeyPress(KEY_D); //本体が右設置の場合はd(右)キーを押下
      }
      if(hallIcRead == LOW){ //磁気センサ無反応
        DigiKeyboard.sendKeyPress(KEY_A); //本体が右設置の場合はa(左)キーを押下
      }
    } else {  //磁気センサをステッパーの左側に設置した場合
      if(hallIcRead == HIGH){ //磁気センサ反応
        DigiKeyboard.sendKeyPress(KEY_A); //本体が右設置の場合はa(左)キーを押下
      }
      if(hallIcRead == LOW){ //磁気センサ無反応
        DigiKeyboard.sendKeyPress(KEY_D); //本体が右設置の場合はd(右)キーを押下
      }
    }
  }

  if(buttonPast == true && buttonStat == HIGH){ //ボタンが押されなくなった瞬間はa or dキー押下をやめる
    DigiKeyboard.sendKeyPress(0,0);
    buttonPast = false;    
  }

  hallIcPast = hallIcRead;

}
