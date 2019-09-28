#include <RGBmatrixPanel.h>

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup() {
  Serial.begin(9600);
   matrix.begin();
}

void loop() {
  if (Serial.available() > 0)
  {
    int redInt = Serial.parseInt();  //converts the first sent vALUE TO integer which is seperated by a special character
    int greenInt = Serial.parseInt();//converts the second sent vALUE TO integer which is seperated by a special character
    int blueInt = Serial.parseInt();  //converts the third sent vALUE TO integer which is seperated by a special character
    redInt = constrain(redInt, 0, 255);
    greenInt = constrain(greenInt, 0, 255);
    blueInt = constrain(blueInt, 0, 255);
    Serial.print(redInt);
    Serial.print('\t');
    Serial.print(greenInt );
    Serial.print('\t');
    Serial.println(blueInt);

    int mapping1 = map(redInt, 0, 255, 0, 15);   // mapping the value to 15 and will have 17 steps
    int mapping2 = map(greenInt, 0, 255, 0, 15);
    int mapping3 = map(blueInt, 0, 255, 0, 15);
    matrix.fillScreen(matrix.Color444(mapping1, mapping2, mapping3));
  }
}
