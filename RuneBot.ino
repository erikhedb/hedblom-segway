// Erik Hedblom - Sweden 
//
// 0.1 2014-XX-XX Base version
// 0.2 2016-04-20 Runing and working version

// With thanks to JD Warren and Gaston Marzoratti for IMU i2c mod.
//
// Sparkfun IMU
// I2C 6DOF WITH  ITG3200/ADXL345 
// https://www.sparkfun.com/products/10121
// 
// Dimensionengineering
// 2 channel motor controll 25A
// https://www.dimensionengineering.com/products/sabertooth2x25
//
// Motor controled via serial interface. 
// Value 64 +- 64 for M1
// Value 192 +- 64 for M2
// 
// PITCH * GAIN +- STEER VALUE = 0-127 for M1
// PITCH * GAIN +- STEER VALUE = 128-255 for M2
//

// IMU INCLUDE
#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>
#include <Wire.h>

// SABERTOOTH INCLUDE
#include <SabertoothSimplified.h>

#define DEBUG 1               // 0 OR 1
#define MOTOR_CHANNEL 1      // DIGITAL TX OUT
#define CUTTOFANGLE 30        //USED FOR SECURITY REASONS
#define ENGAGEBUTTON 2       //Digital input 1 as input for engage button
#define STEERPOT 1           //ANALOG INPUT 1
#define GAINPOT 2            //ANALOG INPUT 2
#define LEDPIN 13              //ON INDICATION

//FreeSixIMU object and local variables
FreeSixIMU sixDOF = FreeSixIMU(); 
SabertoothSimplified ST;

// Controll Variables
float fPitch = 0.0;
int iSteerValue = 0;
int iGainValue = 0;

int iMotorR = 0;
int iMotorL = 0;

void setup(){

  SabertoothTXPinSerial.begin(9600);
  //Serial.begin(9600);

  //Init and sleep 5 milis to wait for proper startup
  Wire.begin();
  delay(5);
  sixDOF.init();

  //Sleep 5 milis
  delay(5);

}

void loop(){

  //FETCH INBUT VALUES 
  fPitch = getPitch();
  iSteerValue = getSteerValue();
  iGainValue = getGainValue();

  //ONLY LOOP WHEN ENGAGED AND iGainValue LESS THEN CUTTOFANGLE
  if(getEngadeButtonValue()){
    
      iMotorR = fPitch * iGainValue + iSteerValue;
      iMotorL = fPitch * iGainValue - iSteerValue;
     
      iMotorR = iMotorR<127?iMotorR:127;
      iMotorR = iMotorR>-127?iMotorR:-127;
      iMotorL = iMotorL<127?iMotorL:127;
      iMotorL = iMotorL>-127?iMotorL:-127;

      ST.motor(1,iMotorL);
      ST.motor(2,iMotorR);
     
  }else{
     iMotorR = 0;
     iMotorL = 0;
     ST.stop();
     ST.stop();
  }

  //serial_print_stuff();

}

/********************
* Return Steer Value
********************/
int getSteerValue(){
  
  #define STEER_RANGE 75
  
  int steerRaw =  analogRead(STEERPOT);
  int iVal = map(steerRaw, 0, 1023, -STEER_RANGE, STEER_RANGE);   
  return  iVal*sqrt(abs(iVal));
}

/********************
* Return Steer Value
********************/
int getGainValue(){
  //First I tried 6 but this is way to small
  // Now 20
  #define GAIN_RANGE 10
  
  int gainRaw =  analogRead(GAINPOT);
  return  map(gainRaw, 0, 1023,1,GAIN_RANGE);  
}


/*******************
* RETURN ENGAGE BUTTON VALUE
*******************/
int getEngadeButtonValue(){
  return !digitalRead(ENGAGEBUTTON);
}

/********************
* Calculate and pitch
********************/
float getPitch(){
  #define Xi 2                // X axis on IMU
  #define Yi 1                //Y axis on IMU
  #define Zi 0                //Z Axis om IMU

  float angles[3];
  float angle = 0.00;
  
  float zRad;
  double zSin;
  double zCos;
  
  sixDOF.getEuler(angles);
  zRad = ((angles[Zi] * 71) / 4068);
  zSin = sin(zRad);
  zCos = cos(zRad);

  float fCalcPitch = (angles[Xi] * zCos) - (angles[Yi] * zSin);
  return fCalcPitch*sqrt(abs(fCalcPitch));

}

void serial_print_stuff(){

  Serial.print("Steer: ");
  Serial.print(iSteerValue);
  
  Serial.print("\tGAIN: ");
  Serial.print(iGainValue);
  
  
  Serial.print("\t\t Pitch: ");
  Serial.print(fPitch);
 
 
  Serial.print("\t\t Motor Right : ");
  Serial.print(iMotorR);
  Serial.print("\t\t Motor Left: ");
  Serial.println(iMotorL);
}



