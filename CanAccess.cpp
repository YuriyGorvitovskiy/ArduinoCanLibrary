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
 
#include <Arduino.h>
#include "CanAccess.h"
#include "CanUtil.h"

CanAccess::CanAccess() :
	reciever(NULL),
	forMessage(NULL),
	forRequest(NULL) {
}

void CanAccess::loop() {
	CanMessage* msg = recieve();
	if (msg == NULL)
		return;
		
	if (msg->isRTR()) {
		if (reciever != NULL)
			reciever->onRequest(*msg);
		if (forRequest != NULL)
			forRequest(*msg);
	} else {
		if (reciever != NULL)
			reciever->onMessage(*msg);
		if (forMessage != NULL)
			forMessage(*msg);
	}
}

boolean CanAccess::transmit(CanMessage& message, boolean isPost, unsigned long waitMillis) {
	if (isPost ? post(message) : send(message))
		return true;
		
	if (&message == NULL)
		return false;
		
	if (waitMillis == 0) {
		while(isPost ? !post(message) : !send(message))
			delay(1); 
		return true;
	}
	
	unsigned long stop = millis() + waitMillis;
	while(stop > millis()) {
		delay(1); 
		if(isPost ? post(message) : send(message))
			return true;
	}
	return false;	
}

CanMessage* CanAccess::txtMessage(const char* string) {
	boolean msg = false;
	boolean ext = false;
	long  eid = 0;
	short sid = 0;
	if (*string == 'X' || *string == 'R') {
		msg = (string[0] == 'X');
		ext = true;

		string += 2;
		eid  = ((long)parseHexDigit(*string++)) << 28;
		eid |= ((long)parseHexDigit(*string++)) << 24;
		++string;
		eid |= ((long)parseHexDigit(*string++)) << 20;
		eid |= ((long)parseHexDigit(*string++)) << 16;
		eid |= ((long)parseHexDigit(*string++)) << 12;
		++string;
		eid |= ((long)parseHexDigit(*string++)) << 8;
		eid |= ((long)parseHexDigit(*string++)) << 4;
		eid |= ((long)parseHexDigit(*string++));
	} else if (*string == 'S' || *string == 'r') {
		msg = (string[0] == 'S');
		ext = false;
		string += 2;
		sid |= ((short)parseHexDigit(*string++)) << 8;
		sid |= ((short)parseHexDigit(*string++)) << 4;
		sid |= ((short)parseHexDigit(*string++));
	} else {
		return NULL;
	}
	
	string += 3;
	byte length = parseHexDigit(*string++);
	if (length < 0 || 8 < length)
		return NULL;

	byte data[8];
	string += 3;
	for (int p = 0; p < length; ++p) {
		data[p]  = (parseHexDigit(*string++) << 4);
		data[p] |=  parseHexDigit(*string++);
		++string;
	}

	if (msg && ext) 
		return &extMessage(eid, length, data);
	else if (msg)
		return &stdMessage(sid, length, data);
	else if (ext)
		return &extRequest(eid, length, data);
		
	return &stdRequest(sid, length, data);
}

boolean CanAccess::post(CanMessage& message, unsigned long waitMillis) {
	return transmit(message, true, waitMillis);
}

boolean CanAccess::send(CanMessage& message, unsigned long waitMillis) {
	return transmit(message, false, waitMillis);
}
