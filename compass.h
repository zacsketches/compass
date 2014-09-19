// Generateed by /Volumes/User_data/mybin/dazl_bot_sensor Version 0.1

#ifndef COMPASS_H
#define COMPASS_H SEP_2014

#include <arduino.h>
#include <Sensor.h>
#include <compass_msg.h>
#include <compass_fb.h>

#define DEBUG_COMPASS 1

class Compass : public Sensor {
private:
	//Available from the base class
	//	char* n;	//name

	// private data
	bool invert_x;
	
	//feedback object
	Compass_fb<Compass_msg>& fb;

	//local copy of the message
	Compass_msg local_msg;

	//Physical connection

	//I2C connection
	int i2c_address;

	//*** HELPER FUNCTIONS ***
	int get_heading();

public:
	//Constructor
	Compass(Compass_fb<Compass_msg>& feedback, 
		const int _i2c_address, 
		bool _invert_x = false,
		char* name = "CP");

	//*** PURE VIRUTAL MODIFYING METHODS ***
	bool attach();
	void update_feedback();
	void run();
	void config();

	//*** NON-MODIFYING METHODS ***
	//Available from the base class
	//	char* name;
	//Provide a pointer back to the feedback object
	Compass_fb<Compass_msg>* feedback() {return &fb;}
};

#endif
