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
#include "MCP2515Driver.h"

MCP2515Driver::MCP2515Driver(byte csPin) {
	spiCSpin = csPin;
}

void MCP2515Driver::begin() {
 	SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    pinMode(spiCSpin, OUTPUT);
}

byte MCP2515Driver::readByte(byte address) {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_READ);
    SPI.transfer(address);
    byte result = SPI.transfer(0x00);
    stopSpi();
    return result;
}

void MCP2515Driver::writeByte(byte address, byte value) {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_WRITE);
    SPI.transfer(address);
    SPI.transfer(value);
    stopSpi();
}

void MCP2515Driver::writeBytes(byte address, byte value0, byte value1, byte value2, byte value3) {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_WRITE);
    SPI.transfer(address);
    SPI.transfer(value0);
    SPI.transfer(value1);
    SPI.transfer(value2);
    SPI.transfer(value3);
    stopSpi();
}

void MCP2515Driver::modifyByte(byte address, byte mask, byte value) {
 	startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_MODIFY);
    SPI.transfer(address);
    SPI.transfer(mask);
    SPI.transfer(value);
    stopSpi();
}

void MCP2515Driver::reset() {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_RESET);
    stopSpi();
}

void MCP2515Driver::switchMode(byte mode) {
    byte stat = (readByte(MCP2515_ADDR_CANSTAT) & MCP2515_CANSTAT_OPMOD_MASK);
    int cnt = 0;
    while(stat != mode) {
        if (++cnt % 10 == 0) {
            modifyByte( MCP2515_ADDR_CANCTRL
                      , MCP2515_CANCTRL_REQOP_MASK
                      , mode
            );
        }
        delay(100);
        stat = (readByte(MCP2515_ADDR_CANSTAT) & MCP2515_CANSTAT_OPMOD_MASK);
    }
}

void MCP2515Driver::setup10kbps() {
	//Tq = 1/200,000
	//Tb = 1/ 10,000 
	//Total Quantums = 20
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(2) 
			 | MCP2515_CNF1_BRP(39)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(4)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(7)
    );
}

void MCP2515Driver::setup20kbps() {
	//Tq = 1/400,000
	//Tb = 1/ 20,000 
	//Total Quantums = 20
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(2) 
			 | MCP2515_CNF1_BRP(19)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(4)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(7)
    );
}

void MCP2515Driver::setup50kbps() {
	//Tq = 1/1,000,000
	//Tb = 1/   50,000 
	//Total Quantums = 20
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(2) 
			 | MCP2515_CNF1_BRP(7)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(4)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(7)
    );
}

void MCP2515Driver::setup100kbps() {
	//Tq = 1/2,000,000
	//Tb = 1/  100,000 
	//Total Quantums = 20
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(2) 
			 | MCP2515_CNF1_BRP(3)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(4)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(7)
    );
}

void MCP2515Driver::setup125kbps() {
	//Tq = 1/2,000,000
	//Tb = 1/  125,000 
	//Total Quantums = 16
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(1) 
			 | MCP2515_CNF1_BRP(3)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(3)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(5)
    );
}

void MCP2515Driver::setup250kbps() {
	//Tq = 1/4,000,000
	//Tb = 1/  250,000 
	//Total Quantums = 16
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(1) 
			 | MCP2515_CNF1_BRP(1)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(3)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(5)
    );
}

void MCP2515Driver::setup500kbps() {
	//Tq = 1/8,000,000
	//Tb = 1/  500,000 
	//Total Quantums = 16
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(1) 
			 | MCP2515_CNF1_BRP(0)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(7)
             | MCP2515_CNF2_PRSEG(3)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(5)
    );
}

