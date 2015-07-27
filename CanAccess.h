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
#include "CanConsumer.h"

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

class CanAccess {
friend class CanConsumer;

protected:
	
	CanConsumer*		consumer;
	
	OnMessageFuncton	forMessage;
	OnMessageFuncton	forRequest;
	 
protected:
						CanAccess();
			void 		addConsumer(CanConsumer* consumer);

	virtual CanMessage* recieve() = 0;
	
	virtual boolean		doPost(CanMessage& message) = 0;
	virtual boolean		doSend(CanMessage& message) = 0;

			boolean 	transmit(CanMessage& message, boolean isPost, unsigned long waitMillis, void* consumer);
			void 		handleMessage(CanMessage& message, boolean isRTR, void* ignore);

public:	
			void 	 	begin(CanBitRate rate);
	virtual	void 	 	begin(CanBitRate rate, CanFilter filter, CanMode mode) = 0;
			void 		loop();

			void 		subscribe(OnMessageFuncton onMessage);
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
			boolean 	post(CanMessage& message); 							 // same as waitMillis = CAN_NO_WAIT
			boolean 	post(CanMessage& message, unsigned long waitMillis); // waitMillis: timeout(ms), CAN_NO_WAIT, CAN_FOREVER

	// Sent high priority message
			boolean 	send(CanMessage& message); 							 // same as waitMillis = CAN_NO_WAIT
			boolean 	send(CanMessage& message, unsigned long waitMillis); // waitMillis: timeout(ms), CAN_NO_WAIT, CAN_FOREVER
	
};

inline void CanAccess::begin(CanBitRate rate) {
	begin(rate, CanFilterNone, CanModeNormal);
}

inline void CanAccess::subscribe(OnMessageFuncton onMessage) {
	forMessage = onMessage;
	forRequest = onMessage;
}

inline void CanAccess::subscribe(OnMessageFuncton onMessage, OnMessageFuncton onRequest) {
	forMessage = onMessage;
	forRequest = onRequest;
}

inline boolean CanAccess::post(CanMessage& message) {
	return transmit(message, true, CAN_NO_WAIT, this);
}

inline boolean CanAccess::post(CanMessage& message, unsigned long waitMillis) {
	return transmit(message, true, waitMillis, this);
}

inline boolean CanAccess::send(CanMessage& message) {
	return transmit(message, false, CAN_NO_WAIT, this);
}

inline boolean CanAccess::send(CanMessage& message, unsigned long waitMillis) {
	return transmit(message, false, waitMillis, this);
}

inline boolean CanConsumer::post(CanMessage& message) {
	return can->transmit(message, true, CAN_NO_WAIT, this);
}

inline boolean CanConsumer::post(CanMessage& message, unsigned long waitMillis) {
	return can->transmit(message, true, waitMillis, this);
}

inline boolean CanConsumer::send(CanMessage& message) {
	return can->transmit(message, false, CAN_NO_WAIT, this);
}

inline boolean CanConsumer::send(CanMessage& message, unsigned long waitMillis) {
	return can->transmit(message, false, waitMillis, this);
}


#endif