#ifndef COMPASS_H
#define COMPASS_H SEP_2014

#include <arduino.h>

#include <Filter.h>
#include <Wire.h>
#include <clearinghouse.h>
#include <messages/heading.h>

//************************************************************************
//*                         COMPASS
//* Read an HMC5883 magnetometer, calculate heading and publish the
//* heading message.
//************************************************************************

#define DEBUG_COMPASS 0
#define DEBUG_PRINT_CAL_DATA 0

extern Heading_msg h1;
extern gw::Clearinghouse ch;

namespace Scale{
	enum val{x, y};
}

class Compass : public gw::Node {
private:

	// private data
	bool invert_x;
	int rx;		//raw x
	int ry;		//raw y
	Moving_average ma_x;
	Moving_average ma_y;
	
	//calibration data from 18 Sep 2014 saved on RSNS laptop
	const static int x_off = 91;
	const static float x_sf = 1.0;
	const static int y_off = 153;
	const static float y_sf = 1.0786;

	//Publisher and local copy of the message
	gw::Publisher<Heading_msg> pub;
	Heading_msg local_msg;

	//I2C connection
	int i2c_address;

	//*** HELPER FUNCTIONS ***
	void read();
	int scale(int raw_val, Scale::val axis);
	int get_heading();
	int get_smooth_heading();

public:
	//Constructor
	Compass(const int _i2c_address = 0x1E,	//default hmc5883 address
		const int filter_length = 5, bool _invert_x = false)
		: Node("Compass"), 
		pub(&h1, &ch, local_msg),
		i2c_address(_i2c_address), 
		invert_x(_invert_x),
		ma_x(Moving_average(filter_length)), 
		ma_y(Moving_average(filter_length))
	{}
		
	// From Base class
    //    char* name()
	//    int id()
	
	bool begin();
	void run();		//default is smooth heading...edit cpp file for raw heading
	void print();
	
	//set methods
	void set_raw_x(const int new_x) { rx = new_x;}
	void set_raw_y(const int new_y) { ry = new_y;}

	//access methods
	int raw_x() {return rx;}
	int raw_y() {return ry;}
};

#endif
