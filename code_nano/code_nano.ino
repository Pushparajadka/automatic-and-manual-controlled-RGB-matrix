#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#define RxD 8
#define TxD 9
SoftwareSerial SerialH(RxD, TxD);


int j; int k;
int readValue_red, readValue_green, readValue_blue;
int potpin_red = A0, potpin_green = A1 , potpin_blue = A2;
int menuitem = 1;
int frame = 1;
int page = 1;
int lastMenuItem = 1;
int auto_switch = 2;
int manual_switch = 12;
bool flag3 = true;

String menuItem1 = "Contrast";
String menuItem2 = "RED"; 
String menuItem3 = "GREEN";
String menuItem4 = "BLUE";
String menuItem5 = "hue";
String menuItem6 = "Reset";

boolean backlight = true;
int contrast = 37;
int red, green, blue;


boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); //Download the latest Adafruit Library in order to use this constructor
void setup() {
  //pinMode(7, OUTPUT);
  //turnBacklightOn();

  encoder = new ClickEncoder(A4, A3, A5);   // A4 , A3 ,A5
  encoder->setAccelerationEnabled(false);

  display.begin();
  display.clearDisplay();
  setContrast();

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  last = encoder->getValue();
  Serial.begin(9600);
  SerialH.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);

}
void loop() {
  if (digitalRead(auto_switch) == HIGH)
  {    
     if (digitalRead(manual_switch) == 1)
    manualmode();  
    else initialisation();
  }
  else
  {
    automode();
  }
}
void  automode()
{
  if (flag3)   
  {
    display.setContrast(50);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("BLUETOOTH");
    display.setCursor(30, 15);
    display.print("MODE");
    display.display();
    flag3 = false;
    //Serial.println("ble");
  }
  if (Serial.available() > 0)
  {
    //Serial.println("hiier");
    int redInt = Serial.parseInt();  //converts the first sent vALUE TO integer which is seperated by a special character
    int greenInt = Serial.parseInt();//converts the second sent vALUE TO integer which is seperated by a special character
    int blueInt = Serial.parseInt();  //converts the third sent vALUE TO integer which is seperated by a special character


    byte i;
    Serial.print(redInt);
    Serial.print('|');
    Serial.print(greenInt);
    Serial.print('|');
    Serial.println(blueInt);


    SerialH.print(redInt);
    SerialH.print('|');
    SerialH.print(greenInt);
    SerialH.print('|');
    SerialH.println(blueInt);
  }
}
void initialisation()
{

  readValue_red = analogRead(potpin_red);
  readValue_green = analogRead(potpin_green);
  readValue_blue = analogRead(potpin_blue);
  int red=map(readValue_red, 0,1023,0,500);
  int green=map(readValue_green, 0,1023,0,500);
  int blue=map(readValue_blue, 0,1023,0,500);
   display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(15,0);
  display.println("POT MODE");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,15);
  display.println("RED");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
   display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(23,15);
  display.println("GREEN");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
   display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(55,15);
  display.println("BLUE");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  // DATA DISPLAY//
