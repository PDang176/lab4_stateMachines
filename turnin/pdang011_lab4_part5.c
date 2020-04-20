/*	Author: Patrick Dang
 *      Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait_Press, Wait_Release} state;

unsigned char checkPXYX;
unsigned char doorOpen;

void Tick(){
	//Transitions
	switch(state){
		case Start:
			state = Wait_Press;
			PORTB = doorOpen;
			break;
		case Wait_Press:
			if(PINA & 0x80){ // Checks if PA7 (door lock button) is pressed
				state = Wait_Release;
				checkPXYX = 0x00;
				doorOpen = 0x00;
				PORTB = doorOpen;
			}
			else if((PINA & 0x04) && ((PINA & 0xFB) == 0x00)){ // Checks if PA2 ('#' button) is pressed and is the only one pressed
				state = Wait_Release;
				checkPXYX = 0x01;
			}
			else if((PINA & 0x01) && ((PINA & 0xFE) == 0x00) && (checkPXYX & 0x01)){ // Checks if PA0 ('X' button) is pressed and is the only one pressed right after '#' was pressed
				state = Wait_Release;
				checkPXYX = 0x02;
			}
			else if((PINA & 0x02) && ((PINA & 0xFD) == 0x00) && (checkPXYX & 0x02)){ // Checks if PA1 ('Y' button) is pressed and is the only one pressed right after "#X" was pressed
				state = Wait_Release;
				checkPXYX = 0x04;
			}
			else if((PINA & 0x01) && ((PINA & 0xFE) == 0x00) && (checkPXYX & 0x04)){ // Checks if PA0 ('X' Button) is pressed and is the only one pressed right after "#XY" was pressed
				state = Wait_Release;
				checkPXYX = 0x00;
				if(doorOpen){
					doorOpen = 0x00;
					PORTB = doorOpen;
				}
				else{
					doorOpen = 0x01;
					PORTB = doorOpen;
				}
			}
			else if(PINA){ // Checks if any other button combination was pressed
				checkPXYX = 0x00;
				state = Wait_Release;
			}
			else{
				state = Wait_Press;
			}
			break;
		case Wait_Release:
			if(PINA & 0x80){ // Checks if PA7 (door lock button) is pressed
                                state = Wait_Release;
                                checkPXYX = 0x00;
                                doorOpen = 0x00;
                                PORTB = doorOpen;
                        }
			else if((PINA & 0x04) && ((PINA & 0xFB) == 0x00)){ // Checks if PA2 ('#' button) is pressed and is the only one pressed
                                state = Wait_Release;
                                checkPXYX = 0x01;
                        }
			else if((checkPXYX & 0x01) && (PINA & 0xFB)){ // Checks if a button other than the currently needed button in the combination was pressed (Currently '#')
				state = Wait_Release;
				checkPXYX = 0x00;
			}
			else if((checkPXYX & 0x02) && (PINA & 0xFE)){ // Currently 'X'
				state = Wait_Release;
				checkPXYX = 0x00;
			}
			else if((checkPXYX & 0x04) && (PINA & 0xFD)){ // Currently 'Y'
				state = Wait_Release;
				checkPXYX = 0x00;
			}
			else if((PINA & 0xFF) == 0x00){ // Checks if all buttons were released
                                state = Wait_Press;
                        }
			else{
				state = Wait_Release;
			}
			break;
		default:
			state = Start;
			break;

	}

	//State Actions
	switch(state){
		case Start:
		case Wait_Press:
		case Wait_Release:
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	state = Start;
	checkPXYX = 0x00;
	doorOpen = 0x00;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