void MCP2515Driver::setup1Mbps() {
	//Tq = 1/8,000,000
	//Tb = 1/1,000,000 
	//Total Quantums = 8
    writeByte( MCP2515_ADDR_CNF1
             , MCP2515_CNF1_SJW(1) 
			 | MCP2515_CNF1_BRP(0)
    );
    writeByte( MCP2515_ADDR_CNF2
             , MCP2515_CNF2_BTLMODE_BY_CNF3 
             | MCP2515_CNF2_SAM_1TIME
             | MCP2515_CNF2_PHSEG1(3)
             | MCP2515_CNF2_PRSEG(2)
    );
    writeByte( MCP2515_ADDR_CNF3
             , MCP2515_CNF3_CLKOUT_PIN_CLOCKOUT 
			 | MCP2515_CNF3_WAKEUP_OFF
			 | MCP2515_CNF3_PHSEG2(2)
    );
}

void MCP2515Driver::setupNoFilters() {
    modifyByte( MCP2515_ADDR_RXB0CTRL
    		  , MCP2515_RXB0CTRL_RXM_MASK | MCP2515_RXB0CTRL_BUKT_MASK
    		  , MCP2515_RXB0CTRL_RXM_ALL  | MCP2515_RXB0CTRL_BUKT_ON
    );

    modifyByte( MCP2515_ADDR_RXB1CTRL
              , MCP2515_RXB1CTRL_RXM_MASK
              , MCP2515_RXB1CTRL_RXM_ALL
    );
}

void MCP2515Driver::setupOlcbFilters() {
    modifyByte( MCP2515_ADDR_RXB0CTRL
    		  , MCP2515_RXB0CTRL_RXM_MASK 		 | MCP2515_RXB0CTRL_BUKT_MASK
    		  , MCP2515_RXB0CTRL_RXM_EXT_FILTER  | MCP2515_RXB0CTRL_BUKT_ON
    );

    modifyByte( MCP2515_ADDR_RXB1CTRL
              , MCP2515_RXB1CTRL_RXM_MASK
              , MCP2515_RXB1CTRL_RXM_EXT_FILTER
    );
    //Mask:   0x80,0x00,0x00,0x00
    writeBytes( MCP2515_ADDR_RXM0, 0x80,0x00,0x00,0x00);
    writeBytes( MCP2515_ADDR_RXM1, 0x80,0x00,0x00,0x00);    		

    //Filter: 0x80,0x08,0x00,0x00
    writeBytes( MCP2515_ADDR_RXF0, 0x80,0x08,0x00,0x00);
    writeBytes( MCP2515_ADDR_RXF1, 0x80,0x08,0x00,0x00);    		
    writeBytes( MCP2515_ADDR_RXF2, 0x80,0x08,0x00,0x00);
    writeBytes( MCP2515_ADDR_RXF3, 0x80,0x08,0x00,0x00);
    writeBytes( MCP2515_ADDR_RXF4, 0x80,0x08,0x00,0x00);
    writeBytes( MCP2515_ADDR_RXF5, 0x80,0x08,0x00,0x00);        
}


byte MCP2515Driver::readStatus() {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_STATUS);
    byte result = SPI.transfer(0x00);
    stopSpi();
    return result;
}

byte MCP2515Driver::readRxStatus() {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_RX_STATUS);
    byte result = SPI.transfer(0x00);
    stopSpi();
    return result;
}

void MCP2515Driver::setTxPriority(byte addr, byte priority) {
    modifyByte( addr
	  		  , MCP2515_TXBCTRL_TXP
			  , priority
    );
}

void MCP2515Driver::writeTxBuffer(byte buffer, byte priority, byte* data) {
    startSpi();
    SPI.transfer(MCP2515_SPI_INSTR_WRITE_TX(buffer, 0));
    for(int i = 0; i< 13; ++i) 
       SPI.transfer(data[i]);
    stopSpi();

	byte addr = (buffer == 0) ? MCP2515_ADDR_TXB0CTRL : (buffer == 1) ? MCP2515_ADDR_TXB1CTRL : MCP2515_ADDR_TXB2CTRL; 
    modifyByte( addr
	  		  , MCP2515_TXBCTRL_TXREQ | MCP2515_TXBCTRL_TXP
			  , MCP2515_TXBCTRL_TXREQ | priority
    );
}
