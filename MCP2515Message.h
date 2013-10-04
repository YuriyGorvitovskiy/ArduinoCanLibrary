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
 
#ifndef __CAN_MCP2515_MESSAGE_H__
#define __CAN_MCP2515_MESSAGE_H__

#include "CanMessage.h"
#include "MCP2515.h"

class MCP2515Message : public CanMessage {
public:
	byte  buffer[13];

	void  setSID(short sid);
	void  setEID(long  eid);
	void  setRTRData(boolean rtr, byte length, byte* data);

public:	
	virtual boolean	isSID();
	virtual boolean	isEID();
	virtual boolean	isRTR();
	
	virtual short getSID();
	virtual long  getEID();
	
	virtual byte  getDataLength();
	virtual byte* getData();
};

#endif //__CAN_MCP2515_MESSAGE_H__