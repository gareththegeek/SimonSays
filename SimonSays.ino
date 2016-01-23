// Include Gamer library
#include <Gamer.h>

// Create a copy of the Gamer library.
Gamer gamer;

byte UpArrow[] = {
  B00000000,
  B00011000,
  B00111100,
  B01111110,
  B00011000,
  B00011000,
  B00011000,
  B00000000
};

byte DownArrow[] = {
  B00000000,
  B00011000,
  B00011000,
  B00011000,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

byte LeftArrow[] = {
  B00000000,
  B00010000,
  B00110000,
  B01111110,
  B01111110,
  B00110000,
  B00010000,
  B00000000
};

byte RightArrow[] = {
  B00000000,
  B00001000,
  B00001100,
  B01111110,
  B01111110,
  B00001100,
  B00001000,
  B00000000
};

byte Tick[] = {
  B00000001,
  B00000011,
  B00000111,
  B00001110,
  B11011100,
  B11111000,
  B01110000,
  B00100000
};

byte Cross[] = {
  B11000011,
  B01100110,
  B00111100,
  B00011000,
  B00011000,
  B00111100,
  B01100110,
  B11000011
};

bool gameOver = true;
const int Speed = 700;

#define PossibleMoveCount 4
int PossibleMoves[] = { UP, DOWN, LEFT, RIGHT };

#define MaxMoveCount 1000
int moveCount;

int* moves;

void setup() {
  moves = (int*)malloc(MaxMoveCount);
  gamer.begin();
  int a = analogRead(A5);
  randomSeed(a);
}

void clearInputs(){
  for(int i = 0; i < PossibleMoveCount; i++){
    gamer.isPressed(PossibleMoves[i]);
  }
}

bool newGame(){
  if(gamer.isPressed(START)){
    gameOver = false;
    moveCount = 1;
    clearInputs();
    return true;
  }
  return false;
}

void splashScreen(){
  gamer.printString("Remember");
  if(newGame()) return;
  gamer.printString("Me");
  if(newGame()) return;
  delay(100);
  gamer.printString("Press");
  if(newGame()) return;
  gamer.printString("Start");
  if(newGame()) return;
  delay(100);
}

void generateMoves(){
  moves[moveCount-1] = (int)random(PossibleMoveCount);
}

void printMove(int mov){
  switch(mov){
    case UP:
      gamer.printImage(UpArrow);
      gamer.playTone(220);
      break;
    case DOWN:
      gamer.printImage(DownArrow);
      gamer.playTone(200);
      break;
    case LEFT:
      gamer.printImage(LeftArrow);
      gamer.playTone(240);
      break;
    case RIGHT:
      gamer.printImage(RightArrow);
      gamer.playTone(180);
      break;
  }
  delay(100);
  gamer.stopTone();
}

void showMoves(){
  for(int i = 0; i < moveCount; i++){
    printMove(moves[i]);
    delay(Speed);
    gamer.clear();
    delay(100);
  }
  clearInputs();
}

bool checkMoves(){
  int currentMove = 0;
  while(true){
    for(int j = 0; j < PossibleMoveCount; j++){
      if(gamer.isPressed(PossibleMoves[j])){
        printMove(PossibleMoves[j]);
        if(PossibleMoves[j] != moves[currentMove++]){
          delay(500);
          return false;
        }
        if(currentMove==moveCount){
          delay(500);
          return true;
        }
      }
    }
  }
}

void showGameOver(){
  gamer.printString("Game Over");
  delay(100);
  gamer.printString("Score");
  delay(100);
  gamer.showScore(moveCount-1);
  delay(1000);
}

void gameLoop(){
  generateMoves();
  showMoves();
  
  if(!checkMoves()) {
    gamer.setLED(true);
    gamer.printImage(Cross);
    gamer.playTone(100);
    gameOver = true;
  }else{
    moveCount += 1;
    gamer.printImage(Tick);
    gamer.playTone(300);
  }
  delay(100);
  gamer.stopTone();
  gamer.setLED(false);
  delay(2000);

  if(gameOver){
    showGameOver();
  }
}

void loop() {
  if(gameOver){
    splashScreen();
  }else{
    gameLoop();
  }
}

