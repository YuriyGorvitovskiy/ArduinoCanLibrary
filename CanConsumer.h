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
 
#ifndef __CAN_CONSUMER_H__
#define __CAN_CONSUMER_H__

#include <Print.h>
#include <SPI.h>
#include "CanMessage.h"

class CanAccess;

class CanConsumer {
	friend class 	CanAccess;
	
protected:
	
	CanAccess*		can;	
	CanConsumer*  	next;
	
					CanConsumer();

			void	addConsumer(CanConsumer* consumer);
			void 	handleMessage(CanMessage& message, boolean isRTR, CanConsumer* from);
	
	//CanMessage is usable till end of the call
	virtual void 	onMessage(CanMessage& message); // message handling is optional
	virtual void 	onRequest(CanMessage& request); // request handling is optional

			//implementation of this functions is in CanAccess.h	
			boolean post(CanMessage& message);
			boolean post(CanMessage& message, unsigned long waitMillis); // waitMillis == 0 - try to post forever

			boolean send(CanMessage& message);
			boolean send(CanMessage& message, unsigned long waitMillis); // waitMillis == 0 - try to post forever

public:

	virtual void	begin(CanAccess& can); //Derived class can override this method to do additional initialization but should call CanConsumer::begin(...) 
};


#endif //__CAN_SUBSCRIBER_H__