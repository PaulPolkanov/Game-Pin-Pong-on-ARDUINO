#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//Initializing the display and constants for the game
Adafruit_SSD1306 display(128, 64, &Wire, -1);
const int screen_width = 127;
const int screen_height = 63;
const int gamer_height = 16;
unsigned long timer_game = 0;

//State game 
bool stateGame = false;

//Score Players
int counterGamer1 = 0;
int counterGamer2 = 0;

//Nomber port input value from potentiometer 
//for chang position Y player 1 and 2 
byte controlPlayer_1 = 33;
byte controlPlayer_2 = 14;


//Port and status for Led Win Players
bool WinPl_2StateLed = true;
int LedWinPl_2 = 23;
bool WinPl_1StateLed = true;
int LedWinPl_1 = 26;

//Ball's properties
int ball_X = 64;
int ball_Y = 32;
int ball_radius = 2;
int ball_dir_X = 2;
int ball_dir_Y = 2;


//Players' properties
int gamer1_X = 7;
int gamer1_Y = 26;
int gamer2_X = 121;
int gamer2_Y = 26;

void setup()
{
  pinMode(controlPlayer_1, INPUT);
  pinMode(controlPlayer_2, INPUT);
  pinMode(LedWinPl_1, OUTPUT);
  pinMode(LedWinPl_2, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}
//Game's logical 
void loop()
{//Control of the game 
  //by tracking the score of the players
  if(counterGamer1 < 9 && counterGamer2 < 9){
    //Loader for game
    if(stateGame == false){
      display.setCursor(30, 32);
      display.println("Starting...");
      if(millis()-timer_game > 5000){
         display.clearDisplay();
         stateGame = true;
      }
    }
    else{
      //start game
      gamer1_Y = analogRead(controlPlayer_1)/66+1;
      gamer2_Y = analogRead(controlPlayer_2)/66+1;
      //Chang ball's position every 1 ms
      if(millis() - timer_game >= 1){
        //chang ball's position 
        ball_X += ball_dir_X;
        ball_Y += ball_dir_Y;
        
        //The ball bounces off field's botton 
        if(ball_X + ball_radius > screen_width){
          ball_dir_X = -2;
        }
        //The ball bounces off field's right
        if(ball_Y + ball_radius > screen_height){
          ball_dir_Y = -2;
        }
        //The ball bounces off player 1
        if(((ball_X + ball_radius == gamer1_X+1) && (ball_X + ball_radius == gamer1_Y+1)) || ((ball_Y + ball_radius >= gamer1_Y) && (ball_Y + ball_radius <= gamer1_Y + gamer_height) && (ball_X + ball_radius == gamer1_X+1))){
          ball_dir_X = -ball_dir_X;
        }
        //The ball bounces off player 2
        if((ball_X + ball_radius == gamer2_X+1 && (ball_X + ball_radius == gamer2_Y+1)) || ((ball_Y + ball_radius >= gamer2_Y) && (ball_Y + ball_radius <= gamer2_Y + gamer_height) && (ball_X + ball_radius == gamer2_X+1))){
          ball_dir_X = -ball_dir_X;
        }
        //The ball bounces off field's left 
        if(ball_X < 1){
          ball_dir_X = 2;
        }
        //The ball bounces off field's top
        if(ball_Y < 2){
          ball_dir_Y = 2;
        }
        //Chang Player's 1 Score and ball - starting position
        if(ball_X == 0){
          counterGamer1++;
          ball_X = 64;
          ball_Y = 32;
        }
         //Chang Player's 2 Score and ball - starting position
        if(ball_X == 126){
          counterGamer2++;
          ball_X = 64;
          ball_Y = 32;
        }
        display.clearDisplay();
        //draw objects: Field, ball, and Players
        display.drawRect(0, 0, 128, 64, WHITE);
        display.fillCircle(ball_X, ball_Y, ball_radius, WHITE);
        display.drawFastVLine(gamer1_X, gamer1_Y, gamer_height, WHITE);
        display.drawFastVLine(gamer2_X, gamer2_Y, gamer_height, WHITE);
        //draw Players' Score
        display.setCursor(58, 2);
        display.print(counterGamer1);
        display.print(":");
        display.print(counterGamer2);
        display.display();
        //Update timer
        timer_game = millis();
     }
    }
  }
  else{
    display.clearDisplay();
    //Win 1 or 2 player and change state led 
    if(counterGamer1 < 9){
      digitalWrite(LedWinPl_1, WinPl_1StateLed);
      display.setCursor(25, 32);
      display.println("Win player 1");
    }
   if(counterGamer2 < 9){
      digitalWrite(LedWinPl_2, WinPl_2StateLed);
      display.setCursor(25, 32);
      display.println("Win Player 2");
    }
  }
   display.display();
}
