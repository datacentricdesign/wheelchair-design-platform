// Here we have an example of the RGB Rotary Encoder use.


#define EI_ARDUINO_INTERRUPTED_PIN // this is needed so we can see the interrupted pin's state with the variable arduinoPinState - 0 is low, other than 0 means high
// careful, this needs to be defined before the include for the library.

#include <EnableInterrupt.h>
// Library for our interrupt functions

// Let's define our pins
// Rotary LEDS
#define RT_LED_R 52
#define RT_LED_G A1
#define RT_LED_B 53

//Pushbutton
#define RT_BUTTON 10 // careful, not all ports work here, check the last part of setup for details.

// Data pins 
#define RT_A 3  // careful, this pin will be used with interrupts, so it can't be just any pin
#define RT_B A3 // the analog pins can be used as digital pins!

// Let's define LED COLORS, since we're using common anode LEDs, their behaviour is inverted, 0 is on, and 1 is off
#define OFF 0b111 //0b stands for bitwise, 0x for hexadecimal
#define RED 0b110
#define GREEN 0b101
#define YELLOW 0b100
#define BLUE 0b011
#define PURPLE 0b010
#define CYAN 0b001
#define WHITE 0b000 

// Global Variables susceptible to changes from the interrupt routines (thats why the qualifier volatile is used)
volatile int rotary_counter = 0; // current indentations (increments (Clockwise) or decrements (counter Clockwise) 
volatile boolean rotary_change = false; // indicates if the rotary counter has changed
volatile boolean button_pressed = false; // indicates if the pushbutton has been pressed 
volatile boolean button_released = false; // indicates if the pushbutton has been released
volatile unsigned button_downtime = 0L; // timer counting how long pushbutton was pressed before release


// "Static" variables are initialized once the first time the loop runs, but they keep their values through successive loops.

static unsigned char colour = 0; //final LED colour
static boolean button_down = false; 
static unsigned long int button_down_start, button_down_time;

void setup() {
 
  // I/O pin Setup
  
  pinMode(RT_A, INPUT_PULLUP);  // Inverting behaviour of INPUT MODE: HIGH means off, LOW means on

  pinMode(RT_B, INPUT_PULLUP);

  pinMode(RT_BUTTON, INPUT); // since the switch button is a common anode with an external pulldown resistor, do not turn pullup on.

  pinMode(RT_LED_R, OUTPUT);
  pinMode(RT_LED_G, OUTPUT);
  pinMode(RT_LED_B, OUTPUT);
  
  setLED(CYAN);
  
  
  Serial.begin(9600); // setting baud speed for Serial (a baud is a pulse)
  Serial.println("Lets start our RGB Rotary Encoder Example!");

  // Lets assign our interrupts

  // We will use the enableInterrupt library for the button (pinChange interrupts), and for the rotary (external interrupts).
  // in the Arduino mega, external interrupts can be used with pins 2, 3, 18-21, so be sure to use one of those
  enableInterrupt(RT_A, rotary_ISR, CHANGE); // ISR Interrupt service routine, the last parameter is the mode, which calls this, whenever the value in the pin changes.
  // on the mega, the Pin Change interrutps can be had on pins  10-15
  enableInterrupt(RT_BUTTON, button_ISR, CHANGE); // check f
}

void setLED(unsigned char color)
// Set RGB LED to one of eight colors (see #defines above)
{
  digitalWrite(RT_LED_R, color & 0b001); // this & is a mask, essentially deleting everything besides the R, G , B part of the color, respectively for each of the leds.
  digitalWrite(RT_LED_G, color & 0b010); // don't forget each color has a R, G, and a B bit. 
  digitalWrite(RT_LED_B, color & 0b100);
}

 // CAREFUL, FUNCTION MUST NOT RETURN VALUES NEITHER TAKE ANY INPUTS, DO NOT USE Serial.print() inside.
void button_ISR(){
  // Process rotary encoder button presses and releases, including
  // debouncing (noisy switch contacts can trigger false 'presses').
  // If button is pressed, the button_pressed flag is set to true.
  //  (set to false after handling the change)
  // If button is released, the button_released flag is set to true,
  // and button_downtime will contain the duration of the button
  // press in ms. (Set to false after handling the change)
 
  static boolean button_state = false;
  static unsigned long start, end;

  if ((arduinoPinState != 0) && (button_state == false))
    // Button was up, but is currently being pressed down
  {
    // Discard button presses too close together (debounce)
    start = millis();
    if (start > (end + 10)) // 10ms debounce timer
    {
      button_state = true;
      button_pressed = true;
    }
  }
  else if ((arduinoPinState == 0 ) && (button_state == true))
    // Button was down, but has just been released
  {
    // Discard button releases too close together (debounce)
    end = millis();
    if (end > (start + 10)) // 10ms debounce timer
    {
      button_state = false;
      button_released = true;
      button_downtime = end - start;
    }
  }
  
}

