## The yoga wheelchair

## Project description

Sitting in a wheelchair during long periods of time might increase problems of posture, blood circulation and stiffness in the users’ body. There are so many reasons why yoga can be considered as a therapy for this kind of users, not to mention that it brings together mind and body, increasing well being in general. Posture is improved through different exercises which can also be considered as healing to joints and muscles avoiding stiffness and increasing blood circulation. Furthermore, it must be noted that this practice links exercises with respiration, which will not just increase flexibility but also lung capacity.

## Sensors

The following sensors will be used to verify if each posture made by the user.

* _**Force Sensing Resistor (FSR):**_ This sensor will be used to detect the different yoga postures of the user. The recommended setup is 5 FSR in the back rest and 6 FSR distributed in the bottom cushion.

* _**Force Sensing Resistor (FSR):**_ This sensor will be used to detect the different yoga postures of the user. The recommended setup is 5 FSR in the back rest and 6 FSR distributed in the bottom cushion.

## Research of possible sensors

__**Direction and speed:**__

* Rotatory Encoder: Pretty simple, but we need to connected in the same rotation axis than the wheel; with one part fixed to the wood while the other rotates with the wheel.

* Optical encoding (IR reflective sensor): The assembly of the hardware and the programming might be a bit more complex, but does the same thing as the rotatory encoder.

__**Pressure, weight and applied force detection:**__

* Force Sensing Resistor (FSR): Can be used to detect different postures of the user. The recommended setup is 5 FSR in the back rest and 6 FSR distributed in the bottom cushion.

* Load Cell (Strain Gauge): A bit more complex I think, its generally used to detect changes in weight, for example in a bathroom scale. It could be used to detect the weight progress of the wheelchair user

__**Wheelchair's orientation:**__

* Gyroscope: Can track wheel chairs rotation in three axis. This can detect where the user is pointing (maybe also looking by combining with the position detection system), detect changes on the slope, or detect if the chair overturns.

__**Acceleration:**__

* Accelerometer: Bump detection (Strong upwards inclination followed by downwards inclination)
                 Ramp detection (Strong upwards inclination over a medium distance)
                 Road condition detection (Frequency and intensity of the bumps)
                 Assisted drive detection?

__**Heart Rate (HR):**__

* Fitness tracking devices like fitBit or Apple Watch: They are only accurate for wheelchair users in low intensity tasks (below 100 bpm) [(reference)](https://www.resna.org/sites/default/files/conference/2016/outcomes/tsang.html). Diferent sensors allow to do this kind of tracking, like EKG sensors (Apple Watch) or PPG sensors.

As HR is difficult to track for Wheelchair users, even with a wristband, it is also very difficult to track the Energy Expenditure (EE), which calculation highly depends on the HR tracking.

__**Electrocardiogram (ECG):**__

* ECG sensor: Is able to detect Arrhythmias (AFib episodes) and therefore reduce the risk of strokes in people with decreased mobility. There are some fitness trackers with this capabilities implemented, like Withings Move ECG or Apple Watch.

__**Time:**__

* RTC module: Real time clock

__**Location:**__

* GPS module
* Connect to the GPS of the phone via Bluetooth
____
_________________________________________________________________________________________________

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


Feel free to contact us at jacky@datacentricdesign.org. We welcome feedback, pull requests
or links to your project.
