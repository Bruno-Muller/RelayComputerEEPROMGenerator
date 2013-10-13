/*
 *  EEPROMs.h
 *  EEPROM
 *
 *  Created by Bruno Muller on 29/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DEF_EEPROMs
#define DEF_EEPROMs

#include "Pulse.h"

class EEPROMs {
	
public:
	EEPROMs(); // Constructeur
	void addPulse(unsigned short int adresse, Pulse *pulse);
	void printAdresse(unsigned short int adresse);
	unsigned short int getWord(unsigned short int numEEPROM, unsigned short int adresse);

private:
	unsigned short int m_eeprom[5][65536];
	
};

#endif