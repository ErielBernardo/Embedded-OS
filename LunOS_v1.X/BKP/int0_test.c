/**
 * LunOS - Luan Operating System
 */

#include <pic18f4520.h>

#include "int0_test.h"
#include "kernel.h"

extern t_r_queue ready_queue;
int quantum = QUANTUM;

void config_INT0() {
  TRISDbits.RD0 = 0;
  INTCONbits.GIE = 1;
  INTCONbits.INT0E = 1;
  INTCONbits.INT0IE = 1;
  INTCONbits.INT0IF = 0;
  INTCON2bits.RBPU = 0;
  INTCON2bits.INTEDG0 = 0;
}

void config_timer0() {
  // Timer
  // Configuração para o timer estourar a cada
  // 4 ms
  // Configuração considerando o timer 0 8 bits
  T0CONbits.T0CS = 0;
  T0CONbits.PSA = 0;
  T0CONbits.T0PS = 0b111;
  INTCONbits.PEIE = 1;
  INTCONbits.TMR0IE = 1;
  TMR0L = 252;
}

//void __interrupt() ISR_Int0(void) {
//  DISABLE_ALL_INTERRUPTS();
//
//  if (INTCONbits.INT0IF) {
//    PORTDbits.RD0 = 1;
//    INTCONbits.INT0IF = 0;
//    SAVE_CONTEXT(READY);
//    // Decrementa o tempo de espera por delay de cada
//    // tarefa que está na fila de aptos em estado de 
//    // WAITING
//    delay_decrement();
//    ready_queue.task_running = round_robin();
//    PORTDbits.RD0 = 0;
//    RESTORE_CONTEXT(RUNNING);
//  }
//
//  ENABLE_ALL_INTERRUPTS();
//
//}

void __interrupt() ISR_timer0() {
  DISABLE_ALL_INTERRUPTS();

  if (INTCONbits.TMR0IF) {
    INTCONbits.TMR0IF = 0;
    TMR0L = 252;    
    PORTDbits.RD0 = ~PORTDbits.RD0;
    delay_decrement();
#if RR_SCH
    quantum--;
    if (quantum <= 0) {
      quantum = QUANTUM;
      SAVE_CONTEXT(READY);
      ready_queue.task_running = round_robin();
      RESTORE_CONTEXT(RUNNING);
    }
#endif
  }

  ENABLE_ALL_INTERRUPTS();
}

