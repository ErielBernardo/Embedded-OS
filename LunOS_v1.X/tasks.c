/**
 * LunOS - Luan Operating System
 */

#include <pic18f4520.h>
#include "tasks.h"
#include <xc.h>
#include "semaphore.h"
#include "kernel.h"
#include "pipe.h"
#include "sralloc.h"
#include "types.h"

unsigned char* mem;

sem_t teste_1, teste_2, w_pipe, r_pipe;
pipe_t p;
t_buffer global_buffer;

void user_conf() {
  TRISB = 0b00000001;
  TRISC = 0b01111111;
  TRISD = 0b11111110;
  PORTCbits.RC7 = 1;
  
  sem_init(&teste_1, 1);
  sem_init(&teste_2, 0);
  pipe_create(&p, &w_pipe, &r_pipe);
  mem = SRAMalloc(5);

  global_buffer.count = 0;
  global_buffer.p_state = FREE_;
}


void count_bottles(){
    while(global_buffer.count < MAX_BOTTLES){
        if(!PORTBbits.RB6){
            lunos_delayTask(500);
            t_bottle bottle;
            global_buffer.bottles[global_buffer.count] = bottle;
            global_buffer.bottles[global_buffer.count].bottle_state = EMPTY;
            global_buffer.count += 1;
            }
        }
    global_buffer.p_state = BUSY_;
}

void fill_bottle(){
    while (!PORTCbits.RC0 && !PORTCbits.RC1 && !PORTCbits.RC2){
        lunos_delayTask(100);
    }
}

void check_level(){
    if (!PORTDbits.RD1)
        global_buffer.bottles[0].bottle_state = EMPTY;
    else if (PORTDbits.RD1)
        global_buffer.bottles[0].bottle_state = FULL;
    else global_buffer.bottles[0].bottle_state = global_buffer.bottles[0].bottle_state;
                
    if (!PORTDbits.RD2)
        global_buffer.bottles[1].bottle_state = EMPTY;
    else if (PORTDbits.RD2)
        global_buffer.bottles[1].bottle_state = FULL;
    else global_buffer.bottles[1].bottle_state = global_buffer.bottles[1].bottle_state;
        
    if (!PORTDbits.RD3)
        global_buffer.bottles[2].bottle_state = EMPTY;
    else if (PORTDbits.RD3)
        global_buffer.bottles[2].bottle_state = FULL;  
    else global_buffer.bottles[2].bottle_state = global_buffer.bottles[2].bottle_state;
}

void cover_bottle(){
    if (PORTCbits.RC3){
        if(global_buffer.bottles[0].bottle_state == FULL)
            global_buffer.bottles[0].bottle_state = CLOSED;
        else if (PORTCbits.RC3)
            global_buffer.bottles[0].bottle_state = FAIL;
        else global_buffer.bottles[0].bottle_state = global_buffer.bottles[0].bottle_state;
    }  
    if (PORTCbits.RC4){
        if(global_buffer.bottles[1].bottle_state == FULL)
            global_buffer.bottles[1].bottle_state = CLOSED;
        else if (PORTCbits.RC4)
            global_buffer.bottles[1].bottle_state = FAIL;
        else global_buffer.bottles[1].bottle_state = global_buffer.bottles[1].bottle_state;
    }
    if (PORTCbits.RC5){
        if(global_buffer.bottles[2].bottle_state == FULL)
            global_buffer.bottles[2].bottle_state = CLOSED;
        else if (PORTCbits.RC5)
            global_buffer.bottles[2].bottle_state = FAIL;
        else global_buffer.bottles[2].bottle_state = global_buffer.bottles[2].bottle_state;
    }
}

void count_out(){
    
}

void task_0() {
  while(1){
    pipe_write(&p, 1);
    PORTBbits.RB3 = ~PORTBbits.RB3;
    lunos_delayTask(1000);
  }

}

void task_1() {
  unsigned int dado;
  while(1){    
    pipe_read(&p, &dado);
    PORTBbits.RB4 = ~PORTBbits.RB4;
    lunos_delayTask(2000);
  }
}

void task_2() {
  while(1){
    PORTBbits.RB5 = ~PORTBbits.RB5;
    lunos_delayTask(1000);
  }

}

void task_bozo() {
  while (1) {
    int i;
    sem_wait(&teste_1);
    for (i = 0; i < 4; i++) {
      mem[i] = i+1;
    }
    sem_post(&teste_2);
  }
}

void task_xuxa() {
  int i;
  while (1) {
    sem_wait(&teste_2);
    for (i = 0; i < 4; i++) {
      if (mem[i] % 2 == 0) 
        PORTBbits.RB3 = ~PORTBbits.RB3;
      else
        PORTBbits.RB5 = ~PORTBbits.RB5;
      lunos_delayTask(1000);
    }
    sem_post(&teste_1);
  }
}