#include <SPI.h>
#include <Can.h>
#include <TextInput.h>
#include "SimpleCanConsumer.h"

TextInput           TXT;
CanMCP2515          CAN(10);
SimpleCanConsumer 	Consumer1(1);
SimpleCanConsumer 	Consumer2(2);

void onMessage(CanMessage& msg) {
    Serial.print("0-");
    msg.println(Serial);
}

void onTextInput(const char* txt) {
	switch(txt[0]) {
		case '0': 
			Serial.print("0>");
			Serial.println(txt+2);
			Serial.println(Consumer1.postMessage(txt+2) ? "OK" : "FAILED"); 
			break;
		case '1': 
			Serial.print("1>");
			Serial.println(txt+2);
			Serial.println(Consumer1.postMessage(txt+2) ? "OK" : "FAILED"); 
			break;
		case '2': 
			Serial.print("2>");
			Serial.println(txt+2);
			Serial.println(Consumer2.postMessage(txt+2) ? "OK" : "FAILED"); 
			break;
		default:
			Serial.print("0>");
			Serial.println(txt);
			Serial.println(CAN.post(*CAN.txtMessage(txt)) ? "OK" : "FAILED"); 
			break;
	}
}

void setup() {
    delay(100);
    Serial.begin(115200);
    
    CAN.begin(Can125kbps);
    CAN.subscribe(onMessage);
    Consumer1.begin(CAN);
    Consumer2.begin(CAN);
        
    TXT.begin(Serial, 50);
    TXT.subscribe(onTextInput);
    
    Serial.println("Started");
}

void loop() {
    CAN.loop();
    TXT.loop();
}
