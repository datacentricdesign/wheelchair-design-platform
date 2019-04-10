
# Wheel.ID - *the chair that knows you*
 *by William Hagen, Jelle Dott & Rosan Foppen*

## Project Description
 We want to build a safety system that is designed for the wheelchair owner, by making a profile of him/her in order for the system to understand who is sitting/ using the wheelchair. This system works based on the weight, way of sitting and the way someone handles the wheelchair. If the wheelchair is used by the wrong person (not saved in the recognised profiles) an alarm will go off, together with some LEDs.

### Features

* Provides feedback on who is sitting in the chair by audio
* Shows (un)authorized usage by LED strips on the wheels
* Makes a user profile based on posture, weight and frequency of rotation of the wheels

## Components

...

## Installation

The collection of data and the display of (un)authorized usage is achieved by the use of multiple components

*Steps*  

- ...
- ...


## Selection and motivation of the Sensors

- 6 Force sensing resistors
The force sensors will be placed in the seating of the wheelchair and, based on data, these will shape a profile of the main user. This profile can then be differentiated from other users, because the differences in weight and posture etc. vary.

- Accelerometer + gyro
The accelerometer is the second part of the two-step security system, it will measure the acceleration patterns of the main user. People might have very similar weight and postures, therefore this accelerometer will measure usage of the product, and act as a second security feature.

## Selection and motivation of the Actuators

Whenever a person with the incorrect profile uses the wheelchair, the security system will let the surrounding area/people + the main user know that something is wrong. The faulty usage will be made clear on the phone of the user (it’s connected) but also through different actuators on the wheelchair itself, namely:

- Audio
When the correct user uses the wheelchair, the built-in speaker system will welcome him or her and, automatically, the wheelchair will adjust to the user’s preferred settings. When an unauthorized person sits down in the wheelchair, the same audio system will says so.

- Light
There is also a LED-strip that is connected to the wheels. It will shine bright red or green depending on the user.

=======================================================================================

Wheelchair Design Platform is a repository that contains some resources to help
designers and developers speak the same language, and work together towards
addressing relevant challenges for wheelchair users. It is a collection of
workshop materials, code examples and also a compilation of resources to foster
a prospering research and design community around wheelchair users.

![IoT1 Exhibition](/docs/workshops/images/iot1_exhibition.jpg)

## Workshops

* [Getting started](/docs/workshops/GettingStarted.md)
* [Workshop 1: Building an Internet-Connected Wheelchair](/docs/workshops/Workshop1.md)
* [Workshop 2: Integrating and Visualising Sensor-Based Data](/docs/workshops/Workshop2.md)
* [Workshop 3: Developing Algorithms and Controlling Actuators](/docs/workshops/Workshop3.md)
* [Workshop 4: Developing and Conducting a Data Collection Campaign](/docs/workshops/Workshop4.md)
* [Workshop 5: Implementing a Machine Learning Pipeline](/docs/workshops/Workshop5.md)
* [Workshop 6: Developing a Product Analytics Dashboard](/docs/workshops/Workshop6.md)

## Resources

* This platform uses two programming languages, Python on computers and C on
micro-controllers. While descriptions and examples of code should help you
get started, you can find some additional resources
[here](/docs/resources/software.md "Python and C resources").

* Documentation of your project is key,
[here are some tips and examples](/docs/resources/documentation.md "Documentation tips and examples").

* [Git manipulation such as Pull Request](/docs/resources/git.md "Git manipulation").

## Main Components

__**Disclaimer:**__ the design of this platform focuses on flexibility and
technology exploration rather than optimisation.

The main design includes a Raspberry Pi 3 and an Arduino Mega 2560 on the wheelchair frame.

The Arduino Mega is the micro-controller of the platform. Fixed on the main frame of the wheelchair,
it can collect data from sensors (e.g. force sensors, accelerometers), and trigger actions from actuators
(e.g. LEDs, vibration motors).

More on the Arduino Mega can be found [here](/docs/resources/arduino.md "Arduino resources").

Raspberry Pi is a small computer. It is also fixed to the main frame of the wheelchair,
where it can:
* interact with the Arduino Mega via USB to receive data and transmit commands;
* interact with the Internet to transmit commands and receive data;
* store data locally in files;
* run (machine learning) algorithms.

More on the Raspberry Pi can be found [here](/docs/resources/raspberrypi.md "Raspberry Pi resources").

These components fit together as shown on the following diagram. A large powerbank
powers the Raspberry Pi. The Arduino Mega communicates and receives power from the
Raspberry Pi via USB. A Feather (Arduino-like development board) on the wheel connects to
the Raspberry Pi via Bluetooth to sense and actuate from the wheel.

![Main Wheelchair components](/docs/workshops/images/wheechair-components.png)

## List of suggested components:

On the frame:

* 1 Raspberry Pi 3B;
* 1 SD card (Some come directly with NOOBS installed);
* 1 Arduino Mega;
* 1 Large power bank;
* 1 large breadboard;
* 1 USB cable A/micro (Powerbank to Raspberry Pi);
* 1 USB cable A/B (Raspberry Pi to Arduino Mega).

On the wheel:

* 1 Feather (Bluetooth enabled);
* 1 small power bank;
* 1 small breadboard;
* 1 USB cable A/B (power bank to Arduino Uno).


## Contact and Existing projects

* [The hiking wheelchair](https://github.com/cprecioso/wheelchair-design-platform)
* [The EDU wheelchair](https://github.com/ctsai-1/wheelchair-design-platform)
* [Weelchair tracking for basketball players](https://github.com/FabianIDE/wheelchair-design-platform)
* [Disco Wheelchair](https://github.com/MatthijsBrem/wheelchair-design-platform)
* [Wheelchair Madness 2222](https://github.com/pherkan/wheelchair-design-platform/tree/master/wheelchair)
* [Who is sitting?](https://github.com/Rosanfoppen/wheelchair-design-platform/tree/master/wheelchair)
* [Magic Wheelchair](https://github.com/Yuciena/wheelchair-design-platform)
* [Yoga Wheelchair](https://github.com/artgomad/wheelchair-design-platform)


Feel free to contact us at jacky@datacentricdesign.org. We welcome feedback, pull requests
or links to your project.
