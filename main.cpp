#include <iostream>
#include <fstream>

#include "Pulse.h"
#include "EEPROMS.h"

// EEPROM1
Pulse A_LOAD(0,1);
Pulse A_SELECT(0,2);	
Pulse B_LOAD(0,3);		
Pulse B_SELECT(0,4);	
Pulse C_LOAD(0,5);		
Pulse C_SELECT(0,6);	
Pulse D_LOAD(0,7);		
Pulse D_SELECT(0,8);

// EEPROM2
Pulse M1_LOAD(1,1);		
Pulse M1_SELECT(1,2);
Pulse M2_LOAD(1,3);
Pulse M2_SELECT(1,4);
Pulse M_SELECT(1,5);
Pulse X_LOAD(1,6);
Pulse X_SELECT(1,7);
Pulse Y_LOAD(1,8);

// EEPROM3
Pulse Y_SELECT(2,1);
Pulse XY_LOAD(2,2);
Pulse XY_SELECT(2,3);
Pulse J1_LOAD(2,4);
Pulse J2_LOAD(2,5);
Pulse J_SELECT(2,6);
Pulse INST_LOAD(2,7);
Pulse PC_LOAD(2,8);

// EEPROM4
Pulse PC_SELECT(3,1);
Pulse INC_LOAD(3,2);
Pulse INC_SELECT(3,3);
Pulse MEMORY_READ(3,4);
Pulse MEMORY_WRITE(3,5);
Pulse ALU_LOAD_FLAGS(3,6);
Pulse ALU_SETCODE0(3,7);
Pulse ALU_SETCODE1(3,8);

// EEPROM5
Pulse ALU_SETCODE2(4,1);
Pulse HALT(4,2);
Pulse RESETCLOCK(4,3);
Pulse LOAD_IMMEDIATE_TO_BUS(4,4);

EEPROMs eeproms;

void genererEEPROMs();