delay(100);
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.println(red);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
   display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(25,25);
  display.println(green);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
   display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(55,25);
  display.println(blue);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.display();
  SerialH.print(red);
  SerialH.print('|');
  SerialH.print(green);
  SerialH.print('|');
  SerialH.println(blue);
  
  Serial.print(red);
  Serial.print('|');
  Serial.print(green);
  Serial.print('|');
  Serial.println(blue);
  delay(100);
}
void manualmode()
{
  flag3 = true;
  drawMenu();

  readRotaryEncoder();

  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Clicked:
        middle = true;
        break;
    }
  }

  if (up && page == 1 ) {

    up = false;
    if (menuitem == 2 && frame == 2)
    {
      frame--;
    }

    if (menuitem == 4 && frame == 4)
    {
      frame--;
    }
    if (menuitem == 3 && frame == 3)
    {
      frame--;
    }
    lastMenuItem = menuitem;
    menuitem--;
    if (menuitem == 0)
    {
      menuitem = 1;
    }
  } else if (up && page == 2 && menuitem == 1 ) {
    up = false;
    contrast--;
    setContrast();
  }
  else if (up && page == 2 && menuitem == 2 ) {
    up = false;
    red = red - 10;
    if (red <= 0)
    {
      red = 0;
    }
  }
  else if (up && page == 2 && menuitem == 3 ) {
    up = false;
    green = green - 10;
    if (green <= 0)
    {
      green = 0;
    }
  }
  else if (up && page == 2 && menuitem == 4 ) {
    up = false;
    blue = blue - 10;
    if (blue <= 0)
    {
      blue = 0;
    }
  }

  if (down && page == 1) //We have turned the Rotary Encoder Clockwise
  {

    down = false;
    if (menuitem == 3 && lastMenuItem == 2)
    {
      frame ++;
    } else  if (menuitem == 4 && lastMenuItem == 3)
    {
      frame ++;
    }
    else  if (menuitem == 5 && lastMenuItem == 4 && frame != 4)
    {
      frame ++;
    }
    lastMenuItem = menuitem;
    menuitem++;
    if (menuitem == 7)
    {
      menuitem--;
    }

  } else if (down && page == 2 && menuitem == 1) {
    down = false;
    contrast++;
    setContrast();
  }
  else if (down && page == 2 && menuitem == 2) {
    down = false;
    red = red + 10;
    if (red > 255)
    {
      red = 255;
    }
  }
  else if (down && page == 2 && menuitem == 3 ) {
    down = false;
    green = green + 10;
    if (green > 255)
    {
      green = 255;
    }
  }
  else if (down && page == 2 && menuitem == 4 ) {
    down = false;
    blue = blue + 10;
    if (blue > 255)
    {
      blue = 255;
    }
  }

  if (middle) //Middle Button is Pressed
  {
    middle = false;

    if (page == 1 && menuitem == 5) // Backlight Control
    { //Equation to be added
      //      if (backlight)
      //      {
      //        backlight = false;
      //        menuItem5 = "Light: OFF";
      //        turnBacklightOff();
      //      }
      //      else
      //      {
      //        backlight = true;
      //        menuItem5 = "Light: ON";
      //        turnBacklightOn();
      //      }
    }

    if (page == 1 && menuitem == 6) // Reset
    {
      resetDefaults();
    }


    else if (page == 1 && menuitem <= 4) {
      page = 2;
    }
    else if (page == 2)
    {
      page = 1;
    }
  }
  Serial.print(red);
  Serial.print('|');
  Serial.print(green);
  Serial.print('|');
  Serial.println(blue);

  SerialH.print(red);
  SerialH.print('|');
  SerialH.print(green);
  SerialH.print('|');
  SerialH.println(blue);

}

