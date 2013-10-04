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
 
#ifndef __CAN_MCP2515_H__
#define __CAN_MCP2515_H__

#include <SPI.h>
#include "CanAccess.h"
#include "MCP2515Driver.h"
#include "MCP2515Message.h"

class CanMCP2515 : public CanAccess, MCP2515Driver {
private:
	boolean		   debugEnabled;

	boolean		   rxOverload;
	
	MCP2515Message txMessage;
	MCP2515Message rxMessage;
	
private:
			boolean 	postTxMessage();
			boolean 	sendTxMessage();
	
			void 		DEBUG(const char* msg);

	virtual CanMessage* recieve();
	
public:	
						CanMCP2515(byte spiCSpin);
						CanMCP2515(byte spiCSpin, boolean debugToSerial);
					
	virtual	void    	begin(CanBitRate rate, CanFilter filter, CanMode mode);
	
	//CanMessage is usable till next call
	virtual CanMessage& stdRequest(short sid, byte length, byte* data);
	virtual CanMessage& stdMessage(short sid, byte length, byte* data);
	virtual CanMessage& extRequest(long  eid, byte length, byte* data);
	virtual CanMessage& extMessage(long  eid, byte length, byte* data);
	
	// Sent low priority message
	virtual boolean 	post(CanMessage& message);

	// Sent high priority message
	virtual boolean 	send(CanMessage& message);
};


inline void CanMCP2515::DEBUG(const char* msg) {
	if (debugEnabled) 
		Serial.println(msg);
}

#endif //__CAN_MCP2515_H__