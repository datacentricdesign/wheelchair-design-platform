# IOT Assignment 1 - Group 01

# Context
We want to build a safety system that is designed for the wheelchair owner, by making a profile of him/her in order for the system to understand who is sitting/ using the wheelchair. This system works on the basis of the weight, way of sitting and the way someone handles the wheelchair. If the wheelchair is used by the wrong person (not saved in the recognised profiles) an alarm will go off (maybe also some LED lights).

# 2 different sensors
* 4-6 Force sensors (square)
The force sensors will be placed in the seating of the wheelchair and, based on data, these will shape a profile of the main user. This profile can then be differentiated from other users, because the differences in weight and posture etc. vary.
* Accelerometer + gyro
The accelerometer is the second part of the two-step security system, it will measure the acceleration patterns of the main user. People might have very similar weight and postures, therefore this accelerometer will measure usage of the product as well, and act as a second security feature.

# Actuators
Whenever a person with the ‘wrong’ profile uses the wheelchair, the security system will let the main user know that something is wrong. The faulty usage will be made clear by:
* Speakers
* LED’s / LED string
