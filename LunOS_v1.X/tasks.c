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

unsigned int data_PIPE;

unsigned char* mem;

sem_t count_sem, fill_sem, check_sem, cover_sem, out_sem, w_pipe, r_pipe;
pipe_t p;
t_buffer global_buffer;

void user_conf() {
//  TRISX = 0b76543210;
    TRISB = 0b00000101;
    TRISC = 0b01111111;
    TRISD = 0b00001110;

    PORTBbits.RB1 = 1;

    PORTCbits.RC7 = 1;

    PORTDbits.RD4 = 1;
    PORTDbits.RD5 = 1;
    PORTDbits.RD6 = 1;
    PORTDbits.RD7 = 1;

    sem_init(&count_sem, 1);
    sem_init(&fill_sem, 0);
    sem_init(&check_sem, 0);
    sem_init(&cover_sem, 0);
    sem_init(&out_sem, 0);
    pipe_create(&p, &w_pipe, &r_pipe);
    //mem = SRAMalloc(5);

    global_buffer.count = 0;
    global_buffer.p_state = FREE_;
}

/*
 * global_buffer é o recurso do sistema,
 * representa um conjunto de bottles,
 * tem tamnho maximo de 3
 * 
 * semafaros sao usados para travar tarefas
 * que estao usando o recurso
 * 
 * pipe é usado para a comunicacao da quantidade
 * de garrafas que foram enchidas e tampadas
 */

void count_bottles(){
    while(1){
        sem_wait(&count_sem);    // espera liberar semafaro dessa tarefa
        PORTDbits.RD4 = 0;  // indica q a tarefa esta executando
        if (global_buffer.p_state != BUSY_) { // trata estado do buffer
            while(global_buffer.count < MAX_BOTTLES){   // enche o buffer
                while(PORTCbits.RC6);   // trata botao
                PORTCbits.RC7 = 0;  // liga a esteira
                lunos_delayTask(300);   // por 300 ms
                t_bottle bottle; // cria garrafa
                global_buffer.bottles[global_buffer.count] = bottle;    // adiciona a garrafa no buffer
                global_buffer.bottles[global_buffer.count].bottle_state = EMPTY;    // estado da garrafa
                global_buffer.count += 1;   // incrementa count de garrafas no buffer
                PORTCbits.RC7 = 1;  // liga a esteira
            }
        }
        global_buffer.p_state = BUSY_;  // marca que o buffer esta cheio e ocupado
        sem_post(&fill_sem);   // Libera semaforo da proxima tarefa
        PORTDbits.RD4 = 1;  // indica q a tarefa deixou de executar
    }
}

void fill_bottle(){
    while(1){
        sem_wait(&fill_sem);    // espera liberar semafaro dessa tarefa
        PORTDbits.RD5 = 0;  // indica q a tarefa esta executando
        PORTCbits.RC7 = 0;  // liga a esteira
        lunos_delayTask(300); // por 300 ms
        PORTCbits.RC7 = 1;  // desliga a esteira
        lunos_delayTask(500);   // 500 ms para o retorno dos bicos
        while (PORTCbits.RC0 && PORTCbits.RC1 && PORTCbits.RC2);   // Tratamento do botoa
        lunos_delayTask(200);   // Tratamento do botoa
        sem_post(&check_sem);   // Libera semaforo da proxima tarefa
        PORTDbits.RD5 = 1;  // indica q a tarefa deixou de executar
    }
}

void check_level(){
    while(1){
        sem_wait(&check_sem);
        PORTDbits.RD6 = 0;  // indica q a tarefa esta executando
        PORTCbits.RC7 = 0;  // liga a esteira
        lunos_delayTask(500); // por 500 ms
        PORTCbits.RC7 = 1;  // desliga a esteira
        lunos_delayTask(500);   // 500 ms para o retorno dos sensores
        while (PORTDbits.RD1 && PORTDbits.RD2 && PORTDbits.RD3);
        lunos_delayTask(200);   // Tratamento do botoa
        
//        if (PORTDbits.RD1)
//            global_buffer.bottles[0].bottle_state = EMPTY;
//        else
//            global_buffer.bottles[0].bottle_state = FULL;
//
//        if (PORTDbits.RD2)
//            global_buffer.bottles[1].bottle_state = EMPTY;
//        else
//            global_buffer.bottles[1].bottle_state = FULL;
//
//        if (PORTDbits.RD3)
//            global_buffer.bottles[2].bottle_state = EMPTY;
//        else
//            global_buffer.bottles[2].bottle_state = FULL;
        
        sem_post(&cover_sem);   // Libera semaforo da proxima tarefa
        PORTDbits.RD6 = 1;  // indica q a tarefa deixou de executar
    }
}

void cover_bottle(){
    while(1){
        sem_wait(&cover_sem);    // espera liberar semafaro dessa tarefa
        PORTDbits.RD7 = 0;
        PORTCbits.RC7 = 0;
        lunos_delayTask(500);
        while (PORTCbits.RC3 && PORTCbits.RC4 && PORTCbits.RC5); // trata botao
        lunos_delayTask(200); // por 500 ms
//        if (PORTCbits.RC3){
//            if(global_buffer.bottles[0].bottle_state == FULL)
                global_buffer.bottles[0].bottle_state = CLOSED;
//            else if (PORTCbits.RC3)
//                global_buffer.bottles[0].bottle_state = FAIL;
//        }  
//        if (PORTCbits.RC4){
//            if(global_buffer.bottles[1].bottle_state == FULL)
//                global_buffer.bottles[1].bottle_state = CLOSED;
//            else if (PORTCbits.RC4)
//                global_buffer.bottles[1].bottle_state = FAIL;
//        }
//        if (PORTCbits.RC5){
//            if(global_buffer.bottles[2].bottle_state == FULL)
//                global_buffer.bottles[2].bottle_state = CLOSED;
//            else if (PORTCbits.RC5)
//                global_buffer.bottles[2].bottle_state = FAIL;
//        }
//        unsigned int count = 0;
//        for (int i = 0; i < MAX_BOTTLES; i++){
//            if (global_buffer.bottles[i].bottle_state == CLOSED) count++;
//        }
        unsigned int count = 3; // MUDAR ISSO, SÓ SERVE PARA TESTE DO PIPE PARA count_out !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        pipe_write(&p, count);
        sem_post(&out_sem);
        PORTCbits.RC7 = 1;
        PORTDbits.RD7 = 1; 
    }
}

void count_out(){
    while(1){
        unsigned int data;
        unsigned int count = 0;
        sem_wait(&out_sem);    // espera liberar semafaro dessa tarefa
        PORTBbits.RB1 = 0;  // indica q a tarefa esta executando
        PORTCbits.RC7 = 0;  // liga esteira
        pipe_read(&p, &data);   // le o pipe
        data_PIPE = data;
        while (count < data){
            while(PORTBbits.RB2);
            lunos_delayTask(200);
            count++;
        }
        PORTBbits.RB1 = 1; // pisca led da tarefa
        lunos_delayTask(200); // pisca led da tarefa
        PORTCbits.RC7 = 1;  // deliga esteira
        PORTBbits.RB1 = 0; // pisca led da tarefa
        lunos_delayTask(200); // pisca led da tarefa
        
        sem_post(&count_sem);   // Libera semaforo da proxima tarefa
        global_buffer.p_state = FREE_;
        global_buffer.count = 0;
        PORTBbits.RB1 = 1;  // indica q a tarefa deixou de executar 
    }
}
