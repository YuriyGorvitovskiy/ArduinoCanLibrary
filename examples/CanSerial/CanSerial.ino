#include <SPI.h>
#include <Can.h>
#include <TextInput.h>

TextInput  TXT;
CanMCP2515 CAN(10);

void onMessage(CanMessage& msg) {
    msg.println(Serial);
}

void onTextInput(const char* txt) {
    Serial.print(txt);
    Serial.println(CAN.post(*CAN.txtMessage(txt)) ? " - OK" : " - FAILED" );
}

void setup() {
    delay(100);
    Serial.begin(115200);
    
    CAN.begin(Can125kbps, CanFilterNone, CanModeNormal);
    CAN.subscribe(onMessage, onMessage);
    
    TXT.begin(Serial, 50);
    TXT.subscribe(onTextInput);
    
    Serial.println("Started");
}

void loop() {
    CAN.loop();
    TXT.loop();
}
