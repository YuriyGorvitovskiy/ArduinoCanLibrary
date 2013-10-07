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
#include "CanUtil.h"
#include "MCP2515.h"
#include "CanMCP2515.h"

CanMCP2515::CanMCP2515(byte spiCSpin) :
	MCP2515Driver(spiCSpin),
	debugEnabled(false),
	rxOverload(false) {
}	 

CanMCP2515::CanMCP2515(byte spiCSpin, boolean debugToSerial) :
	MCP2515Driver(spiCSpin),
	debugEnabled(debugToSerial),
	rxOverload(false) {
}	 

void CanMCP2515::begin(CanBitRate rate) {
	CanAccess::begin(rate);
}

void CanMCP2515::begin(CanBitRate rate, CanFilter filter, CanMode mode) {
	rxOverload   = false;
	MCP2515Driver::begin();
    
    reset();
    DEBUG("Config");
    switchMode(MCP2515_CANSTAT_OPMOD_CONFIG);
    
    // Setting up CAN 
    switch(rate) {
		case Can10kbps:	
			setup10kbps();
			break;
		case Can20kbps:
			setup20kbps();
			break;
		case Can50kbps:
			setup50kbps();
			break;
		case Can100kbps:
			setup100kbps();
			break;
		case Can125kbps:
			setup125kbps();
			break;
		case Can250kbps:
			setup250kbps();
			break;
		case Can500kbps:
			setup500kbps();
			break;
		case Can1Mbps:
			setup1Mbps();
			break;
		default:
			DEBUG("!BitRate!");
			return;
    }
    
    writeByte( MCP2515_ADDR_CANINTE
             , MCP2515_CANINTE_MERRE_OFF 
			 | MCP2515_CANINTE_WAKIE_OFF
			 | MCP2515_CANINTE_ERRIE_OFF
			 | MCP2515_CANINTE_TX2IE_OFF
			 | MCP2515_CANINTE_TX1IE_OFF
			 | MCP2515_CANINTE_TX0IE_OFF
			 | MCP2515_CANINTE_RX1IE_OFF
			 | MCP2515_CANINTE_RX0IE_OFF
    );
    writeByte(MCP2515_ADDR_CANINTF, 0);

    modifyByte( MCP2515_ADDR_CANCTRL
              , MCP2515_CANCTRL_OSM_MASK
              , MCP2515_CANCTRL_OSM_OFF
    );

    modifyByte( MCP2515_ADDR_CANCTRL
 			  , MCP2515_CANCTRL_CLKEN_MASK | MCP2515_CANCTRL_CLKPRE_MASK
 			  , MCP2515_CANCTRL_OSM_OFF    | MCP2515_CANCTRL_CLKPRE_1
    );

    // Setting up Transfering Buffers
    writeByte( MCP2515_ADDR_TXRTSCTRL
             , MCP2515_TXRTSCTRL_B2RTS_ZERO
			 | MCP2515_TXRTSCTRL_B1RTS_ZERO
			 | MCP2515_TXRTSCTRL_B0RTS_ZERO
			 | MCP2515_TXRTSCTRL_B2RTSM_DIM
			 | MCP2515_TXRTSCTRL_B1RTSM_DIM
			 | MCP2515_TXRTSCTRL_B0RTSM_DIM
    );

    // Setting up Recieving Buffers
    writeByte( MCP2515_ADDR_BFPCTRL
    		 , MCP2515_BFPCTRL_B1BFS_ZERO
			 | MCP2515_BFPCTRL_B0BFS_ZERO
			 | MCP2515_BFPCTRL_B1BFE_PIN_OFF
			 | MCP2515_BFPCTRL_B0BFE_PIN_OFF
			 | MCP2515_BFPCTRL_B1BFM_DOM
			 | MCP2515_BFPCTRL_B0BFM_DOM
    );
    switch(filter) {
	    case CanFilterNone: 
	    		setupNoFilters();
	    		DEBUG("NO Filter");
	    		break;
	    		
		case CanFilterOlcb: 
				setupOlcbFilters();
				DEBUG("OLCB Filter");
				break;
				
    	default:	        
    			DEBUG("!Filter!");
    			return;
    }
    
    // Switch to requested mode
    switch(mode) {
    	case CanModeNormal:   
    			DEBUG("Normal");
    			switchMode(MCP2515_CANCTRL_REQOP_NORMAL);   
    			break;
    			
    	case CanModeListen:   
    			DEBUG("Listen");
    			switchMode(MCP2515_CANCTRL_REQOP_LISTEN);   
    			break;
    			
    	case CanModeLoopback: 
		    	DEBUG("Loopback");
    			switchMode(MCP2515_CANCTRL_REQOP_LOOPBACK); 
    			break;
    			
    	default:	          
    			DEBUG("!Mode!"); 						      
    			break;
	}
	DEBUG("CAN Ready");
}