void drawMenu()
{

  if (page == 1)
  {
    display.setContrast(50);
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("MANUAL MODE");
    display.drawFastHLine(0, 10, 83, BLACK);

    if (menuitem == 1 && frame == 1)
    {
      displayMenuItem(menuItem1, 15, true);
      displayMenuItem(menuItem2, 25, false);
      displayMenuItem(menuItem3, 35, false);
    }
    else if (menuitem == 2 && frame == 1)
    {
      displayMenuItem(menuItem1, 15, false);
      displayMenuItem(menuItem2, 25, true);
      displayMenuItem(menuItem3, 35, false);
    }
    else if (menuitem == 3 && frame == 1)
    {
      displayMenuItem(menuItem1, 15, false);
      displayMenuItem(menuItem2, 25, false);
      displayMenuItem(menuItem3, 35, true);
    }
    else if (menuitem == 4 && frame == 2)
    {
      displayMenuItem(menuItem2, 15, false);
      displayMenuItem(menuItem3, 25, false);
      displayMenuItem(menuItem4, 35, true);
    }

    else if (menuitem == 3 && frame == 2)
    {
      displayMenuItem(menuItem2, 15, false);
      displayMenuItem(menuItem3, 25, true);
      displayMenuItem(menuItem4, 35, false);
    }
    else if (menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 15, true);
      displayMenuItem(menuItem3, 25, false);
      displayMenuItem(menuItem4, 35, false);
    }

    else if (menuitem == 5 && frame == 3)
    {
      displayMenuItem(menuItem3, 15, false);
      displayMenuItem(menuItem4, 25, false);
      displayMenuItem(menuItem5, 35, true);
    }

    else if (menuitem == 6 && frame == 4)
    {
      displayMenuItem(menuItem4, 15, false);
      displayMenuItem(menuItem5, 25, false);
      displayMenuItem(menuItem6, 35, true);
    }

    else if (menuitem == 5 && frame == 4)
    {
      displayMenuItem(menuItem4, 15, false);
      displayMenuItem(menuItem5, 25, true);
      displayMenuItem(menuItem6, 35, false);
    }
    else if (menuitem == 4 && frame == 4)
    {
      displayMenuItem(menuItem4, 15, true);
      displayMenuItem(menuItem5, 25, false);
      displayMenuItem(menuItem6, 35, false);
    }
    else if (menuitem == 3 && frame == 3)
    {
      displayMenuItem(menuItem3, 15, true);
      displayMenuItem(menuItem4, 25, false);
      displayMenuItem(menuItem5, 35, false);
    }
    else if (menuitem == 2 && frame == 2)
    {
      displayMenuItem(menuItem2, 15, true);
      displayMenuItem(menuItem3, 25, false);
      displayMenuItem(menuItem4, 35, false);
    }
    else if (menuitem == 4 && frame == 3)
    {
      displayMenuItem(menuItem3, 15, false);
      displayMenuItem(menuItem4, 25, true);
      displayMenuItem(menuItem5, 35, false);
    }
    display.display();
  }
  else if (page == 2 && menuitem == 1)
  {
    displayIntMenuPage(menuItem1, contrast);
  }

  else if (page == 2 && menuitem == 2)
  {
    displayIntMenuPage(menuItem2, red);
  }
  else if (page == 2 && menuitem == 3)
  {
    displayIntMenuPage(menuItem3, green);
  }
  else if (page == 2 && menuitem == 4)
  {
    displayIntMenuPage(menuItem4, blue);
  }
  else if (page == 2 && menuitem == 4)
  {
    displayIntMenuPage(menuItem4, blue);
  }

}

void resetDefaults() //create white color
{
  contrast = 50;
  red = 255;
  blue = 255;
  green = 255;
  setContrast();
  backlight = true;
  menuItem5 = "Light: ON";
  //turnBacklightOn();
}

void setContrast()
{
  display.setContrast(contrast);
  display.display();
}

//void turnBacklightOn()
//{
//  digitalWrite(8, HIGH);
//}
//
//void turnBacklightOff()
//{
//  digitalWrite(8, LOW);
//}

void timerIsr() {
  encoder->service();
}

void displayIntMenuPage(String menuItem, int value)
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 15);
  display.print("Value");
  display.setTextSize(2);
  display.setCursor(5, 25);
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayStringMenuPage(String menuItem, String value)
{
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15, 0);
  display.print(menuItem);
  display.drawFastHLine(0, 10, 83, BLACK);
  display.setCursor(5, 15);
  display.print("Value");
  display.setTextSize(2);
  display.setCursor(5, 25);
  display.print(value);
  display.setTextSize(2);
  display.display();
}

void displayMenuItem(String item, int position, boolean selected)
{
  if (selected)
  {
    display.setTextColor(WHITE, BLACK);
  } else
  {
    display.setTextColor(BLACK, WHITE);
  }
  display.setCursor(0, position);
  display.print(">" + item);
}

void readRotaryEncoder()
{
  value += encoder->getValue();

  if (value / 2 > last) {
    last = value / 2;
    down = true;
    delay(150);
  } else   if (value / 2 < last) {
    last = value / 2;
    up = true;
    delay(150);
  }
}
