#include "compass.h"

//************************************************************************
//*                     CLASS COMPASS
//************************************************************************

void Compass::read() 
{
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

	#if DEBUG_PRINT_CAL_DATA == 1
		char buf[50];
		sprintf(buf, "%d,\t%d", x, y);
		Serial.println(buf);
	#endif
	
	x = scale(x, Scale::x);
	y = scale(y, Scale::y);
	
	set_raw_x(x);
	set_raw_y(y);
	
	ma_x.filter(x);
	ma_y.filter(y);

	
}

int Compass::scale(int raw_val, Scale::val axis)
{
	int res = 0;
	
	if (axis == Scale::x){
		res = x_sf * raw_val + x_off; 
	}
	if (axis == Scale::y) {
		res = y_sf * raw_val + y_off;
	}
	
	return res;
	 
}

int Compass::get_heading() 
{
	// based on the work of Jordan McConell from SparkFun Electronics

    int x, y, Z;  //triple axis data.  Z is heading

	x = raw_x();
	y = raw_y();
    
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
		Serial.print(F("From compass.get_heading(), Z is:\t"));
		Serial.println(Z);
	#endif
	
	return Z;
}

int Compass::get_smooth_heading() 
{
    int x, y, Z;  //triple axis data.  Z is heading

	x = ma_x.current();
	y = ma_y.current();
    
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
		Serial.print(F("From compass.get_smooth_heading(), Z is:\t"));
		Serial.println(Z);
	#endif
	
	return Z;
}

bool Compass::begin() 
{
	/*
		TODO use compass begin method from compass cal bot because 
		* it checks the device id to ensure there are good comms with the
		* compass
	*/
	
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

void Compass::run() 
{
	read();
	local_msg.heading = get_heading();
	//local_msg.heading = get_smooth_heading();
	pub.publish();
	
}

void Compass::print() 
{
	Serial.print(F("Node id: "));
	Serial.print(id());
	Serial.print("\t");
	Serial.print(name());   
	Serial.print(F("\tconnected via I2C with address: "));
	Serial.println(i2c_address, HEX);
	Serial.print(F("\tPublishing to: "));
	pub.publishing_where();
	Serial.print(F("\tLocal msg is:"));
	local_msg.print();
	
}

