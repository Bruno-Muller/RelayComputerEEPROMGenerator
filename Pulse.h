/*
 *  Pulse.h
 *  EEPROM
 *
 *  Created by Bruno Muller on 29/10/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DEF_PULSE
#define DEF_PULSE

class Pulse {
	
public:
	Pulse(unsigned short int numEEPROM, unsigned short int numBit); // Constructeur
	unsigned short int getNumEEPROM();
	unsigned short int getNumBit();
	unsigned short int getOutput();
	
private:
	unsigned short int m_numEEPROM, m_numBit;
	
};

#endif

