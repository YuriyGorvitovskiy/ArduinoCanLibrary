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
#include "CanConsumer.h"
#include "CanAccess.h"
#include "CanUtil.h"


CanConsumer::CanConsumer() :
	can(NULL),
	next(NULL) {
}

void CanConsumer::begin(CanAccess& aCan) {
	can = &aCan;
	can->addConsumer(this);
}

void CanConsumer::addConsumer(CanConsumer* consumer) {
	if (next != NULL)
		next->addConsumer(consumer);
	else
		next = consumer;
}

void CanConsumer::handleMessage(CanMessage& message, boolean isRTR, CanConsumer* from) {
	if (this != from) {
		if (isRTR)
			onRequest(message);
		else
			onMessage(message);
	}
	if (next != NULL)
		next->handleMessage(message, isRTR, from);
}
	
void CanConsumer::onMessage(CanMessage& message) {
}

void CanConsumer::onRequest(CanMessage& request) {
}
