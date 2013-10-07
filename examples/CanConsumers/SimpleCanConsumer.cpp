#include <Arduino.h>
#include "SimpleCanConsumer.h"

SimpleCanConsumer::SimpleCanConsumer(int anId) :
	id(anId) {
}

boolean SimpleCanConsumer::postMessage(const char* txt) {
	return post(*can->txtMessage(txt));
}

void SimpleCanConsumer::onMessage(CanMessage& message) {
	Serial.print(id);
	Serial.print('-');
	message.println(Serial);
}

void SimpleCanConsumer::onRequest(CanMessage& message) {
	Serial.print(id);
	Serial.print('-');
	message.println(Serial);
}

