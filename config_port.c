#include "config_port.h"
#include "cmd.h"
#include <xc.h>

//se configuran las interrupciones

/*void conf_int() {
    INTCONbits.INTE = 1;
    INTCONbits.INTF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1;
    IOCB = 0XF8;
}

//configuracion del pueto analogico

void conf_adc() {
    ADCON1 = 0X80;
    ADCON0 = 0X69;
}

//configuracion del PWM

/*void c_pwm(){
    T2CON=0X00;
    PR2=0X64;
///////////
    CCP2CON=0X0F;
    CCPR2L=0XC8;
    T2CONbits.TMR2ON=1;
}
 */
void c_port() {
    char character;
    ANSEL = 0x01;
    ANSELH = 0;
    TRISE = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB1 = 1;
    TRISD = 0XF0; //configura el puero para teclado
    TRISA = 0;
    TRISC = 0;
    PORTD = 0;
    PORTA = 0;
    PORTC = 0;
    PORTB = 0;
    PORTE = 0;

    send_cmd(character = 0X38);

    send_cmd(character = 0X0C);

    send_cmd(character = 0X06);

    send_cmd(character = 0X01); // limpia pantalla del LCD
}
