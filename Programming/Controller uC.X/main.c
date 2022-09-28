/*
 * File:   main.c
 * Author: ahmed gaber
 *
 * Created on May 3, 2021, 4:59 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#define _XTAL_FREQ 48000000

int pot;
bool start = 0;
// pwm generation
void PWM1(int duty_cycle) {
    
    DC1B1 = (duty_cycle) & 1;
    DC1B0 = (duty_cycle) & 2;
    CCPR1L = (duty_cycle) >> 2;
    
    return;
}
// ADC read
int readADC(int pin) {
    ADCON0 = pin << 2;
    ADON = 1;
    GO = 1;  // Aquisition is done automatically, so starting conversion
    while(GO);  // waiting till conversion is done
    return (ADRESH << 8) + ADRESL;
}
// send uart
void sendUART(int value) {
    while(!TRMT) {
        // wait until TRMT bit is 0
    }
    TXREG = value;
}

void main(void) {
    
    //choosing CPU clock source
    SCS1 = 0;  // Primary Source
    SCS0 = 0;
    
    //Interrupts
    GIE = 1;
    PEIE = 1;
    TMR1IE = 0;
    
    //PORTA
    TRISA = 0b11111111;
    CM2 = 1;
    CM1 = 1;
    CM0 = 1;
    
    //PORTC set up
    TRISC2 = 0;  // CCP1 PWM output
    TRISC6 = 1;
    TRISC7 = 1;
    
    //PORTD set up
    TRISD1 = 0;
    LATD1 = 0;
    TRISD2 = 1;
    SSPEN = 0;  // disabling SSP module
    
    //TMR1
    TMR1ON = 0;
    TMR1CS = 0;
    T1OSCEN = 0;
    
    //TMR2
    TMR2ON = 0;
    T2CKPS1 = 0;
    T2CKPS0 = 1; // prescalar to 4
    
    //CCP1
    CCP1M3 = 1;
    CCP1M2 = 1;
    CCP1M1 = 0;
    CCP1M0 = 0;
    PR2 = 149; // 20kHz
    
    //ADC
    PCFG3 = 0; 
    PCFG2 = 1;
    PCFG1 = 1;
    PCFG0 = 1;  // pins RA0 to RA7 are all analogue pins
//    PBADEN = 0;  // it is there in the datasheet i don't know why it's not working????!!!
    VCFG0 = 0;  // set Vref- to Vss
    VCFG1 = 0;  // set Vref+ to Vdd
    ADON = 1;  //turning on ADC module
    ADFM = 1; // right justification
    ACQT2 = 0;
    ACQT1 = 1;
    ACQT0 = 0;  // Tacq = 4 * Tad = 5.2us > 2.45us which is min in datasheet
    ADCS2 = 1;
    ADCS1 = 1;
    ADCS0 = 0;  // Tad = 64 * Tosc = 6 * (48*10^-6)^-1 = 1.3us which is > 0.7us which is Min in datasheet
    
    // UART Transmitter simplex mode
    SPEN = 1;
    TX9 = 0;
    TXEN = 1;
    SYNC = 0;
    BRG16 = 0;
    BRGH = 1;
    TXCKP = 0;
    SPBRG = 312;
    
    //Main Routine
    while(1) {
        if(RD2) {
//            start = 1;
//            __delay_ms(300);
            sendUART(0);
            for(int i = 0; i <= 5; i++) {
                sendUART(readADC(i)/5.688 + 1); // for some reason 4 points to AN5
            }
        }
//        if (start) {
//            sendUART(0);
//            for(int i = 0; i <= 5; i++) {
//                sendUART(readADC(i)/4.015+1); // for some reason 4 points to AN5
//            }
//        }
    }
    return;
}

uint8_t stage = 0;
int stage1;
int stage2;

void __interrupt() ISR(void) {
    
    if(TMR1IF) {
        if (stage == 0) {
            RD1 = 1;
            stage1 = 65535 - (6000 + (pot*23));
            stage2 = 5535 - stage1;
            TMR1 = stage1;
            stage++;
        } else if (stage == 1) {
            RD1 = 0;
            TMR1 = stage2;
            stage++;
        } else if (stage == 2) {
            TMR1 = 5535;
            stage++;
        } else if (stage == 3) {
            TMR1 = 5535;
            stage++;
        } else if (stage == 4) {
            TMR1 = 5535;
            stage = 0;
        }
        TMR1IF = 0;
    }
    
}