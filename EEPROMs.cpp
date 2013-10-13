/*
 *  EEPROMs.cpp
 *  EEPROM
 *
 *  Created by Bruno Muller on 29/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EEPROMs.h"

EEPROMs::EEPROMs() {
	for (unsigned short int numEEPROM(0); numEEPROM<5; numEEPROM++) {
		for (unsigned int adresse(0); adresse<65536; adresse++) {
			m_eeprom[numEEPROM][adresse] = 0x00;
		}
	}
}

void EEPROMs::addPulse(unsigned short int adresse, Pulse *pulse) {
	m_eeprom[pulse->getNumEEPROM()][adresse] |= pulse->getOutput();
}

unsigned short int EEPROMs::getWord(unsigned short int numEEPROM, unsigned short int adresse) {
	return m_eeprom[numEEPROM][adresse];
}