int main (int argc, char * const argv[]) {
    std::cout << "Code EEPROM\n";
	
	unsigned short int adresse(0), instruction(0), cycle(0);
	
	// ALU ~
	// 1000rfff	
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int destination(0); destination<2; destination++) {
			for (unsigned short int fonction(0); fonction < 8; fonction++) {
				
				instruction = 0x80 + fonction + (destination<<3) + (conditionCode<<13);

				// cycle 1 : PC_SELECT, MEMORY_READ
				cycle = 0;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);			

				// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				eeproms.addPulse(adresse, &INST_LOAD);
				eeproms.addPulse(adresse, &INC_LOAD);
				
				// cycle 3 : PC_SELECT, MEMORY_READ
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);		
				
				// cycle 4 : fonction
				cycle++;
				adresse = instruction + (cycle<<8);
				if (fonction & 0x01) eeproms.addPulse(adresse, &ALU_SETCODE0);
				if (fonction & 0x02) eeproms.addPulse(adresse, &ALU_SETCODE1);
				if (fonction & 0x04) eeproms.addPulse(adresse, &ALU_SETCODE2);
				
				// cycle 5 : INC_SELECT, PC_LOAD, ALU_LOAD_FLAGS, fonction, destination
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &INC_SELECT);
				eeproms.addPulse(adresse, &PC_LOAD);
				eeproms.addPulse(adresse, &ALU_LOAD_FLAGS);	
				if (fonction & 0x01) eeproms.addPulse(adresse, &ALU_SETCODE0);
				if (fonction & 0x02) eeproms.addPulse(adresse, &ALU_SETCODE1);
				if (fonction & 0x04) eeproms.addPulse(adresse, &ALU_SETCODE2);
				if (destination == 0) eeproms.addPulse(adresse, &A_LOAD);
				if (destination == 1) eeproms.addPulse(adresse, &D_LOAD);
				
				// cycle 6 : INC_SELECT, fonction
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &INC_SELECT);
				if (fonction & 0x01) eeproms.addPulse(adresse, &ALU_SETCODE0);
				if (fonction & 0x02) eeproms.addPulse(adresse, &ALU_SETCODE1);
				if (fonction & 0x04) eeproms.addPulse(adresse, &ALU_SETCODE2);
				
				// cycle 7 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				
				// cycle 8 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &RESETCLOCK);
			}
		}	
	}
	
	// MOV-8 ~
	// 00dddsss
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int destination(0); destination < 8; destination++) {
			for (unsigned short int source(0); source < 8; source++) {
				
				instruction = 0x00 + source + (destination<<3) + (conditionCode<<13);
				
				// cycle 1 : PC_SELECT, MEMORY_READ
				cycle = 0;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				
				// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				eeproms.addPulse(adresse, &INST_LOAD);
				eeproms.addPulse(adresse, &INC_LOAD);
				
				// cycle 3 : PC_SELECT, MEMORY_READ
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				
				// cycle 4 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				
				// cycle 5 : INC_SELECT, PC_LOAD, source_select, destination_load
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &INC_SELECT);
				eeproms.addPulse(adresse, &PC_LOAD);
				switch (destination) {
					case 0:
						eeproms.addPulse(adresse, &A_LOAD);
						break;
					case 1:
						eeproms.addPulse(adresse, &B_LOAD);
						break;
					case 2:
						eeproms.addPulse(adresse, &C_LOAD);
						break;
					case 3:
						eeproms.addPulse(adresse, &D_LOAD);
						break;
					case 4:
						eeproms.addPulse(adresse, &M1_LOAD);
						break;
					case 5:
						eeproms.addPulse(adresse, &M2_LOAD);
						break;
					case 6:
						eeproms.addPulse(adresse, &X_LOAD);
						break;
					case 7:
						eeproms.addPulse(adresse, &Y_LOAD);
						break;
					default:
						break;
				}
				switch (source) {
					case 0:
						eeproms.addPulse(adresse, &A_SELECT);
						break;
					case 1:
						eeproms.addPulse(adresse, &B_SELECT);
						break;
					case 2:
						eeproms.addPulse(adresse, &C_SELECT);
						break;
					case 3:
						eeproms.addPulse(adresse, &D_SELECT);
						break;
					case 4:
						eeproms.addPulse(adresse, &M1_SELECT);
						break;
					case 5:
						eeproms.addPulse(adresse, &M2_SELECT);
						break;
					case 6:
						eeproms.addPulse(adresse, &X_SELECT);
						break;
					case 7:
						eeproms.addPulse(adresse, &Y_SELECT);
						break;
					default:
						break;
				}
				
				// cycle 6 : INC_SELECT, source_select
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &INC_SELECT);
				switch (source) {
					case 0:
						eeproms.addPulse(adresse, &A_SELECT);
						break;
					case 1:
						eeproms.addPulse(adresse, &B_SELECT);
						break;
					case 2:
						eeproms.addPulse(adresse, &C_SELECT);
						break;
					case 3:
						eeproms.addPulse(adresse, &D_SELECT);
						break;
					case 4:
						eeproms.addPulse(adresse, &M1_SELECT);
						break;
					case 5:
						eeproms.addPulse(adresse, &M2_SELECT);
						break;
					case 6:
						eeproms.addPulse(adresse, &X_SELECT);
						break;
					case 7:
						eeproms.addPulse(adresse, &Y_SELECT);
						break;
					default:
						break;
				}
				
				// cycle 7 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				
				// cycle 8 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &RESETCLOCK);

			}
		}
	}
	
	// SETAB ~
	// 01rddddd
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int destination(0); destination < 2; destination++) {
			for (unsigned short int valeur(0); valeur < 32; valeur++) {
				
				instruction = 0x40 + valeur + (destination<<5) + (conditionCode<<13);
				
				// cycle 1 : PC_SELECT, MEMORY_READ
				cycle = 0;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);			
				
				// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				eeproms.addPulse(adresse, &INST_LOAD);
				eeproms.addPulse(adresse, &INC_LOAD);
				
				// cycle 3 : PC_SELECT, MEMORY_READ
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				
				// cycle 4 : LOAD_IMMEDIATE_TO_BUS
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				eeproms.addPulse(adresse, &LOAD_IMMEDIATE_TO_BUS);
				
				// cycle 5 : INC_SELECT, PC_LOAD, LOAD_IMMEDIATE_TO_BUS, destination_load
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &INC_SELECT);
				eeproms.addPulse(adresse, &PC_LOAD);
				eeproms.addPulse(adresse, &LOAD_IMMEDIATE_TO_BUS);
				switch (destination) {
					case 0:
						eeproms.addPulse(adresse, &A_LOAD);
						break;
					case 1:
						eeproms.addPulse(adresse, &B_LOAD);
						break;
					default:
						break;
				}
				
				// cycle 6 : INC_SELECT, LOAD_IMMEDIATE_TO_BUS
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &INC_SELECT);
				eeproms.addPulse(adresse, &LOAD_IMMEDIATE_TO_BUS);
				
				// cycle 7 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				
				// cycle 8 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &RESETCLOCK);
			}
		}
	}
	
	// INC-XY ~
	// 10110000
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		
		instruction = 0xB0 + (conditionCode<<13);
		
		// cycle 1 : PC_SELECT, MEMORY_READ
		cycle = 0;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &PC_SELECT);
		eeproms.addPulse(adresse, &MEMORY_READ);
		
		// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &PC_SELECT);
		eeproms.addPulse(adresse, &MEMORY_READ);
		eeproms.addPulse(adresse, &INST_LOAD);
		eeproms.addPulse(adresse, &INC_LOAD);
		
		// cycle 3 : PC_SELECT, MEMORY_READ
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &PC_SELECT);
		eeproms.addPulse(adresse, &MEMORY_READ);
		
		// cycle 4 : Rien
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
		
		// cycle 5 : INC_SELECT, PC_LOAD
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &INC_SELECT);
		eeproms.addPulse(adresse, &PC_LOAD);

		// cycle 6 : INC_SELECT
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &INC_SELECT);		
		
		// cycle 7 : Rien
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
		
		// cycle 8 : XY_SELECT, INC_LOAD
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &XY_SELECT);
		eeproms.addPulse(adresse, &INC_LOAD);
		
		// cycle 9 : XY_SELECT
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &XY_SELECT);
		
		// cycle 10 : Rien
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		
		// cycle 11 : INC_SELECT, XY_LOAD
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &INC_SELECT);
		eeproms.addPulse(adresse, &XY_LOAD);
		
		// cycle 12 : INC_SELECT
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &INC_SELECT);
		
		// cycle 11 : Rien
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		
		// cycle 12 : Rien
		cycle++;
		adresse = instruction + (cycle<<8);
		eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
		eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
		eeproms.addPulse(adresse, &RESETCLOCK);
	}
	
	// LOAD ~
	// 100100rr
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int destination(0); destination < 4; destination++) {
			
			instruction = 0x90 + destination + (conditionCode<<13);
			
			// cycle 1 : PC_SELECT, MEMORY_READ
			cycle = 0;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			eeproms.addPulse(adresse, &INST_LOAD);
			eeproms.addPulse(adresse, &INC_LOAD);
			
			// cycle 3 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 4 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			
			// cycle 5 : INC_SELECT, PC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			eeproms.addPulse(adresse, &PC_LOAD);
			
			// cycle 6 : INC_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);			
			
			// cycle 7 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			
			// cycle 8 : M_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 9 : M_SELECT, MEMORY_READ, destination_load
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			switch (destination) {
				case 0:
					eeproms.addPulse(adresse, &A_LOAD);
					break;
				case 1:
					eeproms.addPulse(adresse, &B_LOAD);
					break;
				case 2:
					eeproms.addPulse(adresse, &C_LOAD);
					break;
				case 3:
					eeproms.addPulse(adresse, &D_LOAD);
					break;
				default:
					break;
			}
			
			// cycle 10 : M_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 11 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 12 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &RESETCLOCK);
		}
	}
	
	// STORE ~
	// 100110rr
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int source(0); source < 4; source++) {
			
			instruction = 0x98 + source + (conditionCode<<13);
			
			// cycle 1 : PC_SELECT, MEMORY_READ
			cycle = 0;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
					
			
			// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			eeproms.addPulse(adresse, &INST_LOAD);
			eeproms.addPulse(adresse, &INC_LOAD);
			
			// cycle 3 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 4 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			
			// cycle 5 : INC_SELECT, PC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			eeproms.addPulse(adresse, &INC_SELECT);
			eeproms.addPulse(adresse, &PC_LOAD);
			
			// cycle 6 : INC_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			eeproms.addPulse(adresse, &INC_SELECT);
	
			// cycle 7 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			
			// cycle 8 : M_SELECT, source_select
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			switch (source) {
				case 0:
					eeproms.addPulse(adresse, &A_SELECT);
					break;
				case 1:
					eeproms.addPulse(adresse, &B_SELECT);
					break;
				case 2:
					eeproms.addPulse(adresse, &C_SELECT);
					break;
				case 3:
					eeproms.addPulse(adresse, &D_SELECT);
					break;
				default:
					break;
			}
			
			// cycle 9 : M_SELECT, MEMORY_WRITE, source_select
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			eeproms.addPulse(adresse, &MEMORY_WRITE);
			switch (source) {
				case 0:
					eeproms.addPulse(adresse, &A_SELECT);
					break;
				case 1:
					eeproms.addPulse(adresse, &B_SELECT);
					break;
				case 2:
					eeproms.addPulse(adresse, &C_SELECT);
					break;
				case 3:
					eeproms.addPulse(adresse, &D_SELECT);
					break;
				default:
					break;
			}
			
			// cycle 10 : M_SELECT, source_select
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &M_SELECT);
			switch (source) {
				case 0:
					eeproms.addPulse(adresse, &A_SELECT);
					break;
				case 1:
					eeproms.addPulse(adresse, &B_SELECT);
					break;
				case 2:
					eeproms.addPulse(adresse, &C_SELECT);
					break;
				case 3:
					eeproms.addPulse(adresse, &D_SELECT);
					break;
				default:
					break;
			}
			
			// cycle 11 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 12 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &RESETCLOCK);
		}
	}
	
	// MOV-16 ~
	// 1010dss0
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int destination(0); destination < 2; destination++) {
			for (unsigned short int source(0); source < 4; source++) {
				
				instruction = 0xA0 + (source<<1) + (destination<<3) + (conditionCode<<13);
				
				// cycle 1 : PC_SELECT, MEMORY_READ
				cycle = 0;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				
				// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				eeproms.addPulse(adresse, &INST_LOAD);
				eeproms.addPulse(adresse, &INC_LOAD);
				
				// cycle 3 : PC_SELECT, MEMORY_READ
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				eeproms.addPulse(adresse, &PC_SELECT);
				eeproms.addPulse(adresse, &MEMORY_READ);
				
				// cycle 4 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				
				// cycle 5 : INC_SELECT, PC_LOAD
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				eeproms.addPulse(adresse, &INC_SELECT);
				eeproms.addPulse(adresse, &PC_LOAD);
				
				// cycle 6 : INC_SELECT
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				eeproms.addPulse(adresse, &INC_SELECT);
				
				// cycle 7 : Rien
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				
				// cycle 8 : source_select, destination_load
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
				switch (source) {
					case 0:
						eeproms.addPulse(adresse, &M_SELECT);
						break;
					case 1:
						eeproms.addPulse(adresse, &XY_SELECT);
						break;
					case 2:
						eeproms.addPulse(adresse, &J_SELECT);
						break;
					case 3:
						// rien
						break;
					default:
						break;
				}
				switch (destination) {
					case 0:
						eeproms.addPulse(adresse, &XY_LOAD);
						break;
					case 1:
						eeproms.addPulse(adresse, &PC_LOAD);
						break;
					default:
						break;
				}
				
				// cycle 9 : source_select
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps				
				switch (source) {
					case 0:
						eeproms.addPulse(adresse, &M_SELECT);
						break;
					case 1:
						eeproms.addPulse(adresse, &XY_SELECT);
						break;
					case 2:
						eeproms.addPulse(adresse, &J_SELECT);
						break;
					default:
						break;
				}
				
				// cycle 10 : halt
				cycle++;
				adresse = instruction + (cycle<<8);
				eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
				eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
				if (source == 3) eeproms.addPulse(adresse, &HALT);
				eeproms.addPulse(adresse, &RESETCLOCK);
			}
		}
	}
	
	// GOTO ~
	// 11dscznx aaaaaaaa aaaaaaaa
	for (unsigned short int conditionCode(0); conditionCode<8; conditionCode++) {
		for (unsigned short int flags(0); flags<64; flags++) {
			
			// LOAD 16-bit immediate	11000000 
			// Jump						11100110
			// Call						11100111
			// Branche if Negative		11110000
			// Branch if No Carry		11101000
			// Branch if Zero			11100100
			// Branch if Not Zero		11100010
			if ((flags != 0x00) && (flags != 0x26) && (flags != 0x27) && (flags != 0x30) && (flags != 0x28) && (flags != 0x24)  && (flags != 0x22)) continue;
		
			instruction = 0xC0 + flags + (conditionCode<<13);
			
			// cycle 1 : PC_SELECT, MEMORY_READ
			cycle = 0;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 2 : PC_SELECT, MEMORY_READ, INST_LOAD, INC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			eeproms.addPulse(adresse, &INST_LOAD);
			eeproms.addPulse(adresse, &INC_LOAD);
			
			// cycle 3 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
				
			// cycle 4 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps	
			
			// cycle 5 : INC_SELECT, PC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			eeproms.addPulse(adresse, &PC_LOAD);	
			
			// cycle 6 : INC_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			
			// cycle 7 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 8 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 9 : PC_SELECT, MEMORY_READ, INC_LOAD, m1/j1_load
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			eeproms.addPulse(adresse, &INC_LOAD);
			if (instruction & 0x20) eeproms.addPulse(adresse, &J1_LOAD);
			else eeproms.addPulse(adresse, &M1_LOAD);

			// cycle 10 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 11 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 12 : INC_SELECT, PC_LOAD
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			eeproms.addPulse(adresse, &PC_LOAD);
			
			// cycle 13 : INC_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			
			// cycle 14 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 15 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 16 : PC_SELECT, MEMORY_READ, INC_LOAD,  m2/j2_load
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			eeproms.addPulse(adresse, &INC_LOAD);
			if (instruction & 0x20) eeproms.addPulse(adresse, &J2_LOAD);
			else eeproms.addPulse(adresse, &M2_LOAD);
			
			// cycle 17 : PC_SELECT, MEMORY_READ
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &PC_SELECT);
			eeproms.addPulse(adresse, &MEMORY_READ);
			
			// cycle 18 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 19 : INC_SELECT, PC_LOAD, xy_load
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			eeproms.addPulse(adresse, &PC_LOAD);
			if (instruction & 0x01) eeproms.addPulse(adresse, &XY_LOAD);
			
			// cycle 20 : INC_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &INC_SELECT);
			
			// cycle 21 : Rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			
			// cycle 22 : J_SELECT, pc_load
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &J_SELECT);
			// conditionCode :
			// Z : 0x2000
			// Cy: 0X4000
			// S : 0x8000
			// Instruction :
			// s : 0x0010
			// c : 0x0008
			// z : 0x0004
			// n : 0x0002
			if ((instruction & 0x8010) == 0x8010) eeproms.addPulse(adresse, &PC_LOAD);
			if ((instruction & 0x4008) == 0x0008) eeproms.addPulse(adresse, &PC_LOAD);
			if ((instruction & 0x2004) == 0x2004) eeproms.addPulse(adresse, &PC_LOAD);
			if ((instruction & 0x2002) == 0x0002) eeproms.addPulse(adresse, &PC_LOAD);
			
			// cycle 23 : J_SELECT
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &J_SELECT);
			
			// cycle 24 : rien
			cycle++;
			adresse = instruction + (cycle<<8);
			eeproms.addPulse(adresse, &ALU_SETCODE0); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE1); // tout le temps
			eeproms.addPulse(adresse, &ALU_SETCODE2); // tout le temps
			eeproms.addPulse(adresse, &RESETCLOCK);
		}
	}
	
	genererEEPROMs();
		
    return 0;
}