CanMessage& CanMCP2515::stdRequest(short sid, byte length, byte* data) {
	txMessage.setSID(sid);
	txMessage.setRTRData(true, length, data);
	return txMessage;
}

CanMessage& CanMCP2515::stdMessage(short sid, byte length, byte* data) {
	txMessage.setSID(sid);
	txMessage.setRTRData(false, length, data);
	return txMessage;
}

CanMessage& CanMCP2515::extRequest(long  eid, byte length, byte* data) {
	txMessage.setEID(eid);
	txMessage.setRTRData(true, length, data);
	return txMessage;
}

CanMessage& CanMCP2515::extMessage(long  eid, byte length, byte* data) {
	txMessage.setEID(eid);
	txMessage.setRTRData(false, length, data);
	return txMessage;
}

// Sent low priority message
boolean CanMCP2515::doPost(CanMessage& message) {
	if (&message == NULL)
		return false;
		
	if (debugEnabled)
		message.print(Serial);
	
	byte status = (readStatus() & (MCP2515_STATUS_TX0REQ | MCP2515_STATUS_TX1REQ));
	if (status == (MCP2515_STATUS_TX0REQ | MCP2515_STATUS_TX1REQ)) {
		DEBUG("-FAIL");
		return false;
	}
	
	if (status & MCP2515_STATUS_TX0REQ) {
		setTxPriority(MCP2515_ADDR_TXB0CTRL, 1);
		writeTxBuffer(1, 0, ((MCP2515Message&)message).buffer);
	} else {
		setTxPriority(MCP2515_ADDR_TXB1CTRL, 1);
		writeTxBuffer(0, 0, ((MCP2515Message&)message).buffer);
	}
	DEBUG("-OK");
	return true;
}

boolean CanMCP2515::doSend(CanMessage& message) {
	if (&message == NULL)
		return false;

	if (debugEnabled)
		message.print(Serial);

	//Buffer 2 reserved for High Priority message
	if (readStatus() & MCP2515_STATUS_TX2REQ) {
		DEBUG("-FAIL");
		return false;
	}

	writeTxBuffer(2, 3, ((MCP2515Message&)message).buffer);
	DEBUG("-OK");
	return true;
}

CanMessage* CanMCP2515::recieve() {
	byte stat = readRxStatus();
    if ((stat & MCP2515_RX_STATUS_RXB_MASK) == 0)
        return NULL;

    startSpi();
    if (stat & MCP2515_RX_STATUS_RXB1) {
    	if (rxOverload)
	        SPI.transfer(MCP2515_SPI_INSTR_READ_RX(1, 0));
	    else 
		    SPI.transfer(MCP2515_SPI_INSTR_READ_RX(0, 0));
		rxOverload = !rxOverload;    
    } else {
        SPI.transfer(MCP2515_SPI_INSTR_READ_RX(0, 0));
        rxOverload = false;
    }
    
    for(int i = 0; i< 13; ++i) 
        rxMessage.buffer[i] = SPI.transfer(0x00);
    stopSpi();

	if (debugEnabled)
		rxMessage.println(Serial);

    return &rxMessage;
}
