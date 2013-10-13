/*
 *  Pulse.cpp
 *  EEPROM
 *
 *  Created by Bruno Muller on 29/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Pulse.h"

Pulse::Pulse(unsigned short int numEEPROM, unsigned short int numBit) : m_numEEPROM(numEEPROM), m_numBit(numBit) {
}

unsigned short int Pulse::getNumEEPROM() {
	return m_numEEPROM;
}

unsigned short int Pulse::getNumBit() {
	return m_numBit;
}

unsigned short int Pulse::getOutput() {
	return 1<<(m_numBit - 1);
}