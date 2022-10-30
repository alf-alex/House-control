/*
 * File:   main.c
 * Author: Alexis
 *
 * Created on 17 de abril de 2020, 8:55
 */

#define _XTAL_FREQ 8000000
#include <xc.h>
#include "config_pic.h"
#include "config_port.h"
#include "cmd.h"


char antiRebound(char key);
void verification(char password[]);



/*hacer que funcione solo con lo basico, osea contraseña y alarma, en programas
 aparte realizar las demas funciones y simularlas para asegurar su uso para
 despues agregar al programar completo, funcones complementarias: PWM pára el
 foco, un protocolo de comunicacion para blootho o algo mas, regustrar quien
 esta en casa permedio de sensores (ver si se pueden mejorar), configuracion de
 entrad analogica para temperatura*/

//arreglar problema de funcion verificacion, su tipo para que pueda aceptar cadenas completas



//funcion que realiza la lectura del keydo matricial

/*arreglar funcion pasword y tipo de variable key ya que no enjeca en funcion,
 en switch guardar primero el caracter solo, luego en cadena completa para seuir*/
int inpassword() {
    int i = 0; //???
    PORTDbits.RD0 = 1;
    char in, key = 0; //caracter para contrasenia
    char password[4] = "empty"; //cadena para contrasenia
    char hBits;
    while (1) {
        while (key < 15) {
            if (PORTD == 0x08) {
                PORTD = 0x01;
            }
            hBits = PORTD << 0x10;
            switch (hBits) {
                case 16: in = antiRebound(key);
                    key++;
                    break;

                case 32: in = antiRebound(key);
                    key++;
                    break;

                case 64: in = antiRebound(key);
                    key++;
                    break;

                case 128: in = antiRebound(key);
                    key++;
                    break;
            }
            //STATUSbits.C = 0;
            PORTD <<= 1;
            //password[i] += in;
            
            if (key > 16) {
                key = 0;
            }
        }
        if (i == 4) {
            verification(password);
            return 1;
        }
        key = 0;
    }

}

//funcion de antirebote, para evitar fallo en el boton y llama, a las funciones
//de activacion de la contrasenia o rutinas de la casa, segun ese esepecificaco

char antiRebound(char key) {
    char keyboard[16] = {
        '1', '2', '3', 'A',
        '4', '5', '6', 'B',
        '7', '8', '9', 'C',
        '*', '0', '#', 'D'
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
    char *inv = "INVALIDO", *acep = "ACEPTADO", *esp = "ESPERE 5 SEGUNDOS", *pass = "ABCD0";
    //se guarda el caracte en la cadena contrasenia para comparar mas adelante
    for (int i = 0; i < 4; i++) {
        if (password[i] == pass[i]) {
            validation++;
        }
    }

    //si es correcta despliega un mensaje indicandolo y
    //sale del modo alarma
    if (validation == 4) {
        send_cmd(0x01);
        send_cmd(0x83);
        for (int i = 0; i < 7; i++) {
            send_char(acep[i]);
        }
        __delay_ms(3000);
        //
        send_cmd(0x01);
        send_cmd(0x01);
        PORTCbits.RC0 = 0;
        //
    }//sino es correcta se despliega un mensaje indicandolo
    else {
        send_cmd(0x01);
        send_cmd(0x81);
        for (int i = 0; i < 7; i++) {
            send_char(inv[i]);
        }
        __delay_ms(1000);
        send_cmd(0x01);
        //colocar acumulador que active una interrupcion a los tres intentos

    }

    //a los 3 intentos se despliega un mensaje de espera
    /*if (var2 == 3) {
        send_cmd(pass = 0x01);
        for (i = 0; i < 9; i++) {
            send_char(pass = esp[i]);
        }
        __delay_ms(5000);
        send_cmd(pass = 0x01);
        var2 = 0;
    }*/
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

/*
void interrupt() {

    if (INTCONbits.INTF == 1) alarm_actived();
    if (INTCONbits.RBIF == 1) bulb();

    INTCONbits.RBIF = 0;
    INTCONbits.INTF = 0;
}
 */

void main(void) {

    c_port();
    //conf_int();
    //conf_adc();
    int est = 1;
    unsigned short pass;
    while (1) {
        switch (est) {
            case 1: est = inpassword();
                break;
        }

    }
    return;
}
