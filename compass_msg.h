// Generateed by /Volumes/User_data/mybin/dazl_bot_msg Version 0.1

#ifndef COMPASS_MSG_H
#define COMPASS_MSG_H SEP_2014

#include <arduino.h>
#include <Message.h>

struct Compass_msg : public Message {
	//Data available form the base class
	//	const char* n   //name

	char* name;
	int heading;

	//Minimal constructor
	Compass_msg() : Message("compass") {
		heading = 0;
	}

	//assignment operator
	Compass_msg& operator=(const Compass_msg& a) {
		n = a.n;
		heading = a.heading;
		return *this;
	}

	//print
	void print() {
		Serial.print(F("\t\t\tHeading is: "));
		Serial.println(heading);
	}
};
#endif
