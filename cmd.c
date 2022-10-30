#define _XTAL_FREQ 8000000
#include "cmd.h"
#include <xc.h>

//funcion para mandar caracteres al LCD
void send_char(char character){
	PORTEbits.RE1=1;
	__delay_ms(3);
	PORTA=character;
	PORTEbits.RE2=1;
	PORTEbits.RE2=0;
}

//funcion para ordenar al LCD
void send_cmd(char character){
	PORTEbits.RE1=0;
	__delay_ms(3);
	PORTA=character;
	PORTEbits.RE2=1;
	PORTEbits.RE2=0;
}