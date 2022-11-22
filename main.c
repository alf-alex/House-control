/*
 * File:   main.c
 * Author: Alexis
 *
 * Created on 17 de abril de 2020, 8:55
 */

#define _XTAL_FREQ 8000000
#include <xc.h>
#include <string.h>
#include "config_pic.h"
#include "config_port.h"
#include "cmd.h"



char antiRebound(char key);
void verification(char password[]);
void printLCD(char key);



int keyPad() {
    PORTDbits.RD0 = 1;
    int space=0, hBits; //caracter para contrasenia
    char password[4], selec = 0, key=0;
    while (1) {
        hBits = PORTD >> 4;
        switch(hBits){
            case 1: key = antiRebound(selec); break;
            case 2: key = antiRebound(selec+1); break;
            case 4: key = antiRebound(selec+2); break;
            case 8: key = antiRebound(selec+3); break;
        }
        selec+=4;
        PORTD <<=1;
        if(PORTD == 0x08){
            PORTD = 0x01;
            selec=0;
        }
        
        if (key!=0) {
            printLCD(key);
            password[space]=key;
            key=0;
            space++;
            if (space == 5){
                verification(password);
                space=0;
            }
        }

    }
}

void printLCD(char key){
     //send_cmd(0x01);
     //send_cmd(0x81);
    if (key == '.') send_cmd(0x01);
    else send_char(key);
}



char antiRebound(char key) {
    char keyboard[16] = {
        '7', '8', '9', '/',
        '4', '5', '6', 'x',
        '1', '2', '3', '-',
        '.', '0', '=', '+'
    };
    while (PORTDbits.RD4 == 1) {}
    while (PORTDbits.RD5 == 1) {}
    while (PORTDbits.RD6 == 1) {}
    while (PORTDbits.RD7 == 1) {}
    PORTDbits.RD0 = 1;
    return keyboard[key];
    
}

void verification(char password[]) {
    int validation;
    char *inv = "-------", *acep = "*****", *pass = "12345";
    //se guarda el caracte en la cadena contrasenia para comparar mas adelante
    
    if ( strcmp(password,pass) == 0) {
        send_cmd(0x01);
        send_cmd(0x81);
        for(int i=0;i < 4;i++){
            send_char(acep[i]);
        }
        __delay_ms(3000);
        send_cmd(0x01);
    }
    else {
        send_cmd(0x01);
        send_cmd(0x81);
        for (int i = 0; i < 7; i++) {
            send_char(inv[i]);
        }
        __delay_ms(3000);
        send_cmd(0x01);
        //colocar acumulador que active una interrupcion a los tres intentos
        TMR0--;
    }

}
//configuracion del puerto analogico para ensender a un abanico en caso de
//exeder cierta temperatura
/*
void adq(){
    ADCON0.B1=1;
    while(ADCON0.B1==1){}
    if(ADCON0.B1==0){
        if(ADRESH>0x02 && ADRESl>0XFF){PORTE.B1=1;}
        else {
            PORTE.B3=0;
        }
    }
}
 */

/*
//configura la frecuencia del PWM para el foco (rutina no utilizada)
void cambio(){
//si se activa el bit B0 se decrementa el cilco util
    if(PORTBbits.RB0==1){
        //estan oprimidos ambos botones se ignora y sale de la funcion
        if(PORTBbits.RB1==1){
            return;
        }
        else {
            CCPR1L=CCPR1L--;
            CCPR1L=CCPR1L--;
        }
    }
    else {
        //si se activa el bit B1 se incrementa el ciclo util
        if(PORTBbits.RB1==1){ //si estan oprimido ambos botones se ignora
            if(PORTBbits.RB0==1){
                return;
            }
            //incrementa ciclo util
            else{
                CCPR1L=CCPR1L++;
                CCPR1L=CCPR1L++;
            }
       }
    }
}


//se configuran las interrupciones dependiendo si esta en modo alarma, o casa

void alarm_actived() {
    //si se activa la interrucion INT indica que se activa la alrma encaso de intruso

    char *message = "ALARMA ACTIVADA", character;
    //verifica si esta en modo alarma
    if (alarm == 1) {
        PORTCbits.RC0 = 1; //activa la salida para una vocina indicando un intruci
        send_cmd(character = 0x01); //limpia pantalla
        for (int k = 0; k < 16; k++) {
            send_char(character = message[k]);
        } //indica en el LCD que se activo la alarma
        i = 0; //limpia la variable para que en el keydo se pueda escribir desde cero
        INTCONbits.INTF = 0; //se limpia la bandera
    }
}


void bulb() {
    //activacion de focos por sensado, se apaga o prende el foco segun,
    //el sensor activado dependiendo de la interrupcion
    if (alarm == 0) {
        int bport = PORTB;
        switch (bport) {
            case 8: PORTCbits.RC3 = ~PORTCbits.RC3;
                break;
            case 16: PORTCbits.RC4 = ~PORTCbits.RC4;
                break;
            case 32: PORTCbits.RC5 = ~PORTCbits.RC5;
                break;
            case 64: PORTCbits.RC6 = ~PORTCbits.RC6;
                break;
            case 128: PORTCbits.RC7 = ~PORTCbits.RC7;
                break;
        }
    }
}
 */


void __interrupt () alarm(){
    char *esp = "-*-*-";
    //if (INTCONbits.INTF == 1) alarm_actived();
    //if (INTCONbits.RBIF == 1) bulb();
    
    //a los 3 intentos se despliega un mensaje de espera
    if (INTCONbits.T0IF=1){
        send_cmd(0x01);
        send_cmd(0x81);
        for(int i=0;i < 4;i++){
            send_char(esp[i]);
        }
        __delay_ms(3000);
        send_cmd(0x01);
        INTCONbits.T0IE=0;
    }
    INTCONbits.RBIF = 0;
    INTCONbits.INTF = 0;
}


void main(void) {

    c_port();
    //conf_int();
    //conf_adc();
    int est = 1;
    unsigned short pass;
    while (1) {
        switch (est) {
            case 1: est = keyPad();
                break;
        }

    }
    return;
}