void genererEEPROMs() {
	//EEPROM1
	std::ofstream eeprom1("EEPROM1.dat", std::ios::out | std::ios::trunc);
	if (eeprom1) {
		for (unsigned int adresse(0); adresse < 65536; adresse++) {
			eeprom1.put((char) eeproms.getWord(0,adresse));
		}
		eeprom1.close();
	}
	
	//EEPROM2
	std::ofstream eeprom2("EEPROM2.dat", std::ios::out | std::ios::trunc);
	if (eeprom2) {
		for (unsigned int adresse(0); adresse < 65536; adresse++) {
			eeprom2.put((char) eeproms.getWord(1,adresse));
		}
		eeprom2.close();
	}
	
	//EEPROM3
	std::ofstream eeprom3("EEPROM3.dat", std::ios::out | std::ios::trunc);
	if (eeprom3) {
		for (unsigned int adresse(0); adresse < 65536; adresse++) {
			eeprom3.put((char) eeproms.getWord(2,adresse));
		}
		eeprom3.close();
	}
	
	//EEPROM4
	std::ofstream eeprom4("EEPROM4.dat", std::ios::out | std::ios::trunc);
	if (eeprom4) {
		for (unsigned int adresse(0); adresse < 65536; adresse++) {
			eeprom4.put((char) eeproms.getWord(3,adresse));
		}
		eeprom4.close();
	}
	
	//EEPROM5
	std::ofstream eeprom5("EEPROM5.dat", std::ios::out | std::ios::trunc);
	if (eeprom5) {
		for (unsigned int adresse(0); adresse < 65536; adresse++) {
			eeprom5.put((char) eeproms.getWord(4,adresse));
		}
		eeprom5.close();
	}
}


