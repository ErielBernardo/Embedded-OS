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
sem_t count_sem, fill_sem, check_sem, cover_sem, out_sem;
pipe_t p;
t_buffer global_buffer;

void user_conf() {
  TRISB = 0b00000001;
  TRISC = 0b01111111;
  TRISD = 0b01111110;
  PORTCbits.RC7 = 1;
  
  //sem_init(&teste_1, 1);
  //sem_init(&teste_2, 0);
  sem_init(&count_sem, 1);
  sem_init(&fill_sem, 0);
  sem_init(&check_sem, 0);
  sem_init(&cover_sem, 0);
  sem_init(&out_sem, 0);
  pipe_create(&p, &w_pipe, &r_pipe);
  mem = SRAMalloc(5);
  
  global_buffer.count = 0;
  global_buffer.p_state = FREE_;
}


void count_bottles(){
    while(1){
        sem_wait(&count_sem);
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
    sem_post(&fill_sem);
}

void fill_bottle(){
    while(1){
        sem_wait(&fill_sem);
        while (!PORTCbits.RC0 && !PORTCbits.RC1 && !PORTCbits.RC2){
            lunos_delayTask(100);
        }
    }
    sem_post(&check_sem);
}

void check_level(){
    while(1){
        sem_wait(&check_sem);
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
    sem_post(&cover_sem);
}

void cover_bottle(){
    while(1){
        sem_wait(&cover_sem);
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
    int count = 0;
    for (int i = 0; i < MAX_BOTTLES; i++){
        if (global_buffer.bottles[i].bottle_state == FULL) count++;
    }
    pipe_write(&p, count);
    sem_post(&out_sem);
}

void count_out(){
    sem_wait(&out_sem);
    unsigned int count;
    while(1){
        pipe_read(&p, &count);
    }
    sem_post(&count_sem);
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