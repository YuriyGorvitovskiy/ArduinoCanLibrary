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
#include "CanMessage.h"


void CanMessage::printSID(Print& to) {
	short sid = getSID();
	to.print((sid >> 8) & 0xF, HEX);  
	to.print((sid >> 4) & 0xF, HEX);
	to.print( sid       & 0xF, HEX);
}

void CanMessage::printEID(Print& to) {
	long xid = getEID();
	to.print((xid >> 28) & 0xF, HEX);
	to.print((xid >> 24) & 0xF, HEX);
	to.print(".");
	to.print((xid >> 20) & 0xF, HEX);
	to.print((xid >> 16) & 0xF, HEX);
	to.print((xid >> 12) & 0xF, HEX);
	to.print(".");
	to.print((xid >> 8) & 0xF, HEX);
	to.print((xid >> 4) & 0xF, HEX);
	to.print( xid       & 0xF, HEX);
}

void CanMessage::printData(Print& to) {
	byte length = getDataLength();
	to.print(" L:");
	to.print(length, DEC);
	if (length <= 0)
		return;
		
	byte* data = getData();
	for (int i = 0; i < length; ++i) {
         if (i == 0)
             Serial.print(" D:");
         else
             Serial.print(".");
         Serial.print(data[i] >> 4,  HEX);
         Serial.print(data[i] & 0XF, HEX);
     }
}

void CanMessage::print(Print& to) {
	if (isEID()) {
		to.print(isRTR() ? "R:" : "X:"); 
		printEID(to);
	} else {
		to.print(isRTR() ? "r:" : "S:"); 
		printSID(to);
	}
	printData(to);
}

void CanMessage::println(Print& to) {
	print(to);
	to.println("");
}
