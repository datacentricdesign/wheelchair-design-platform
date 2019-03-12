// Based on echo example from Adafruit
//
// This code  echos whatever is coming from the GPS unit to the
// serial monitor, handy for debugging!
//
#include <Adafruit_GPS.h>   // Adafruit GPS Library

// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground

//   Using hardware serial (e.g. Arduino Mega):
//   Connect the GPS TX (transmit) pin to Arduino RX1, RX2 or RX3
//   Connect the GPS RX (receive) pin to matching TX1, TX2 or TX3

// Using hardware serial:
Adafruit_GPS GPS(&Serial1); // Creating our GPS object.


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
#define GPSECHO  true

// Keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;

void setup()  
{    
  // Begin serial with  115200 baud rate so we can read the GPS fast enough. 
  Serial.begin(115200);
  Serial.println("Adafruit GPS library echo test!");

  // 9600 NMEA is the default baud rate for MTK - some use 4800
  GPS.begin(9600); //Beginning our GPS transmission
  
  
  // Uncomment this line to turn on all the available data - for 9600 baud you'll want 1 Hz rate
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  
  // Set the update rate
  // Note you must send both commands below to change both the output rate (how often the position
  // is written to the serial line), and the position fix rate.
  
  // 1 Hz update rate
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  //GPS.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  
  // 5 Hz update rate- for 9600 baud you'll have to set the output to RMC or RMCGGA only (see above)
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  
  // 10 Hz update rate - for 9600 baud you'll have to set the output to RMC only (see above)
  // Note the position can only be updated at most 5 times a second so it will lag behind serial output.
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  //GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // This code can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS. That makes the
  // loop code  a lot easier!
  useInterrupt(true);
  
  delay(1000);
}

// Interrupt is called once a millisecond, it then looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // Interest point for debugging
  if (GPSECHO)
    if (c) UDR0 = c;  
    // Writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}


void loop()                    
{
   // Nothing is happening! All reading and printing is done in the interrupt
}
