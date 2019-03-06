#Rotary Encoders
Encoders can measure the rotation of a shaft. There are two main types of encoders:
* Absolute - this type of encoders relate information about the absolute position of the shaft;
* Incremental - this type of encoders relate information on the relative motion of the shaft of the encoder, i.e. changes in the position of its shaft, but not its absolute position.
## Sparkfun RGB rotary encoder
The Sparkfun RGB rotary encoder is one of the most common types for general electronics, a quadrature rotary encoder (a type of incremental encoder), and is rather similar to a potentiometer; however its rotation is infinite in both directions. It also possesses full RGB illumination of the shaft, with the shaft also doubling as a clickable button.

### Functionality

This encoder is conveniently soldered into a breakout board, for easy use with a breadboard and access of its I/O.  With the help of the following diagram we can see how this encoder works.

![](1.png)

1. B contact, one of the data pins;
2. "Common" contact - to be connected to ground;
3. A contact, another of the data pins;
4. Input to Red LED;
5. Input to Green LED;
6. "Switch" contact, LOW by default, switches to HIGH when shaft is clicked;
7. Input to Blue LED;
8. "Vcc" connector, to be connected to 3.3V.

Let's review how an rotary encoder works:
![](2.gif)

The incremental rotary encoder works in a very simple way. There are 3 terminals, , where C is the common, which is connected to ground. The rotary encoder also has metallic contacts connected to VCC. As you can see, the data pins are phased between each other, and during the turning of the wheel, when the pin touches the contact it will be 1, and 0 for when it touches the ground. The data streams are shifted 90˚(hence quadrature) from each other (since there's a particular distance between the contacts).
Why use two data streams? Well to figure out which direction we're turning it, since you can tell which data stream comes first!

#### Diagram of the circuit
Here follows a diagram of a test circuit using the Arduino Mega.
The LEDS  current is 25 mA. They have the following forward voltages(voltage drop), so if you are using a different Vcc than 5V, you must calculate other resistances (check the LED tutorial):
R : 2V
G : 3.3V
B : 3.3V

![](3.png)

#### Results
Now you should have a working RGB encoder! Below you can find what your serial output should look like:

![](4.gif)
