# Assignment 2 - Wheelchair Design

The second assignment focuses on selecting and implementing the sensors on the wheelchair. Afterwards a collection of samples will be obtained with Grafana. Finally, actuators will be explored and 3 actuators will be described with their behaviour and the motivation to use in our project.


## Grafana dashboard with samples of data



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
