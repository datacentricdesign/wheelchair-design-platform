// Based on Adafruit spitftbitmap example
#include <Adafruit_GFX.h>    // Core Adafruit graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for out ST7735 tft display
#include <SPI.h>             // SPI library for arduino
#include <SD.h>              // SD library for arduino          

// The TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  
#define TFT_CS  2           // Chip select line for TFT display
#define TFT_RST  3          // Reset line for TFT 
//#define TFT_RST  -1       // You can also connect this pin to the Arduino reset!
#define TFT_DC   4          // Data/command line for TFT
#define SD_CS   5           // Chip select line for SD card

// We are using HARDWARE SPI pins, which are unique to each board and not reassignable. 
// This is the fastest mode of operation (there are slower, software SPI pins)
// Hardware pins are required if using the breakout board's microSD card.

// tft screen variable, using our pins for its control.
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);

  while (!Serial) {
    delay(10);                // wait for serial console
  }

  // 1.8" TFT screen Initializer:
  tft.initR(INITR_BLACKTAB);  // Init ST7735R chip with a black tab from ST7735 library
  Serial.println("Lets begin our SD Card Example"); 
  
  tft.fillScreen(ST77XX_BLUE); // Blue screen... don't panic

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {    // initializing our SD
    Serial.println("It failed!");
    return;
  }
  Serial.println("Successful!");

  File root = SD.open("/");  // Opening our root directory, where all the bitmaps are.
  printDirectory(root, 0);   // Printing directory of File Object
  root.close();

}

void loop() {
// uncomment these lines to draw bitmaps in different locations/rotations!
/*
  tft.fillScreen(ST7735_BLACK); // Clear display
  for(uint8_t i=0; i<4; i++)    // Draw 4 parrots
    bmpDraw("1.bmp", tft.width() / 4 * i, tft.height() / 4 * i);
  delay(1000);
  tft.setRotation(tft.getRotation() + 1); // Inc rotation 90 degrees
*/
    Serial.println("Starting Image drawing sequence");

    // Put Name of image here 
    bmpDraw("1.bmp", 0, 0);
    // wait 2.5 seconds
    delay(2500);
    bmpDraw("2.bmp", 0, 0);
    // wait 2.5 seconds
    delay(2500);
    bmpDraw("3.bmp", 0, 0);
    // wait 2.5 seconds
    delay(2500);
    
    // 'Blinking' screen to note the end of the sequence
    for(int i = 0 ; i < 4 ; i++) {
    tft.invertDisplay(true); // inverting the colours of the  display every half second.
    delay(500);
    tft.invertDisplay(false);
    delay(500);
  }

    Serial.println("Ending Image drawing sequence");
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

// Function to draw Bitmpas
void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // Width + Hheight  in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite();
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
            
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      }  // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian (data storage convention), Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}



// Printing the directory in the parameters
void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
