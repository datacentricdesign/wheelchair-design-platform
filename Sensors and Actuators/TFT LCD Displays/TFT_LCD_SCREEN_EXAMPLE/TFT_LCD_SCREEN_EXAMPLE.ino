// Based on graphicstest example code of Adafruit ST7735 library
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>             // Library for use of the SPi protocol

// Pins from the breakout board to the arduino
#define TFT_CS         2 
#define TFT_RST        3 
#define TFT_DC         4


// By using HARDWARE SPI pins, which are unique to each board and not reassignable. 
// This is the fastest mode of operation (there are slower, software SPI pins) and 
// it is required if using the breakout board's microSD card.

// tft screen variable, using our pins for its control.
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Hello World, here is the ST77xx TFT Test")); // the F() macro keeps the string in flash memory, 
                                             // freeing up space in RAM!

  // 1.8" TFT screen Initializer:
  tft.initR(INITR_BLACKTAB);                // Init ST7735R chip with a black tab from ST7735 library

  Serial.println(F("Screen Initialized")); 

  uint16_t time = millis();                 // creating times for filling the screen action
  tft.fillScreen(ST77XX_BLACK);             // filling screen with premade colour BLACK
  time = millis() - time; 
  // Premade colors: ST77XX_BLACK, ST77XX_WHITE, ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE, 
  // ST77XX_CYAN, ST77XX_MAGENTA, ST77XX_YELLOW , ST77XX_ORANGE 
  Serial.print("Time to fill the screen: ");
  Serial.print(time, DEC);                // Printing time to fill the screen 
  Serial.println(" ms.");    
  delay(1500);
}

void graphics_test()
{
        // Output large block of text (the famous lorem ipsum) in white
  tft.fillScreen(ST77XX_BLACK);             // Clear Screen

  Serial.println("Print Lorem Ipsum");
  testDrawText("Lorem ipsum dolor sit amet, consectetur adipiscing elit Curabitur adipiscing ante sed \     
               nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit \
               amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam \
               aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor \
               imperdiet posuere. ", ST77XX_WHITE);

               // As you can see, we broke this large instruction into several ones using the '\' character
  delay(5000); // dont forget, this is in milliseconds

  // tft print function!
  Serial.println("Print test");
  tftPrintTest();
  delay(6000);

  // draw a single pixel in the center of the screen
  Serial.println("Pixel test");
  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);
  delay(1500);

  // line drawing test
  Serial.println("Line drawing test");
  testLines(ST77XX_YELLOW);
  delay(1500);

  // optimized lines
  Serial.println("Optimized Lines Test");
  testFastLines(ST77XX_RED, ST77XX_BLUE);
  delay(1500);

  Serial.println("Rectangle test");
  testDrawRects(ST77XX_GREEN);
  delay(1500);

  Serial.println("Filled rectangles test");
  testFillRects(ST77XX_YELLOW, ST77XX_MAGENTA);
  delay(1500);

  tft.fillScreen(ST77XX_BLACK);
  Serial.println("Filled circles test");
  testFillCircles(10, ST77XX_BLUE);
  Serial.println("Circles test");
  testDrawCircles(10, ST77XX_WHITE);
  delay(1500);

  Serial.println("Round rectangles test");
  testRoundRects();
  delay(1500);

  Serial.println("Triangles test");
  testTriangles();
  delay(1500);

  Serial.println("Drawing media buttons");
  mediaButtons();
  delay(3000);

  Serial.println("done");
  delay(2000);
}

void loop() {
  // After we're done we will just invert colours  4 times until we reset again.
  for(int i = 0 ; i < 4 ; i++) {
    tft.invertDisplay(true); // inverting the colours of the  display every half second.
    delay(500);
    tft.invertDisplay(false);
    delay(500);
  }

  
  Serial.println("Starting graphical tests");
  graphics_test();
  Serial.println("Ending graphical tests");
}

void testLines(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK); // Clear screen with black
  for (int16_t x=0; x < tft.width(); x+=6) {
    // drawLine from (x, y) point to  (a,b) point with color
    tft.drawLine(0, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
    delay(0);
  }

  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
    delay(0);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
    delay(0);
  }
}

// Text drawing function, starting with upper left corner.
void testDrawText(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testFastLines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}


// Drawing Rectangles 
void testDrawRects(uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

// Drawing filled rectangles
void testFillRects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST77XX_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

//Drawing filled Circles
void testFillCircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

// Drawing circles
void testDrawCircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}


// Drawing triangles, requires 3 points
void testTriangles() {
  tft.fillScreen(ST77XX_BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width()/2;  
  int x = tft.height()-1; 
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

//Drawing round rectangles, requires position of left upper corner (x,y) , width, height, radius and color
void testRoundRects() {
  tft.fillScreen(ST77XX_BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

// Printing text in several ways and current timer of the running of the program
void tftPrintTest() {
  tft.setTextWrap(false); // whether text is wrapped.
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
}

// Render Buttons on the Screen! 
void mediaButtons() {
  // play
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}
