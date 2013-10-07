/**
 ** This is Public Domain Software.
 ** 
 ** The author disclaims copyright to this source code.  
 ** In place of a legal notice, here is a blessing:
 **
 **    May you do good and not evil.
 **    May you find forgiveness for yourself and forgive others.
 **    May you share freely, never taking more than you give.
 **/
 
#ifndef __SIMPLE_CAN_CONSUMER_H__
#define __SIMPLE_CAN_CONSUMER_H__

#include <Arduino.h>
#include <Can.h>

class SimpleCanConsumer : public CanConsumer {
private: 
			int  	id;

public: 	
					SimpleCanConsumer(int id);
			boolean	postMessage(const char* txt);
	virtual void 	onMessage(CanMessage& message);
	virtual void 	onRequest(CanMessage& message);
};

#endif //__SIMPLE_CAN_CONSUMER_H__