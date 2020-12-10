#include <TimerOne.h>

int buttons[2] = {8,9};
int Main_LED = 10;
int player;

boolean currenstate[2] = {LOW, LOW};
boolean laststate[2] = {LOW, LOW};
boolean buttonsPressed[2] = {false, false};
boolean countDownStarted = false;
boolean Led_State = HIGH;

int toggle = 0;

unsigned long time;
long waitTime;
long startTime;
long endTime;
long playerTimer;

bool gameStarted = false;


void setup(){
    Serial.begin(9600);
    pinMode(buttons[0], INPUT);
    pinMode(buttons[1], INPUT);
    pinMode(Main_LED, OUTPUT);
    randomSeed(analogRead(0));
    StartGame();
}

void loop(){
  currenstate[toggle] = debounce(currenstate[toggle],buttons[toggle]);
  if (currenstate[toggle] == LOW && laststate[toggle] == HIGH){
      if (gameStarted && !buttonsPressed[toggle]){
        playerTimer = millis() - startTime;
        Led_State = !Led_State;
        digitalWrite(Main_LED, Led_State);
        player = toggle + 1;
        Serial.print("Player ");
        Serial.print(player);
        Serial.print(": ");
        Serial.print(playerTimer);
        Serial.println("miliseconds");
        buttonsPressed[toggle] = true;
      }
      else if (!buttonsPressed[toggle]){
        playerTimer = millis() - startTime;
        Serial.print("Player ");
        Serial.print(player);
        Serial.print(": ");
        Serial.print(playerTimer);
        Serial.println(" miliseconds");
        buttonsPressed[toggle] = true;
      }
      if (buttonsPressed[0] && buttonsPressed[1]){
        gameStarted = false;
        StartGame();
      }
  }
  laststate[toggle] = currenstate[toggle];
  toggle = !toggle;
  if (millis() > endTime && countDownStarted){
    Led_State = HIGH;
    digitalWrite(Main_LED, Led_State);
    startTime = millis();
    countDownStarted = false;
  }
  if (gameStarted == false){
    StartGame();
  }
}

boolean debounce(boolean last, int Button)
{
  boolean current = digitalRead(Button);       
  if (last != current)                         
  {
    delay(5);                                
    current = digitalRead(Button);            
  }
  return current;
}

void StartGame(){
  gameStarted = true;
  Led_State = LOW;
  digitalWrite(Main_LED, Led_State);
  waitTime = random(1000,5000);
  startTime = millis();
  endTime = startTime + waitTime;
  countDownStarted = true;
  buttonsPressed[0] = false;
  buttonsPressed[1] = false;
}

void blinky(){
  Led_State = !Led_State;
  digitalWrite(Main_LED, Led_State);
}