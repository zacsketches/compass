// Generateed by /Volumes/User_data/mybin/dazl_bot_msg Version 0.1

#ifndef COMPASS_FB_H
#define COMPASS_FB_H SEP_2014

#include <arduino.h>
#include <Feedback.h>
#include <compass_msg.h>

template<class MSG>
struct Compass_fb : public Feedback<MSG> {
	//Data available form the base class
	//	MSG msg

	//constructor
	explicit Compass_fb() {
		Feedback<MSG>::msg = Compass_msg();
	}

	//update method
	void update(const Compass_msg& update) {
		Feedback<MSG>::msg = update;
	}

	const Compass_msg& message() const {return Feedback<MSG>::msg; }

	void print() {
		Feedback<MSG>::msg.print();
	}
};
#endif
