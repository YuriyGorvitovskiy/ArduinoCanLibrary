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
 
#ifndef __CAN_MESSAGE_H__
#define __CAN_MESSAGE_H__

#include <Print.h>

class CanMessage {
private:	
			void printSID(Print& to);
			void printEID(Print& to);
			void printData(Print& to);

public:	
	virtual boolean	isSID()   = 0;
	virtual boolean	isEID()   = 0;
	virtual boolean	isRTR()   = 0;
	
	virtual short getSID() = 0;
	virtual long  getEID() = 0;
	
	virtual byte  getDataLength() = 0;
	virtual byte* getData()       = 0;
	
			void print(Print& to);
			void println(Print& to);
			
};

#endif