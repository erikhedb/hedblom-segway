# hedblom-segway
This is the code and driver for a Segway Clone based on JD Warren and Gaston Marzoratti work.


Erik Hedblom - Sweden 

0.1 2014-XX-XX Base version

0.2 2016-04-20 Runing and working version



With thanks to JD Warren and Gaston Marzoratti for IMU i2c mod.


Sparkfun IMU

I2C 6DOF WITH  ITG3200/ADXL345 

https://www.sparkfun.com/products/10121

 
Dimensionengineering

2 channel motor controll 25A

https://www.dimensionengineering.com/products/sabertooth2x25


Motor controled via serial interface. 

Value 64 +- 64 for M1

Value 192 +- 64 for M2
 

PITCH * GAIN +- STEER VALUE = 0-127 for M1

PITCH * GAIN +- STEER VALUE = 128-255 for M2