void rotary_ISR(){
  // Process input from the rotary encoder.
  // The rotary "position" is held in rotary_counter, increasing for CW rotation , decreasing for CCW rotation (changes by one per detent).
  // If the position changes, rotary_change will be set true. (set  to false after handling the change).

  // This function will automatically run when rotary encoder input A transitions in either direction (low to high or high to low)
  // By saving the state of the A and B pins through two interrupts, we'll determine the direction of rotation
  // int rotary_counter will be updated with the new value, and boolean rotary_change will be true if there was a value change

  static unsigned char rotary_state = 0; // current and previous encoder states

  // this is so the saving is efficient, as we generally want handlers as fast as possible,  but you can do this with 4 static boolian variables, or a boolean array.
  // we're encoding the rotary_state here in a char so, in 8 bits, 0000 0000, the first 2 bits(to the right) encode the current state of the data pin, with a first, 
  // and the two bits after that record the previous state of the encoder
  // 0000  PREV_B PREV_A B A - this is the final result of the char.
  rotary_state <<= 2;  // remember previous state (moving the previous ' current state' to the position of previous states  in the char)
  rotary_state |= (digitalRead(RT_A) | (digitalRead(RT_B) << 1));  // placing (mask) in the current state of the data pins
  rotary_state &= 0x0F; // zero upper nibble just to keep the upper 4 bits always as zero.


  if (rotary_state == 0x09) // data pins go from 10  (previous state) to 01 (current state), increment counter. Also try 0x06 if unreliable
  {
    rotary_counter++;
    rotary_change = true;
  }
  else if (rotary_state == 0x03) // data pins go from 00(previous state) to 11(current state), decrement counter. Also try 0x0C if unreliable
  {
    rotary_counter--;
    rotary_change = true;
  }

}
void loop() {
  // The rotary IRQ sets the flag rotary_counter to true
  // if the knob position has changed. We can use this flag
  // to do something in the main loop() each time there's
  // a change. We'll clear this flag when we're done, so
  // that we'll only do this once for each change.

  
  if (rotary_change)
  {
    Serial.print("rotary: ");
    Serial.println(rotary_counter, DEC); // printing our rotary counter to serial,
    rotary_change = false; // Clear flag
      
    //changing colour for visual feedback
    colour++;
    if( colour & OFF) colour++; // so off doesn't show up.
    setLED(colour);
    delay(10); //try not to make this too long, otherwise the Arduino will miss ticks (10 ms)
    setLED(colour);
  }

  // The button IRQ also sets flags to true, one for
  // button_pressed, one for button_released. Like the rotary
  // flag, we'll clear these when we're done handling them.

  if (button_pressed)
  {
    Serial.println("button press");
    colour++; // cyclic change in the colour
    if( colour & OFF) colour++; // so off doesn't show up.
    setLED(colour); // Change the color of the knob LED
    button_pressed = false; // Clear flag

    // We'll set another flag saying the button is now down
    // this is so we can keep track of how long the button
    // is being held down. (We can't do this in interrupts,
    // because the button state is not changing).

    button_down = true;
    button_down_start = millis();
  }

  if (button_released)
  {
    Serial.print("button release, downtime: ");
    Serial.println(button_downtime, DEC); // printing how long the button was down. (in decimal numbers
    colour++; 
    if( colour & OFF) colour++; // so off doesn't show up.
    setLED(colour); // Change the colour of the knob LED
    button_released = false; // Clear flag

    // Clear our button-being-held-down flag
    button_down = false;

    //blink 4 times when you release the button press in white
    for (int i = 0; i < 4; i++) {
      digitalWrite(RT_LED_R, LOW);
      digitalWrite(RT_LED_G, LOW);
      digitalWrite(RT_LED_B, LOW);
      delay(50);
      digitalWrite(RT_LED_R, HIGH);
      digitalWrite(RT_LED_G, HIGH);
      digitalWrite(RT_LED_B, HIGH);
      delay(50);
    }

    setLED(colour); // Change the color back to original color
  }

  // Now we can keep track of how long the button is being
  // held down, and perform actions based on that time.
  // This is useful for "hold down for five seconds to power off"
  // -type functions.

  if (button_down)
  {
    button_down_time = millis() - button_down_start; // millis() Returns the number of milliseconds since the Arduino board began running the current program

    if (button_down_time > 1000) {
      Serial.println("button held down for one second");
    }
    //if LED is pressed down, display white
    digitalWrite(RT_LED_R, LOW);
    digitalWrite(RT_LED_G, LOW);
    digitalWrite(RT_LED_B, LOW);
  }

}
