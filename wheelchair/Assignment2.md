# Assignment 1 - Wheelchair Design

The first assignment focuses on the context, explores the sensors that will be used and shows a sample on the DCD Hub.

Jooyoung, Lotte and Ferkan


## Context of our Internet-Connected wheelchair
Dance is moving rhythmically to music, following a sequence of steps. It has many healthy benefits, besides it being fun. And while dancing is pretty accessible for most people, it is not for wheelchair users. This is also the case with arcades that are filled with entertainment machines such as the well-known dance arcade machine.

The context of our internet-connected wheelchair focuses on bringing the same fun experience of dancing at an arcade, but for users that are in a wheelchair.

Wheelchair users shouldn’t be limited by their wheelchair, instead they should embrace it and we as designers should provide them with fun solutions.


## Sensor exploration
The sensors that will be used consist of the IMU Sensor and the Grove Touch sensor. These two sensors will provide enough data that can be used to provide the user with feedback for doing a certain task. The list below contains three more sensors that might always be added if more data is needed or whenever the first two sensors are not sufficient.

Sensors | Data | Info on the context
--- | --- | ---
*IMU Sensor* | Acceleration, X-Coordinate, Y-Coordinate | Detects velocity and dance movement of the wheelchair and check if they are dancing as guided
*Grove Touch Sensor* | Output if touched: HIGH | Detects if sensor is touched. Could be implemented on the anywhere on the wheelchair (in reach) and on the dance floor.
*Force Sensing Resistors (FSRs)* | Change in resistance as output that represents the force | We can play soft and stronger touches. Stroke the armrests or hit it.
*APDS 9960 Gesture Sensors* | Tracked directional location of a wheelchair | Sensor will sense the directional movement of the wheelchair and use this data to generate LED lights
*BNO055 Intelligent 9-axis absolute orientation sensor* | It measures the orientation on 9-axis. | Using this sensor we can track whether the dancer is making the right moves in the right direction.


## Wheelchair plan
To make everything work, certain elements are expected to work in a certain order. Firstly, we start with stimuli given to the user (1). This can consist of light, sound and vibrations. After that the user takes action based on this stimuli (2) and the data is being read (3). This data will be processed (4) to see how the user performed and based on that feedback (5) will be created. This feedback is given to the user (6).

![Wheelchair Plan](images/wheelchair_plan.jpg)
