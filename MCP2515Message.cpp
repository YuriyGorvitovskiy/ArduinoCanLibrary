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
#include "MCP2515.h"
#include "CanMCP2515.h"

boolean MCP2515Message::isSID() {
	return (buffer[MCP2515_RXBSIDL_INDEX] & MCP2515_RXBSIDL_IDE) == 0;
}

boolean MCP2515Message::isEID() {
	return (buffer[MCP2515_RXBSIDL_INDEX] & MCP2515_RXBSIDL_IDE);
}

boolean MCP2515Message::isRTR() {
	return isEID() ? (buffer[MCP2515_RXBDLC_INDEX ] & MCP2515_RXBDLC_RTR )
				   : (buffer[MCP2515_RXBSIDL_INDEX] & MCP2515_RXBSIDL_SRR)
	;
}

short MCP2515Message::getSID() {
	return (((short) buffer[MCP2515_RXBSIDH_INDEX])                             << MCP2515_RXBSIDH_SID_LSHIFT)
		 | (((short)(buffer[MCP2515_RXBSIDL_INDEX] & MCP2515_RXBSIDL_SID_MASK)) >> MCP2515_RXBSIDL_SID_RSHIFT)
	;
}

void MCP2515Message::setSID(short sid) {
	buffer[MCP2515_TXBSIDH_INDEX] = (sid >> MCP2515_TXBSIDH_SID_RSHIFT); 
	buffer[MCP2515_TXBSIDL_INDEX] = (sid << MCP2515_TXBSIDL_SID_LSHIFT) & MCP2515_TXBSIDL_SID_MASK;
}

long MCP2515Message::getEID() {
	long eid = ((long)getSID()) << 18;
	
	eid |= (((long)(buffer[MCP2515_RXBSIDL_INDEX] & MCP2515_RXBSIDL_EID_MASK)) << MCP2515_RXBSIDL_EID_LSHIFT);
	eid |= (((long)(buffer[MCP2515_RXBEID8_INDEX])) << MCP2515_RXBEID8_EID_LSHIFT);
	eid |=          buffer[MCP2515_RXBEID0_INDEX];
	return eid;
}

void MCP2515Message::setEID(long  eid) {
	setSID(eid >> 18);
	
	buffer[MCP2515_TXBSIDL_INDEX] |= MCP2515_TXBSIDL_IDE;
	buffer[MCP2515_TXBSIDL_INDEX] |= ((eid >> MCP2515_TXBSIDL_EID_RSHIFT) & MCP2515_TXBSIDL_EID_MASK);
	buffer[MCP2515_TXBEID8_INDEX]  = ((eid >> MCP2515_TXBEID8_EID_RSHIFT) & MCP2515_TXBEID8_EID_MASK);
	buffer[MCP2515_TXBEID0_INDEX]  =  (eid                                & MCP2515_TXBEID0_EID_MASK);
}
	
byte MCP2515Message::getDataLength() {
	return (buffer[MCP2515_RXBDLC_INDEX] & MCP2515_RXBDLC_DLC);
}

void MCP2515Message::setRTRData(boolean rtr, byte length, byte* data) {
	length = constrain(length, 0, 8);
		
	buffer[MCP2515_TXBDLC_INDEX] = (length & MCP2515_TXBDLC_DLC);
	if (rtr)
		buffer[MCP2515_TXBDLC_INDEX] |= MCP2515_TXBDLC_RTR;
		
	if (length > 0)
		memcpy(buffer + MCP2515_TXBDM_INDEX, data, length);		
}

byte* MCP2515Message::getData() {
	return buffer + MCP2515_RXBDM_INDEX;
}
