// Generateed by /Volumes/User_data/mybin/dazl_bot_sensor Version 0.1

#ifndef COMPASS_H
#define COMPASS_H SEP_2014

#include <arduino.h>

#include <Filter.h>

#include <Sensor.h>
#include <compass_msg.h>
#include <compass_fb.h>

#define DEBUG_COMPASS 1
#define DEBUG_PRINT_CAL_DATA 0

namespace Scale{
	enum val{x, y};
}

class Compass : public Sensor {
private:
	//Available from the base class
	//	char* n;	//name


	// private data
	bool invert_x;
	int rx;		//raw x
	int ry;		//raw y
	Moving_average ma_x;
	Moving_average ma_y;
	
	//calibration data
	int x_off;
	float x_sf;
	int y_off;
	float y_sf;
	
	//feedback object
	Compass_fb<Compass_msg>& fb;

	//local copy of the message
	Compass_msg local_msg;

	//Physical connection

	//I2C connection
	int i2c_address;

	//*** HELPER FUNCTIONS ***
	int get_heading();
	int get_smooth_heading();
	void read();
	int scale(int raw_val, Scale::val axis);

public:
	//Constructor
	Compass(Compass_fb<Compass_msg>& feedback, 
		const int _i2c_address,
		const int filter_length = 1, 
		bool _invert_x = false,
		char* name = "CP");

	//*** PURE VIRUTAL MODIFYING METHODS ***
	bool attach();
	void update_feedback();
	void run();
	void config();
	
	//set methods
	void set_raw_x(const int new_x) { rx = new_x;}
	void set_raw_y(const int new_y) { ry = new_y;}

	//*** NON-MODIFYING METHODS ***
	//Available from the base class
	//	char* name;
	//Provide a pointer back to the feedback object
	Compass_fb<Compass_msg>* feedback() {return &fb;}

	//access methods
	int raw_x() {return rx;}
	int raw_y() {return ry;}
};

#endif
