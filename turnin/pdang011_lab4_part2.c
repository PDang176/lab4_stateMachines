/*	Author: Patrick Dang
 *      Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #4  Exercise #2
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
unsigned char counter;

void Tick(){
	//Transitions
	switch(state){
		case Start:
			PORTC = counter;
			state = Wait_Press;
			break;
		case Wait_Press:
			if((PINA & 0x01) && (PINA & 0x02)){
				state = Wait_Release;
				counter = 0;
				PORTC = counter;
			}
			else if(PINA & 0x01){
				state = Wait_Release;
				counter = (counter < 9) ? counter + 1 : counter;
				PORTC = counter;
			}
			else if(PINA & 0x02){
				state = Wait_Release;
				counter = (counter > 0) ? counter - 1 : counter;
				PORTC = counter;
			}
			else{
				state = Wait_Press;
			}
			break;
		case Wait_Release:
			if((PINA & 0x01) && (PINA & 0x02)){
				state = Wait_Release;
				counter = 0;
				PORTC = counter;
			}
			else if(PINA == 0x00){
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
	DDRC = 0xFF; PORTC = 0x00;

	state = Start;
	counter = 0x07;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
