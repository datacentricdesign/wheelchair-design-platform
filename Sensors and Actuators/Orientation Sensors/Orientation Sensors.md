# Orientation sensors
These sensors serve to provide orientation data, and they normally mix several different sensors such as accelerometers, gyroscopes and magnetometers.

## BNO055 Intelligent 9-axis absolute orientation sensor
This 9-DOF (Degrees of freedom) sensor turns the sensor data from an accelerometer, gyroscope and magnetometer into 3D space orientation.
These so called sensor fusion algorithms that blend accelerometer, magnetometer and gyroscope data into stable three-axis orientation output can be difficult to implement on low cost real time systems.

The BNO055 manages to get this right by taking a MEMS (Microelectromechanical systems) accelerometer, magnetometer and gyroscope and putting them on a single die with a high speed ARM Cortex-M0 based processor to process all the sensor data, abstract away sensor fusion and real time requirements, and output data you can use in quaternions, Euler angles or vectors.

![](1.png)

### Technical details
The data from this board can be accessed in several formats, namely:
* Absolute Orientation (Euler Vector, 100Hz) - Three axis orientation data based on a 360° sphere;
* Absolute Orientation (Quaternion, 100Hz) - Four point quaternion output for more accurate data manipulation;
* Angular Velocity Vector (100Hz) - Three axis of 'rotation speed' in rad/s;
* Acceleration Vector (100Hz) - Three axis of acceleration (gravity + linear motion) in m/s^2;
* Magnetic Field Strength Vector (20Hz) - Three axis of magnetic field sensing in micro Tesla (uT);
* Linear Acceleration Vector (100Hz) - Three axis of linear acceleration data (acceleration minus gravity) in m/s^2;
* Gravity Vector (100Hz) - Three axis of gravitational acceleration (minus any movement) in m/s^2;
* Temperature (1Hz) - Ambient temperature in degrees celsius;

### Pins
Now, lets see what each pin of this board is for:

![](2.png)
