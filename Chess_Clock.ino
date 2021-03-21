/*
  ARDUINO CHESS CLOCK
  VITOR AMORIM FRÓIS
  vitor0frois@gmail.com
  03/2021

  build to be used with a Arduino Nano and a LCD 240x320 display
  soon on GitHub and Youtube hehe
*/

//frois libraries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/FreeSans18pt7b.h"
#include "wireOne20pt7b.h"
#include "WireOne16pt7b.h"
#include "playPauseButton.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_RST 8
#define TFT_MISO 12

//SPI CONNECTIONS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#include <EEPROM.h>

//defining seconds count
#define SEC (60 +(millis() / 1000))

//Configuração dos botões
const int whiteButton = 5;
const int blackButton = 6;
const int pauseButton = 4;
int whiteButtonState = 0; 
int blackButtonState = 0;
int pauseButtonState = 0;


char caractere;

//Variables for keypad and menu
int newMenuCount = 1;
int menuCount;
int blinkTime;
int initBlinkTime;
int blinkDelay = 500;
bool exitMenu = false;
bool sidePlayer = true;
bool blinkState = true;

//Variables for seconds count
int cTemp = 0;
int cSecWhite = 0;
int cSecBlack = 0;

//Increment variable
int setInc;

//Variables to white timer
int setSecWhite;
int horWhite;
int minWhite;
int secWhite;
int modeWhite;
int oldMinWhite;
int oldSecWhite;
int oldHorWhite;
int newModeWhite;
int totalSecWhite;

//Variables to black timer
int setSecBlack;
int horBlack;
int minBlack;
int secBlack;
int modeBlack;
int oldMinBlack;
int oldSecBlack;
int oldHorBlack;
int newModeBlack;
int totalSecBlack;

//set up menu
char menuInput;
void menuSetUp() {
  horWhite = 0;
  minWhite = 3;
  setInc = 2;
  secWhite = setInc;
}

//printing white values to LCD and print timer to set up menu
void printTimerWhite() {
  tft.setFont(&wireOne20pt7b);
  tft.setTextSize(4);
  tft.setFont(&wireOne20pt7b);
  if (newModeWhite == 2) { // = MIN SEGUNDO
    //min
    if (oldMinWhite != minWhite) {
      if ((minWhite < 10) && (minWhite >= 0)) {
        tft.fillRect(2, 5, 100, 110, ILI9341_BLACK);
        tft.setCursor(2, 110);//minutos
        tft.println("0");
        tft.setCursor(60, 110);//minutos
        tft.println(minWhite);
        oldMinWhite = minWhite;
      }
      else {
        tft.fillRect(2,5, 100, 110, ILI9341_BLACK);
        tft.setCursor(2, 110);//minutos
        tft.println(minWhite);
        oldMinWhite = minWhite;
      }
    }

    //sec
    if (oldSecWhite != secWhite) {
      if ((secWhite < 10) && (secWhite >= 0)) {
        tft.fillRect(120, 5, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 110);//minutos
        tft.println("0");
        tft.setCursor(175, 110);//minutos
        tft.println(secWhite);
        oldSecWhite = secWhite;
      }
      else {
        tft.fillRect(120, 5, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 110);//minutos
        tft.println(secWhite);
        oldSecWhite = secWhite;
      }
    }
  }
  else { // = HORA MIN
    //hor
    if (oldHorWhite != horWhite) {
      tft.fillRect(2, 5, 100, 110, ILI9341_BLACK);
      tft.setCursor(2, 110); //horas
      tft.println("0");
      tft.setCursor(60, 110); //horas
      tft.println(horWhite);
      oldHorWhite = horWhite;
    }
    //min
    if (oldMinWhite != minWhite) {
      if ((minWhite < 10) && (minWhite >= 0)) {
        tft.fillRect(120, 5, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 110);//minutos
        tft.println("0");
        tft.setCursor(175, 110);//minutos
        tft.println(minWhite);
        oldMinWhite = minWhite;
      }
      else {
        tft.fillRect(120, 5, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 110);//minutos
        tft.println(minWhite);
        oldMinWhite = minWhite;
      }
    }
  }
}

