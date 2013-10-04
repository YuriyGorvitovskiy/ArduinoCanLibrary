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
 
#ifndef __MCP2515_DRIVER_H__
#define __MCP2515_DRIVER_H__

#include <SPI.h>

class MCP2515Driver {
private:
	byte			  spiCSpin;

protected:
			MCP2515Driver(byte spiCSpin);
			
	void	begin();
			
	void 	startSpi();
	void 	stopSpi();

	byte 	readByte(byte address);
	void 	writeByte(byte address, byte value);
	void 	writeBytes(byte address, byte value0, byte value1, byte value2, byte value3); 
	void 	modifyByte(byte address, byte mask, byte value);

	void 	reset();
	void 	switchMode(byte mode);
	
	void 	setup10kbps();
	void 	setup20kbps();
	void 	setup50kbps();
	void 	setup100kbps();
	void 	setup125kbps();
	void 	setup250kbps();
	void 	setup500kbps();
	void 	setup1Mbps();
	
	void 	setupNoFilters();
	void 	setupOlcbFilters();

	byte 	readStatus();
	byte 	readRxStatus();
	
	void 	setTxPriority(byte addr, byte priority);
	void 	writeTxBuffer(byte buffer, byte priority, byte* data);

};

inline void MCP2515Driver::startSpi() {
	digitalWrite(spiCSpin, LOW);
}

inline void MCP2515Driver::stopSpi() {
	digitalWrite(spiCSpin, HIGH);
}

#endif //__MCP2515_DRIVER_H__