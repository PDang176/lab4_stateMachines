/*	Author: Patrick Dang
 *      Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #4  Exercise #1
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

enum States{Start, Wait_Press1, Wait_Release1, Wait_Press2, Wait_Release2} state;

void Tick(){
	//Transitions
	switch(state){
		case Start:
			PORTB = 0x01;
			state = Wait_Press1;
			break;
		case Wait_Press1:
			if(PINA & 0x01){
				state = Wait_Release1;
				PORTB = 0x02;
			}
			else{
				state = Wait_Press1;
			}
			break;
		case Wait_Release1:
			state = (PINA & 0x01) ? Wait_Release1 : Wait_Press2;
			break;
		case Wait_Press2:
			if(PINA & 0x01){
				state = Wait_Release2;
				PORTB = 0x01;
			}
			else{
				state = Wait_Press2;
			}
			break;
		case Wait_Release2:
			state = (PINA & 0x01) ? Wait_Release2 : Wait_Press1;
			break;
		default:
			state = Start;
			break;
	}

	//State Actions
	switch(state){
		case Start:
                case Wait_Press1:
                case Wait_Release1:
		case Wait_Press2:
		case Wait_Release2:
                default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	state = Start;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
