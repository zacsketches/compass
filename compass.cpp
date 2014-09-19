// Generateed by /Volumes/User_data/mybin/genstubs Version 1.1

#include "compass.h"
#include <Wire.h>

//************************************************************************
//*                     CLASS COMPASS
//************************************************************************
int Compass::get_heading() 
{
	// based on the work of Jordan McConell from SparkFun Electronics

    int x, y, z, Z;  //triple axis data.  Z is heading

    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(i2c_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
  
   //Read data from each axis, 2 registers per axis
    Wire.requestFrom(i2c_address, 6);
    if(6<=Wire.available()){
      x = Wire.read()<<8; //x msb
      x |= Wire.read();   //x lsb
      z = Wire.read()<<8; //z msb
      z |= Wire.read();   //z lsb
      y = Wire.read()<<8; //y msb
      y |= Wire.read();   //y lsb
    }
    
    //convert to heading
    if (x>0 && y==0) {   //000 degrees
      Z = 0; 
    }  
    if (x>0 && y>0) {   //001-089 degrees
      Z = (atan2(y, x)) * 180 / M_PI;
    }
    if (x==0 && y>0) {   //090 degrees
      Z = 90;
    }
    if (x<0 && y>0) {    //91-179 degrees
      Z = (atan2(y, x)) * 180 / M_PI;
    }
    if (x<0 && y==0) {   //180 degrees
      Z = 180; 
    }
    if (x<0 && y<0) {    //181-269 degrees
      Z = 360 + (atan2(y, x)) * 180 / M_PI;
    }
    if (x==0 && y<0) {   //270 degrees
      Z = 270;
    }
    if (x>0 && y<0) {    //271-359 degrees
     Z = 360 + (atan2(y, x)) * 180 / M_PI; 
    }
	
    //Invert X if required by the sensor alignment in the project  
    if (invert_x) {
      Z = Z-180;
      if (Z < 0 ) {
        Z = 360 + Z; 
      }
    }
	
	#if DEBUG_COMPASS == 1
		Serial.print(F("From compass.get_heading(), Z is: "));
		Serial.println(Z);
	#endif
	
	return Z;
}

Compass::Compass(Compass_fb<Compass_msg>& feedback, 
	const int _i2c_address,
	const int filter_length; 
	bool _invert_x,
	char* name )
	:Sensor(name), fb(feedback), i2c_address(_i2c_address), invert_x(_invert_x),
	ma(Moving_average(filter_length)
{	
}

bool Compass::attach() 
{
	//initialize I2C bus
    Wire.begin();  
	
	// Put the HMC5883 IC into the correct operating mode the
	// 0x02 mode register sets the operating mode.  Mode 0x00 is
	// continuous measurement mode.
	Wire.beginTransmission(i2c_address); 
    Wire.write(0x02); 
    Wire.write(0x00); 
	Wire.endTransmission();
}

void Compass::update_feedback() 
{
	fb.update(local_msg);
}

void Compass::run() 
{
	static int new_heading;
	new_heading = get_heading();
	local_msg.heading = new_heading;
	local_msg.filtered_heading = ma.filter(new_heading);
	
	update_feedback();
}

void Compass::config() 
{
	Serial.print(F("\nCompass config:"));
	Serial.print(F("\tconnected via I2C with address: "));
	Serial.println(i2c_address, HEX);
}

