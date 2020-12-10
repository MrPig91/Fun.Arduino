/*Goal of the project
1. To make a fully functional Tic-Tac-Toe game
2. Single player mode with difficulties
3. Two Player Mode

TO DO:
[]1. Create Menu Screen
[X]2. Create Clear Board Function
[X]3. Create function for drawing X and O
[X]4. Create static grid bitmap
[X]5. Create blinking "cursor" function
[x]6. Create Winning Condition Function
[]7. Create Winning Animations
[]8. Create Single Player Mode
[]9. Create Start Menu
************************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Music
int speakerPin = 9;

int notes[] = {
   NOTE_B5, 0, NOTE_B5, 0, NOTE_B5, 0, NOTE_B5, 
  NOTE_G5, NOTE_A5, NOTE_B5, 0, NOTE_A5, NOTE_B5,
  NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_E5, 0,
  NOTE_A5, NOTE_A5, 0, NOTE_GS5, NOTE_A5,
  0, NOTE_GS5, NOTE_GS5, 0, NOTE_FS5,
  NOTE_E5, NOTE_DS5, NOTE_E5, 0, NOTE_CS5,
  NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_E5, 0,
  NOTE_A5, NOTE_A5, 0, NOTE_GS5, NOTE_A5,
  0, NOTE_GS5, NOTE_GS5, 0, NOTE_FS5,
  NOTE_E5, NOTE_FS5, NOTE_A5, 0, NOTE_B5
};

//duration of each note
int times[] = {
   53, 53, 53, 53, 53, 53, 428,
  428, 428, 107, 214, 107, 857,
  428, 428, 428, 107, 107,
  428, 107, 107, 428, 107,
  107, 428, 107, 107, 428,
  428, 428, 107, 107, 1714,
  428, 428, 428, 107, 107,
  428, 107, 107, 428, 107,
  107, 428, 107, 107, 428,
  428, 428, 107, 107, 1714
};

//MainMenu Variables
enum MainCurrentSelection {
  OnePlayer,
  TwoPlayer
};
MainCurrentSelection maincursor = OnePlayer;

enum BotDifficulty {
  Easy,
  Hard
};
BotDifficulty currentDifficulty;

enum BestOf {
  three,
  five,
  unlimited
};
BestOf currentBestOf;

int playeroneScore = 0;
int playertwoScore = 0;

//buttons
int movementButton = 3;
boolean currentmovementButtonState = LOW;
boolean lastmovementButtonState = LOW;
int selectButton = 2;
boolean currentselectButtonState = LOW;
boolean lastselectButtonState = LOW;

char allPositions[9];

enum GameSquareX {
  Left = 69,
  Middle = 91,
  Right = 113,
};

GameSquareX XPosition;
GameSquareX allXPositions[3] = {Left, Middle, Right};

enum GameSquareY {
  Top = 3,
  Center = 25,
  Bottom = 47
};

GameSquareY YPosition;
GameSquareY allYPositions[3] = {Top,Center,Bottom};

char currentPlayer;

enum CurrentScreen {
  Main,
  ModeSelection,
  BestOfScreen,
  GamePlay
};
CurrentScreen currentScreen = Main;

void setup() {
  //Serial.begin(9600);
  pinMode(movementButton, INPUT);
  pinMode(selectButton, INPUT);
  pinMode(speakerPin, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  currentPlayer = 'X';

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2);
  //DrawGameBoard();
  display.display();
  //DrawScoreBoard(playeroneScore,playertwoScore);
  //SelectSquare(Left,Top);
  DrawStartMenu();
  //Select PlayerOne Default
  display.fillRect(0,20,127,20, SSD1306_INVERSE);
  display.display();
}

void loop() {
  //Check for Buttons presses
  currentmovementButtonState = debounce(lastmovementButtonState,movementButton);
  if (lastmovementButtonState == LOW && currentmovementButtonState == HIGH){
    if (currentScreen == Main){
      MainMenuMove(maincursor);
    }
    else if (currentScreen == GamePlay){
      if (maincursor == OnePlayer && currentPlayer == 'X'){
         MoveSquare(XPosition,YPosition);
      }
      else{
         MoveSquare(XPosition,YPosition);
      }
    }
    else if (currentScreen == ModeSelection){
     SinglePlayerModeMove(currentDifficulty);
    }
    else if (currentScreen == BestOfScreen){
      BestOfMove(currentBestOf);
    }
  }
  lastmovementButtonState = currentmovementButtonState;

  currentselectButtonState = debounce(lastselectButtonState,selectButton);
  if (lastselectButtonState == LOW && currentselectButtonState == HIGH){
    if (currentScreen == Main){
      NumberofPlayersSelected(maincursor);
    }
    else if (currentScreen == GamePlay){
      if (maincursor == OnePlayer && currentPlayer == 'X'){
        SquareSelected(currentPlayer);
      }
      else{
        SquareSelected(currentPlayer);
      }
    }
    else if (currentScreen == ModeSelection){
      DifficultySelected();
    }
    else if (currentScreen == BestOfScreen){
      BestOfSelected();
    }
  }
  lastselectButtonState = currentselectButtonState;
  //int x;
  /*for (x = 0;x<=8;x++){
    Serial.print(":");
    Serial.print(x);
    Serial.print(allPositions[x]);
    Serial.println('!');
  }*/
}