//printing black values to LCD
void printTimerBlack() {
  tft.setFont(&wireOne20pt7b);
  tft.setTextSize(4);
  if (newModeBlack == 2) { // = MIN SEGUNDO
    //min
    if (oldMinBlack != minBlack) {
      if ((minBlack < 10) && (minBlack >= 0)) {
        tft.fillRect(2, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(2, 230);//minutos
        tft.println("0");
        tft.setCursor(60, 230);//minutos
        tft.println(minBlack);
        oldMinBlack = minBlack;
      }
      else {
        tft.fillRect(2, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(2, 230);//minutos
        tft.println(minBlack);
        oldMinBlack = minBlack;
      }
    }
    //sec
    if (oldSecBlack != secBlack) {
      if ((secBlack < 10) && (secBlack >= 0)) {
        tft.fillRect(120, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 230);//minutos
        tft.println("0");
        tft.setCursor(175, 230);//minutos
        tft.println(secBlack);
        oldSecBlack = secBlack;
      }
      else {
        tft.fillRect(120, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 230);//minutos
        tft.println(secBlack);
        oldSecBlack = secBlack;
      }
    }

  }
  else { // = HORA MIN
    //hor
    if (oldHorBlack != horBlack) {
      tft.fillRect(2, 125, 100, 110, ILI9341_BLACK);
      tft.setCursor(2, 230); //horas
      tft.println("0");
      tft.setCursor(60, 230); //horas
      tft.println(horBlack);
      oldHorBlack = horBlack;
    }
    //min
    if (oldMinBlack != minBlack) {
      if ((minBlack < 10) && (minBlack >= 0)) {
        tft.fillRect(120, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 230);//minutos
        tft.println("0");
        tft.setCursor(175, 230);//minutos
        tft.println(minBlack);
        oldMinBlack = minBlack;
      }
      else {
        tft.fillRect(120, 125, 100, 110, ILI9341_BLACK);
        tft.setCursor(125, 230);//minutos
        tft.println(minBlack);
        oldMinBlack = minBlack;
      }
    }
  }
}

//MENU FUNCTIONS
//print menu set up
void menuSetup() {
  if(digitalRead(blackButton) == LOW){
    newMenuCount--;
    if(newMenuCount < 1){
      newMenuCount = 8;
    }
    delay(250);
  }
  if(digitalRead(whiteButton) == LOW){
    newMenuCount++;
    if(newMenuCount > 8){
      newMenuCount = 1;
    }
    delay(250);
  }
  if(digitalRead(pauseButton) == HIGH){
    exitMenu = true;
  }
  
//  if (Serial.available()) {
//    menuInput = Serial.read();
//    switch (menuInput) {
//      case 'w':
//        newMenuCount++;
//        if (newMenuCount > 8) {
//          newMenuCount = 1;
//        }
//        break;
//      case 'p':
//        exitMenu = true;
//        break;
//      case 'b':
//        newMenuCount--;
//        if (newMenuCount < 1) {
//          newMenuCount = 8;
//        }
//        break;
//    }
//  }
}

void switchMenu() {
  if (newMenuCount != menuCount) {
    switch (newMenuCount) {
      case 1:
        horWhite = 0;
        minWhite = 3;
        setInc = 2;
        tft.fillCircle(150, 200, 10, ILI9341_BLACK);
        tft.drawCircle(150, 200, 10, ILI9341_WHITE);
        tft.fillCircle(15, 80, 10, ILI9341_WHITE);
        tft.fillCircle(15, 120, 10, ILI9341_BLACK);
        tft.drawCircle(15, 120, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 2:
        horWhite = 0;
        minWhite = 5;
        setInc = 0;
        tft.fillCircle(15, 80, 10, ILI9341_BLACK);
        tft.drawCircle(15, 80, 10, ILI9341_WHITE);
        tft.fillCircle(15, 120, 10, ILI9341_WHITE);
        tft.fillCircle(15, 160, 10, ILI9341_BLACK);
        tft.drawCircle(15, 160, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 3:
        horWhite = 0;
        minWhite = 5;
        setInc = 3;
        tft.fillCircle(15, 120, 10, ILI9341_BLACK);
        tft.drawCircle(15, 120, 10, ILI9341_WHITE);
        tft.fillCircle(15, 160, 10, ILI9341_WHITE);
        tft.fillCircle(15, 200, 10, ILI9341_BLACK);
        tft.drawCircle(15, 200, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 4:
        horWhite = 0;
        minWhite = 10;
        setInc = 0;
        tft.fillCircle(15, 160, 10, ILI9341_BLACK);
        tft.drawCircle(15, 160, 10, ILI9341_WHITE);
        tft.fillCircle(15, 200, 10, ILI9341_WHITE);
        tft.fillCircle(150, 80, 10, ILI9341_BLACK);
        tft.drawCircle(150, 80, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 5:
        horWhite = 0;
        minWhite = 20;
        setInc = 0;
        tft.fillCircle(15, 200, 10, ILI9341_BLACK);
        tft.drawCircle(15, 200, 10, ILI9341_WHITE);
        tft.fillCircle(150, 80, 10, ILI9341_WHITE);
        tft.fillCircle(150, 120, 10, ILI9341_BLACK);
        tft.drawCircle(150, 120, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 6:
        horWhite = 0;
        minWhite = 25;
        setInc = 10;
        tft.fillCircle(150, 80, 10, ILI9341_BLACK);
        tft.drawCircle(150, 80, 10, ILI9341_WHITE);
        tft.fillCircle(150, 120, 10, ILI9341_WHITE);
        tft.fillCircle(150, 160, 10, ILI9341_BLACK);
        tft.drawCircle(150, 160, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 7:
        horWhite = 1;
        minWhite = 0;
        setInc = 30;
        tft.fillCircle(150, 120, 10, ILI9341_BLACK);
        tft.drawCircle(150, 120, 10, ILI9341_WHITE);
        tft.fillCircle(150, 160, 10, ILI9341_WHITE);
        tft.fillCircle(150, 200, 10, ILI9341_BLACK);
        tft.drawCircle(150, 200, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
      case 8:
        horWhite = 1;
        minWhite = 30;
        setInc = 30;
        tft.fillCircle(150, 160, 10, ILI9341_BLACK);
        tft.drawCircle(150, 160, 10, ILI9341_WHITE);
        tft.fillCircle(150, 200, 10, ILI9341_WHITE);
        tft.fillCircle(15, 80, 10, ILI9341_BLACK);
        tft.drawCircle(15, 80, 10, ILI9341_WHITE);
        menuCount = newMenuCount;
        break;
    }
  }
}

void printMenu() {
  tft.setFont(&wireOne16pt7b);
  tft.setTextSize(1);
  tft.setCursor(50, 30);
  tft.print("Choose your time settings.");
  tft.setCursor(85, 60);
  tft.print("Press        to play.");
  tft.drawBitmap(130, 35, ppButton, 30, 30, ILI9341_WHITE);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(2);
  tft.setCursor(30, 90);
  tft.print("3+2");
  tft.setCursor(30, 130);
  tft.print("5+0");
  tft.setCursor(30, 170);
  tft.print("5+3");
  tft.setCursor(30, 210);
  tft.print("10+0");
  tft.setCursor(165, 90);
  tft.print("20+0");
  tft.setCursor(165, 130);
  tft.print("25+10");
  tft.setCursor(165, 170);
  tft.print("60+30");
  tft.setCursor(165, 210);
  tft.print("90+30");
  tft.fillCircle(15, 80, 10, ILI9341_BLACK);
  tft.drawCircle(15, 80, 10, ILI9341_WHITE);
  tft.fillCircle(15, 120, 10, ILI9341_BLACK);
  tft.drawCircle(15, 120, 10, ILI9341_WHITE);
  tft.fillCircle(15, 160, 10, ILI9341_BLACK);
  tft.drawCircle(15, 160, 10, ILI9341_WHITE);
  tft.fillCircle(15, 200, 10, ILI9341_BLACK);
  tft.drawCircle(15, 200, 10, ILI9341_WHITE);
  tft.fillCircle(150, 80, 10, ILI9341_BLACK);
  tft.drawCircle(150, 80, 10, ILI9341_WHITE);
  tft.fillCircle(150, 120, 10, ILI9341_BLACK);
  tft.drawCircle(150, 120, 10, ILI9341_WHITE);
  tft.fillCircle(150, 160, 10, ILI9341_BLACK);
  tft.drawCircle(150, 160, 10, ILI9341_WHITE);
  tft.fillCircle(150, 200, 10, ILI9341_BLACK);
  tft.drawCircle(150, 200, 10, ILI9341_WHITE);
}

void printMenuLabels() {
  if (newModeWhite == 2) {
    tft.setTextSize(5);
    tft.fillRect(115, 120, 20, 55, ILI9341_BLACK);
    tft.setCursor(100, 150);//ponteiro
    tft.println(".");
    tft.setTextSize(1);
    tft.fillRect(270, 150, 40, 30, ILI9341_BLACK);
    tft.setCursor(275, 135); //min
    tft.println("min");
    tft.setCursor(275, 145); //sec
    tft.println("sec");
  }
  else {
    tft.setTextSize(5);
    tft.fillRect(115, 120, 20, 55, ILI9341_BLACK);
    tft.setCursor(100, 150);//ponteiro
    tft.println(":");
    tft.setTextSize(1);
    tft.fillRect(270, 150, 40, 30, ILI9341_BLACK);
    tft.setCursor(275, 135); //hrs
    tft.println("hrs");
    tft.setCursor(275, 145); //hrs
    tft.println("min");
  }
}

//CLOCK LABELS (HOR/MIN AND PONTEIRO)
void printWhiteLabels() {
  tft.setFont(&FreeMonoBold9pt7b);
  if (newModeWhite == 2) {
    tft.fillRect(105, 40, 15, 20, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.fillRect(260, 50, 40, 30, ILI9341_BLACK);
    tft.setCursor(265, 65); //min
    tft.println("min");
    tft.setCursor(265, 75); //sec
    tft.println("sec");
  }
  else {
    tft.setTextSize(5);
    tft.setCursor(85, 50);//ponteiro
    tft.println(".");
    tft.setTextSize(1);
    tft.fillRect(260, 50, 40, 30, ILI9341_BLACK);
    tft.setCursor(265, 65); //hrs
    tft.println("hrs");
    tft.setCursor(265, 75); //hrs
    tft.println("min");
  }
}

void printBlackLabels() {
  tft.setFont(&FreeMonoBold9pt7b);
  if (newModeBlack == 2) {
    tft.setTextSize(5);
    tft.fillRect(105, 160, 15, 20, ILI9341_BLACK);
    tft.setTextSize(1);
    tft.fillRect(260, 170, 40, 30, ILI9341_BLACK);
    tft.setCursor(265, 185); //hrs
    tft.println("min");
    tft.setCursor(265, 195); //hrs
    tft.println("sec");
  }
  else {
    tft.setTextSize(5);
    tft.setCursor(85, 165);//ponteiro
    tft.println(".");
    tft.setTextSize(1);
    tft.fillRect(260, 170, 40, 30, ILI9341_BLACK);
    tft.setCursor(265, 185); //hrs
    tft.println("hrs");
    tft.setCursor(265, 195); //hrs
    tft.println("min");
  }
}

//PAUSE GAME FUNCTIONS
void pauseGameWhite() {
  delay(1000);
  tft.drawBitmap(270, 105, ppButton, 30, 30, ILI9341_WHITE);
  //with game paused in timer white, for exit is necessary push button black
  while (digitalRead(pauseButton) != HIGH) {
  }
  blackButtonState = HIGH;
  tft.fillRect(285, 105, 20, 30, ILI9341_BLACK);
}

void pauseGameBlack() {
  delay(1000);
  tft.drawBitmap(270, 105, ppButton, 30, 30, ILI9341_WHITE);
  //with game paused in timer white, for exit is necessary push button black
  while (digitalRead(pauseButton) != HIGH) {
  }
  whiteButtonState = HIGH;
  tft.fillRect(285, 105, 20, 30, ILI9341_BLACK);
}

//running timer white
void timerWhite() {
  secWhite = setSecWhite - cSecWhite;
  totalSecWhite = secWhite + minWhite * 60 + horWhite * 3600;

  if (totalSecWhite < 3600) {
    newModeWhite = 2; // MIN SEG
  }
  else {
    newModeWhite = 1; // HRS MIN
  }

  if (newModeWhite != modeWhite) {
    printWhiteLabels();
    modeWhite = newModeWhite;
  }
  else {
    modeWhite = newModeWhite;
  }

  //if seconds timer below 0(-1) and minutes >=0 plus counter seconds with 59 to
  //setSecWhite update the value and remove 1 in minutes timer
  if ((secWhite == -1) && (minWhite >= 0)) {
    setSecWhite = cSecWhite + 59; //update setSecWhite
    secWhite = 59;
    minWhite--;
  }
  //if seconds and minutes timer below 0(-1) and hour is > 0 plus 59 to
  //setSecWhite update the value, put minutes to 59 and remove 1 to hour
  if ((secWhite == -1) && (minWhite == -1) && (horWhite > 0)) {
    setSecWhite = cSecWhite + 59;
    minWhite = 59;
    horWhite--;
  }

  printTimerWhite(); //update values
  //if seconds, minutes and hour timers go to 0 loop forever and print end game
  if ((secWhite == 0) && (minWhite == 0) && (horWhite == 0)) {
    while (1) {
      tft.setFont(&FreeMonoBold9pt7b);
      tft.setTextSize(1);
      tft.setCursor(240, 45); //brancas
      tft.println(" lost!");
      tft.setCursor(240, 165); //pretas
      tft.println(" won!");
    }
  }
}

//running timer black
void timerBlack() {
  secBlack = setSecBlack - cSecBlack;
  totalSecBlack = secBlack + minBlack * 60 + horBlack * 3600;

  if (totalSecBlack < 3600) {
    newModeBlack = 2; // MIN SEG
  }
  else {
    newModeBlack = 1; // HRS MIN
  }

  if (newModeBlack != modeBlack) {
    printBlackLabels();
    modeBlack = newModeBlack;
  }
  else {
    modeBlack = newModeBlack;
  }

  //timer
  if ((secBlack == -1) && (minBlack >= 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack--;
    secBlack = 59;
  }
  if ((secBlack == -1) && (minBlack == -1) && (horBlack > 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack = 59;
    horBlack--;
  }

  printTimerBlack();

  if ((secBlack == 0) && (minBlack == 0) && (horBlack == 0)) {
    while (1) {
      tft.setFont(&FreeMonoBold9pt7b);
      tft.setTextSize(1);
      tft.setCursor(240, 45); //brancas
      tft.println(" won!");
      tft.setCursor(240, 165); //pretas
      tft.println(" lost!");
    }
  }
}

//increment to white
void incWhite() {
  if (setInc > 0) {
    secWhite = secWhite + setInc; //increment seconds to seconds timer
    cSecWhite = setSecWhite - secWhite; //start new count

    //if seconds timer + increment > 59 and < 120 add 1 to minutes and remove 60 seconds to seconds timer
    if (secWhite > 59 && secWhite < 120) {
      secWhite = secWhite - 60;
      cSecWhite = setSecWhite - secWhite;
      minWhite++;
    }
    //if seconds timer + increment > 119 add 2 to minutes and remove 120 seconds to seconds timer
    else if (secWhite > 119) {
      secWhite = secWhite - 120;
      cSecWhite = setSecWhite - secWhite;
      minWhite = minWhite + 2;
    }
    //if minutes timer > 59 add 1 to hour and remove 60 to minutes timer
    if (minWhite > 59) {
      minWhite = minWhite - 60;
      horWhite++;
    }

    totalSecWhite = secWhite + minWhite * 60 + horWhite * 3600;

    if (totalSecWhite > 3600) {
      newModeWhite = 1; // MIN SEG
    }

    if (newModeWhite != modeWhite) {
      printWhiteLabels();
      modeWhite = newModeWhite;
      tft.setTextSize(5);
      tft.fillRect(0, 5, 100, 110, ILI9341_BLACK);
      tft.setCursor(0, 110); //horas
      tft.println("0");
      tft.setCursor(60, 110); //horas
      tft.println(horWhite);
      oldHorWhite = horWhite;
    }
    else {
      modeWhite = newModeWhite;
    }
    printTimerWhite(); //print new values
  }
}

//increment to black
void incBlack() {
  if (setInc > 0) {
    secBlack = secBlack + setInc;
    cSecBlack = setSecBlack - secBlack;
    if (secBlack > 59 && secBlack < 120) {
      secBlack = secBlack - 60;
      cSecBlack = setSecBlack - secBlack;
      minBlack++;
    }
    else if (secBlack > 119) {
      secBlack = secBlack - 120;
      cSecBlack = setSecBlack - secBlack;
      minBlack = minBlack + 2;
    }
    if (minBlack > 59) {
      minBlack = minBlack - 60;
      horBlack++;
    }
    totalSecBlack = secBlack + minBlack * 60 + horBlack * 3600;

    if (totalSecBlack < 3600) {
      newModeBlack = 2; // MIN SEG
    }
    else {
      newModeBlack = 1; // HRS MIN
    }

    if (newModeBlack != modeBlack) {
      printBlackLabels();
      modeBlack = newModeBlack;
      tft.setTextSize(4);
      tft.setFont(&wireOne20pt7b);
      tft.fillRect(0, 160, 100, 110, ILI9341_BLACK);
      tft.setCursor(0, 230); //horas
      tft.println("0");
      tft.setCursor(60, 230); //horas
      tft.println(horBlack);
      oldHorBlack = horBlack;
    }
    else {
      modeBlack = newModeBlack;
    }
    printTimerBlack();
  }
}

//Arduino setup
void setup() {

  Serial.begin(115200);
  Serial.println("Inicializando Relógio");

  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);
  tft.setTextColor(ILI9341_WHITE);

  //setup buttons
  pinMode(blackButton, INPUT);
  pinMode(whiteButton, INPUT); //button white
  pinMode(pauseButton, INPUT);
 

  //FIRST SCREEN
  tft.setFont(&wireOne20pt7b);
  tft.setTextSize(2);
  tft.setCursor(30, 100); //brancas
  tft.println("Frois'");
  tft.setTextSize(1);
  tft.setCursor(30, 150);
  tft.println("Chess Clock");
  tft.setCursor(0, 170); //brancas
  delay(2000);


  //THE MENU
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRoundRect(1, 1, 319, 239, 10, ILI9341_WHITE);
  printMenu();
  while (exitMenu == false) {
    menuSetup();
    switchMenu();
  }
  
  //BUILD THE TIMERS ETC
  totalSecWhite = secWhite + minWhite * 60 + horWhite * 3600;
  if (totalSecWhite < 3600) {
    newModeWhite = 2; // MIN SEG
  }
  else {
    newModeWhite = 1; // HRS MIN
  }

  //setting up values for white and black to start timers
  horBlack = horWhite;
  minBlack = minWhite;
  secWhite = setInc;
  secBlack = secWhite;
  setSecBlack = secWhite;
  setSecWhite = secWhite;
  newModeBlack = newModeWhite;
  oldMinWhite = oldMinBlack = oldHorWhite = oldHorBlack = -1;

  tft.fillScreen(ILI9341_BLACK);
  printTimerWhite(); //print timers and labels
  printTimerBlack();
  printWhiteLabels();
  printBlackLabels();

  tft.setTextSize(1);
  tft.setCursor(240, 30); 
  tft.println("White");
  tft.setCursor(240, 150);
  tft.println("Black");   //white/black labels
  tft.drawBitmap(270, 105, ppButton, 30, 30, ILI9341_WHITE);
  tft.setTextSize(5);
  tft.setCursor(85, 110);//ponteiro
  tft.println(".");
  tft.setCursor(85, 230);//ponteiro
  tft.println(".");
  tft.drawRoundRect(230, 1, 80, 239, 10, ILI9341_WHITE);
  tft.drawRoundRect(1, 1, 225, 239, 10, ILI9341_WHITE);   //
  tft.drawCircle(245, 60, 10, ILI9341_WHITE);
  tft.drawCircle(245, 180, 10, ILI9341_WHITE);
  
  caractere = Serial.read(); // Armazena caractere lido
  blackButtonState = digitalRead(blackButton);
  while (blackButtonState == HIGH) {
    caractere = Serial.read();
    blackButtonState = digitalRead(blackButton);
  }
  tft.fillRect(285, 110, 20, 20, ILI9341_BLACK);   //wait black push the button
  Serial.println("Entrou no loop");
}

//Arduino loop
void loop() {
  //push black button run white timer
  whiteButtonState = digitalRead(whiteButton);
  blackButtonState = digitalRead(blackButton);
  if (digitalRead(blackButton) == LOW) {
    cTemp = SEC - cSecWhite; //difference between SEC and previous seconds count
    tft.fillCircle(245, 60, 10, ILI9341_WHITE);
    tft.fillCircle(245, 180, 10, ILI9341_BLACK);
    tft.drawCircle(245, 180, 10, ILI9341_WHITE);
    while (digitalRead(whiteButton) == HIGH && pauseButtonState == LOW)  { //if white button or any keypad pushed exit loop
      cSecWhite = SEC - cTemp; //second count for white timer
      timerWhite(); //run timer white
      pauseButtonState = digitalRead(pauseButton);
    }
    if(pauseButtonState == HIGH){
      pauseGameWhite();
    }
    else{
      incWhite();  //increment seconds
    }
   blackButtonState = digitalRead(blackButton);  
  }
  
  //push white button run black timer
  if (digitalRead(whiteButton)) {
    cTemp = SEC - cSecBlack;
    tft.fillCircle(245, 180, 10, ILI9341_WHITE);
    tft.fillCircle(245, 60, 10, ILI9341_BLACK);
    tft.drawCircle(245, 60, 10, ILI9341_WHITE);
    while (digitalRead(blackButton) == HIGH && pauseButtonState == LOW) {
      cSecBlack = SEC - cTemp;
      timerBlack();
      pauseButtonState = digitalRead(pauseButton);
    }
    if(pauseButtonState == HIGH){
      pauseGameBlack();
    }
    else{
      incBlack();
    }
  }
}
