#include <DigiKeyboard.h>

/*************************************
StreetView walker
v003
**************************************/

#define CHATTERING_WAIT_PERIOD 200
#define SENSOR_PIN 0
#define LED_PIN 1
#define BUTTON_PIN 2

char sensorSide = 'right';
unsigned long upTime;
int stepTotal;
bool holeIcPast;
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

  buttonPast = false;

  pinMode(SENSOR_PIN, INPUT_PULLUP); //HOLE IC Sensor
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Button
  pinMode(LED_PIN, OUTPUT); //LED on Model A 
 
  holeIcPast = digitalRead(0);
  stepTotal = 0;

  flashled(1,1000);

}

// the loop routine runs over and over again forever:
void loop() {

  byte holeIcRead;
  byte buttonStat;
  buttonStat = 0;

  upTime = millis();
  holeIcRead = digitalRead(SENSOR_PIN);

  if(holeIcRead != holeIcPast && holeIcRead == LOW) { //ホールICが磁気反応に転じた瞬間(w送信)
    stepTotal++;
 
    DigiKeyboard.sendKeyStroke(KEY_W);
    digitalWrite(LED_PIN, HIGH);
    DigiKeyboard.delay(CHATTERING_WAIT_PERIOD); //チャタリング防止の不感時間
  }

  if(holeIcRead != holeIcPast && holeIcRead == HIGH) { //ホールICが磁気無反応に転じた瞬間
    digitalWrite(LED_PIN, LOW);
    DigiKeyboard.delay(CHATTERING_WAIT_PERIOD); //チャタリング防止の不感時間
  }

  buttonStat = digitalRead(BUTTON_PIN);

  if(buttonStat == LOW && holeIcRead == LOW && buttonPast == false) { //ホールICが磁気反応かつボタンが押された瞬間
    buttonPast = true;
    if(sensorSide == 'right'){
      DigiKeyboard.sendKeyPress(KEY_D); //本体が右設置の場合はd(右)キーを押下
    } else {
      DigiKeyboard.sendKeyPress(KEY_A); //本体が左設置の場合はa(左)キーを押下
    }
  }
  if(buttonStat == LOW && holeIcRead == HIGH && buttonPast == false) { //ホールICが磁気無反応かつボタンが押された瞬間
    buttonPast = true;
    if(sensorSide == 'right'){
      DigiKeyboard.sendKeyPress(KEY_A); //本体が右設置の場合はa(左)キーを押下
    } else {
      DigiKeyboard.sendKeyPress(KEY_D); //本体が左設置の場合はd(左)キーを押下
    }
  }

  if(buttonPast == true && buttonStat == HIGH){ //ボタンが押されなくなった瞬間はa or dキー押下をやめる
    DigiKeyboard.sendKeyPress(0,0);
    buttonPast = false;    
  }

  holeIcPast = holeIcRead;

}