boolean debounce(boolean last, int Button)
{
  boolean current = digitalRead(Button);       // Read the button state
  if (last != current)                         // If it's different…
  {
    delay(5);                                  // Wait 5ms
    current = digitalRead(Button);             // Read it again
  }
  return current;                              // Return the current value
}

void DrawXorO(char XorO, GameSquareX XSquare, GameSquareY YSquare){
      display.setCursor(XSquare,YSquare);
      display.print(XorO);
      display.display();
}

void SelectSquare(GameSquareX XSquare, GameSquareY YSquare){
  display.fillRect((XSquare - 3),(YSquare - 1), 15, 15, SSD1306_INVERSE);
  display.display();
  XPosition = XSquare;
  YPosition = YSquare;
}

void MoveSquare(GameSquareX XSquare, GameSquareY YSquare){
    display.fillRect((XSquare - 3),(YSquare - 1), 15, 15, SSD1306_INVERSE);
    display.display();
    if (XSquare == Left){
      SelectSquare(Middle,YSquare);
      XPosition = Middle;
    }
    else if (XSquare == Middle){
      SelectSquare(Right,YSquare);
      XPosition = Right;
    }
    else {
      if (YSquare == Top){
        SelectSquare(Left, Center);
        XPosition = Left;
        YPosition = Center;
      }
      else if (YSquare == Center){
        SelectSquare(Left, Bottom);
        XPosition = Left;
        YPosition = Bottom;
      }
      else{
        SelectSquare(Left, Top);
        XPosition = Left;
        YPosition = Top;
      }
    }
}

void SquareSelected(char Player){
  display.fillRect((XPosition - 3),(YPosition - 1), 15, 15, SSD1306_INVERSE);
  if (Player == 'X'){
    if (MarkMove('X',XPosition,YPosition)){
      if (CheckWinConditions()){
        playeroneScore += 1;
        if (CheckBestofCount()){
          return;
        }
        PlayVictorySong(false);
        ClearGameBoard();
        SwitchPlayer(Player);
      }
      else if (CheckForDraw()){
        ClearGameBoard();
        SwitchPlayer(Player);
      }
      else{
        SwitchPlayer(Player);
      }
    }
  }
  else{
    if (MarkMove('O',XPosition,YPosition)){
      if (CheckWinConditions()){
        playertwoScore += 1;
        if (CheckBestofCount()){
          return;
        }
        PlayVictorySong(false);
        ClearGameBoard();
        SwitchPlayer(Player);
      }
      else if (CheckForDraw()){
       ClearGameBoard();
       SwitchPlayer(Player);
      }
      else{
        SwitchPlayer(Player);
      }
    }
  }
  SelectSquare(Left,Top);
}

