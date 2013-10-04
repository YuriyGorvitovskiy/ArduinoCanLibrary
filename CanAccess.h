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
 
#ifndef __CAN_ACCESS_H__
#define __CAN_ACCESS_H__

#include <Print.h>
#include <SPI.h>
#include "CanMessage.h"

enum CanBitRate {
	Can10kbps,
	Can20kbps,
	Can50kbps,
	Can100kbps,
	Can125kbps,
	Can250kbps,
	Can500kbps,
	Can1Mbps
};

enum CanFilter {
	CanFilterNone,
	CanFilterOlcb
};

enum CanMode {
	CanModeNormal,
	CanModeListen,
	CanModeLoopback
};

//CanMessage is usable till next call
typedef void (*OnMessageFuncton)(CanMessage&);

class CanReceiver {
public:

	//CanMessage is usable till end of the call
	virtual void onMessage(CanMessage& message) = 0;
	virtual void onRequest(CanMessage& message) = 0;
};

class CanAccess {
protected:
	CanReceiver*  		reciever;
	OnMessageFuncton	forMessage;
	OnMessageFuncton	forRequest;
	 
protected:
						CanAccess();
			boolean 	transmit(CanMessage& message, boolean isPost, unsigned long waitMillis);

	virtual CanMessage* recieve() = 0;
	
public:	
	virtual	void 	 	begin(CanBitRate rate, CanFilter filter, CanMode mode) = 0;
			void 		loop();

			void 		subscribe(CanReceiver*);
			void 		subscribe(OnMessageFuncton onMessage, OnMessageFuncton onRequest);
			
	//CanMessage is usable till next call
	virtual CanMessage& stdRequest(short sid, byte length, byte* data) = 0;
	virtual CanMessage& stdMessage(short sid, byte length, byte* data) = 0;
	virtual CanMessage& extRequest(long  eid, byte length, byte* data) = 0;
	virtual CanMessage& extMessage(long  eid, byte length, byte* data) = 0;
	
	//CanMessage is usable till next call
	//in case of wrong format return NULL
	virtual CanMessage* txtMessage(const char* string);
	
	// Sent low priority message
	virtual boolean 	post(CanMessage& message) = 0;
			boolean 	post(CanMessage& message, unsigned long waitMillis); // waitMillis == 0 - try to post forever

	// Sent high priority message
	virtual boolean 	send(CanMessage& message) = 0;
			boolean 	send(CanMessage& message, unsigned long waitMillis); // waitMillis == 0 - try to send forever
	
};

inline void CanAccess::subscribe(CanReceiver* aReceiver) {
	reciever = aReceiver;
}
inline void CanAccess::subscribe(OnMessageFuncton onMessage, OnMessageFuncton onRequest) {
	forMessage = onMessage;
	forRequest = onRequest;
}



#endif