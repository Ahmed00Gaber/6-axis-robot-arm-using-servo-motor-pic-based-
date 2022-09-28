/*
 * File:   main.c
 * Author: ambad
 *
 * Created on May 5, 2021, 7:51 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
// 16Mhz oscilator
#define _XTAL_FREQ 48000000

uint8_t UART_data_counter = 0;
bool start = 0;
bool startCount = 0;
uint8_t grandCount = 0;

#define axes 6
int timer_counter = 0;
int received[axes]= {180,180,180,0};// angels given for test
int order[axes] = {1, 2, 4, 8, 16, 32};// for inverse kinematics

void main(void) {
    
    //choosing CPU clock source
    SCS1 = 0;  // Primary Source
    SCS0 = 0;
    
    //TRIS
    TRISB = 0;
    TRISC6 = 1;
    TRISC7 = 1;
    TRISC0 = 0;  // indicator1
    TRISC1 = 0;  // indicator2
    RC0 = 0;
    RC1 = 0;
    
    //Interrupts
    GIE = 1;
    PEIE = 1;
    TMR1IE = 1;
    TMR3IE = 1;
//    RCIE = 1;
    
    //Comparator module
    CM2 = 1;
    CM1 = 1;
    CM0 = 1;
    
    //ADC module
    ADON = 0;
    PCFG3 = 1;  // all pins are digital
    PCFG2 = 1;
    PCFG1 = 1;
    PCFG0 = 1;
    
    //TMR1
    TMR1ON = 1;
    TMR1CS = 0;
    T1OSCEN = 0;
    
    //TMR3
//    RD163 = 0;
    TMR3ON = 1;
    TMR3CS = 0;
    T3CKPS1 = 1;  // prescalar value = 4
    T3CKPS0 = 0;
    
    //UART Receiver Simplex Mode
//    SPEN = 1;
    RX9 = 0;
    CREN = 1;
    SYNC = 0;
    BRG16 = 0;
    BRGH = 1;
    RXDTP = 0;
    SPBRG = 312;
    
    //Main Routine
    while(1) {
    }
    
    return;
}

void __interrupt() ISR(void) {
    
    if(TMR1IF) {
        if (startCount == 0) {
            startCount = 1;
            TMR1 = 65480;  // the period to make 1 degree increment
        } else if (startCount == 1){
//            RC1 = 1;  // indicator on
            if (timer_counter == received[0]) {
                PORTB = PORTB & ~order[0];// signal low when reaching your value 
            }
            if (timer_counter == received[1]) {
                PORTB = PORTB & ~order[1];
            }
            if (timer_counter == received[2]) {
                PORTB = PORTB & ~order[2];
            }
            if (timer_counter == received[3]) {
                PORTB = PORTB & ~order[3];
            }
            if (timer_counter == received[4]) {
                PORTB = PORTB & ~order[4];
            }
            if (timer_counter == received[5]) {
                PORTB = PORTB & ~order[5];
            }
//            RC1 = 0;  // indicator off
            timer_counter++;
                TMR1 = 65480;  // the period to make 1 degree increment
        }
        if(timer_counter == 181) {
            TMR1ON = 0;
            timer_counter = 0;
            startCount = 0;
            SPEN = 1;
            RCIE = 1;
            TMR1 = 57300; // for not start period of 0.5ms, the time to send 0 degree
            
            grandCount++;
            if(grandCount == axes) {
                grandCount = 0;
            }
        }
        TMR1IF = 0;
    }
    
    if(TMR3IF) {
        PORTB = 0b11111111;
        TMR1ON = 1;
        TMR3 = 5535;
        TMR3IF = 0;
    }
    
//    if(RCIF) {
//        received[UART_data_counter] = RCREG;
//        if(start) {
//            UART_data_counter++;
//            if (UART_data_counter == axes) {
//                RC0 = 0;
//                UART_data_counter = 0;
//                start = 0;
//                RCIE = 0;
//                SPEN = 0;
//            }
//        }
//        if(received[0] == 0) {
//            RC0 = 1;
//            start = 1;
//        }
//        RCIF = 0;
//    }
}