void DrawScoreBoard(int player1Score, int player2Score){
  display.drawFastHLine(0,18,60,SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Score");
  display.setCursor(0,22);
  display.print("P1:");
  display.print(player1Score);
  display.setCursor(0,42);
  display.print("P2:");
  display.print(player2Score);
  display.display();
}

void DrawGameBoard(){
  //Draws Game Area Vertical and Horizontal Lines
  display.drawFastVLine(84,0,64,SSD1306_WHITE);
  display.drawFastVLine(106,0,64,SSD1306_WHITE);
  display.drawFastHLine(64,20,64,SSD1306_WHITE);
  display.drawFastHLine(64,42,64,SSD1306_WHITE);
  display.display();
}

void ClearGameBoard(){
  int i;
  display.clearDisplay();
  DrawGameBoard();
  DrawScoreBoard(playeroneScore,playertwoScore);
  for (i = 0;i<=8;i++){
    allPositions[i] = NULL;
  }
}

bool MarkMove(char XorY,GameSquareX XSquare, GameSquareY YSquare){
  int XYposition = 0;
  int x;
  int y;
  for (x = 0; x <= 2; x++){
    for (y = 0; y <= 2; y++){
      if (allXPositions[x] == XSquare && allYPositions[y] == YSquare){
        if (allPositions[XYposition] == NULL){
         DrawXorO(XorY,XPosition,YPosition);
         allPositions[XYposition] = XorY;
         return true; 
        }
      }
      XYposition++;
    }
  }
  return false;
}

void SwitchPlayer(char XorYPlayer){
  if (XorYPlayer == 'X'){
    currentPlayer = 'O';
    if (maincursor == OnePlayer){
      BotMove(currentDifficulty);
    }
  }
  else{
    currentPlayer = 'X';
  }
}
  
bool CheckWinConditions(){
  //Check Columns
  int x;
  for (x = 0; x<=8; x += 3){
    if (allPositions[x] != NULL){
      if (allPositions[x] == allPositions[x+1] && allPositions[x] == allPositions[x+2]){
      return true;
      }
    }
  }
  for (x = 0; x <= 2; x++){
    if (allPositions[x] != NULL && allPositions[x] == allPositions[x + 3] && allPositions[x] == allPositions[x + 6]){
      return true;
    }
  }
  if (allPositions[0] != NULL && allPositions[0] == allPositions[4] && allPositions[0] == allPositions[8]){
    return true;
  }
  else if (allPositions[2] != NULL && allPositions[2] == allPositions[4] && allPositions[2] == allPositions[6]){
    return true;
  }
  else{
   return false; 
  }
}

bool CheckForDraw(){
  int x;
  for (x=0;x<=8;x++){
    if (allPositions[x] == NULL){
      return false;
    }
  }
  return true;
}

void PlayVictorySong(bool fullversion){
  if (fullversion){
      for (int i = 0; i < 53; i++)
      {
        tone(speakerPin, notes[i], times[i]);
        delay(times[i]);
      }
      noTone(speakerPin);
  }
  else{
      for (int i = 0; i < 13; i++)
      {
        tone(speakerPin, notes[i], times[i]);
        delay(times[i]);
      }
      noTone(speakerPin);
  }
}

void DrawStartMenu(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25,9);
  display.print("Tic-Tac-Toe");
  display.drawFastHLine(0,18,128,SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(15,24);
  display.print("1 Player");
  display.setCursor(15,44);
  display.print("2 Player");
  display.display();
}

void MainMenuMove(MainCurrentSelection current){
  if (current == OnePlayer){
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    maincursor = TwoPlayer;
  }
  else{
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    maincursor = OnePlayer;
  }
  display.display();
}

void NumberofPlayersSelected(MainCurrentSelection currentlySelected){
  if (currentlySelected == OnePlayer){
    DrawSinglePlayerModeSelectionScreen();
  }
  else{
    DrawBestOfScreen();
  }
}

void DrawSinglePlayerModeSelectionScreen(){
  currentScreen = ModeSelection;
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Difficulty");
  display.drawFastHLine(0,18,128,SSD1306_WHITE);
  display.setCursor(30,24);
  display.print("Easy");
  display.setCursor(30,44);
  display.print("Hard");
  display.fillRect(0,20,127,20, SSD1306_INVERSE);
  display.display();
  currentDifficulty = Easy;
}

void SinglePlayerModeMove(BotDifficulty current){
  if (current == Easy){
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    currentDifficulty = Hard;
  }
  else{
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    currentDifficulty = Easy;
  }
  display.display();
}

void DifficultySelected(){
  DrawBestOfScreen();
}

void DrawBestOfScreen(){
  currentBestOf = three;
  currentScreen = BestOfScreen;
  display.clearDisplay();
  display.setCursor(5,0);
  display.print("Best of 3");
  display.setCursor(5,24);
  display.print("Best of 5");
  display.setCursor(5,44);
  display.print("Unlimited");
  display.fillRect(0,0,127,20, SSD1306_INVERSE);
  display.display();
}
void BestOfMove(BestOf currentBest){
    if (currentBest == three){
    display.fillRect(0,0,127,20, SSD1306_INVERSE);
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    currentBestOf = five;
  }
  else if (currentBest == five){
    display.fillRect(0,20,127,20, SSD1306_INVERSE);
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    currentBestOf = unlimited;
  }
  else{
    display.fillRect(0,40,127,20, SSD1306_INVERSE);
    display.fillRect(0,0,127,20, SSD1306_INVERSE);
    currentBestOf = three;
  }
  display.display();
}
void BestOfSelected(){
    playeroneScore = 0;
    playertwoScore = 0;
    ClearGameBoard();
    currentScreen = GamePlay;
    SelectSquare(Left,Top);
}

void BotMove(BotDifficulty difficulty){
  if (difficulty == Easy){
    int randomNumber = random(0,8);
    TranslateLocation(randomNumber);
    display.fillRect((XPosition - 3),(YPosition - 1), 15, 15, SSD1306_INVERSE);
    SquareSelected(currentPlayer);
    if (currentPlayer == 'O'){
      BotMove(Easy);
    }
    display.fillRect((XPosition - 3),(YPosition - 1), 15, 15, SSD1306_INVERSE);
  }
  else if (difficulty == Hard){
    int value = BotMoveHard();
    if (value == 9){
      BotMove(Easy);
    }
    else{
      TranslateLocation(value);
      display.fillRect((XPosition - 3),(YPosition - 1), 15, 15, SSD1306_INVERSE);
      SquareSelected(currentPlayer);
      display.fillRect((XPosition - 3),(YPosition - 1), 15, 15, SSD1306_INVERSE);
    }
  }
}

bool CheckBestofCount(){
  if (currentBestOf == three){
    if (playeroneScore == 2){
      ReturnToMainMenu();
      return true;
    }
    else if (playertwoScore == 2){
      ReturnToMainMenu();
      return true;
    }
  }
  else if (currentBestOf == five){
        if (playeroneScore == 3){
      ReturnToMainMenu();
      return true;
    }
    else if (playertwoScore == 3){
      ReturnToMainMenu();
      return true;
    }
  }
  return false;
}

void ReturnToMainMenu(){
  int i;
  display.clearDisplay();
    for (i = 0;i<=8;i++){
    allPositions[i] = NULL;
  }
  DrawWinner();
  currentPlayer = 'X';
  currentScreen = Main;
  display.clearDisplay();
  display.stopscroll();
  DrawStartMenu();
  display.fillRect(0,20,127,20, SSD1306_INVERSE);
  display.display();
}

void DrawWinner(){
  display.clearDisplay();
  display.setCursor(10,10);
  if (currentPlayer == 'X'){
    display.println("Player X");
    display.setCursor(30,30);
    display.println("Wins!!");
    display.display();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    PlayVictorySong(true);
  }
  else{
    display.print("Player O");
    display.setCursor(30,30);
    display.println("Wins!!");
    display.display();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    PlayVictorySong(true);
  }
}

int BotMoveHard(){
  int x;
  for (x = 0; x<=8; x += 3){
      if (allPositions[x] == NULL && allPositions[x+1] == allPositions[x+2] && allPositions[x+1] != NULL){
        return x;
      }
      else if (allPositions[x+1] == NULL && allPositions[x] == allPositions[x+2] && allPositions[x] != NULL){
        return x+1;
      }
      else if (allPositions[x+2] == NULL && allPositions[x] == allPositions[x+1] && allPositions[x] != NULL){
        return x+2;
      }
  }
  for (x = 0; x <= 2; x++){
    if (allPositions[x] == NULL && allPositions[x+3] == allPositions[x + 6] && allPositions[x+3] == NULL){
      return x;
    }
    else if (allPositions[x+3] == NULL && allPositions[x] == allPositions[x+6] && allPositions[x] != NULL){
        return x+3;
    }
    else if (allPositions[x+6] == NULL && allPositions[x] == allPositions[x+3] && allPositions[x] != NULL){
        return x+6;
    }
  }
  if (allPositions[0] == NULL && allPositions[4] == allPositions[8] && allPositions[4] != NULL){
    return 0;
  }
  else if (allPositions[4] == NULL && allPositions[0] == allPositions[8] && allPositions[0] != NULL){
    return 4;
  }
  else if (allPositions[8] == NULL && allPositions[0] == allPositions[4] && allPositions[4] != NULL){
    return 8;
  }
  else if (allPositions[2] == NULL && allPositions[4] == allPositions[6] && allPositions[4] != NULL){
    return 2;
  }
  else if (allPositions[4] == NULL && allPositions[2] == allPositions[6] && allPositions[2] != NULL){
    return 4;
  }
  else if (allPositions[6] == NULL && allPositions[2] == allPositions[4] && allPositions[4] != NULL){
    return 6;
  }
  return 9;
}
void TranslateLocation(int x){
  switch (x){
    case 0:
      XPosition = allXPositions[0];
      YPosition = allYPositions[0];
      break;
    case 1:
      XPosition = allXPositions[0];
      YPosition = allYPositions[1];
      break;
    case 2:
      XPosition = allXPositions[0];
      YPosition = allYPositions[2];
      break;
    case 3:
      XPosition = allXPositions[1];
      YPosition = allYPositions[0];
      break;
    case 4:
      XPosition = allXPositions[1];
      YPosition = allYPositions[1];
      break;
    case 5:
      XPosition = allXPositions[1];
      YPosition = allYPositions[2];
      break;
    case 6:
      XPosition = allXPositions[2];
      YPosition = allYPositions[0];
      break;
    case 7:
      XPosition = allXPositions[2];
      YPosition = allYPositions[1];
      break;
    case 8:
      XPosition = allXPositions[2];
      YPosition = allYPositions[2];
      break;
  }
}