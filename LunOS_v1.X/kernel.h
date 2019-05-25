/**
 * LunOS - Luan Operating System
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include <xc.h>
#include <pic18f4520.h>
#include "types.h"

// Fila de aptos
extern t_r_queue ready_queue;
int who = 0;
int size = 0;

#define ENABLE_ALL_INTERRUPTS() INTCONbits.GIE = 1
#define DISABLE_ALL_INTERRUPTS() INTCONbits.GIE = 0

// Rotinas para tratamento de contexto
#define SAVE_CONTEXT(state) {\
    who = ready_queue.task_running;\
    ready_queue.tasks[who].task_state = state;\
    ready_queue.tasks[who].W_reg = WREG;\
    ready_queue.tasks[who].STATUS_reg = STATUS;\
    ready_queue.tasks[who].BSR_reg = BSR;\
    size = 0;\
    while (STKPTR) {\
        ready_queue.tasks[who].task_stack.h_stack[size].TOSU_reg = TOSU;\
        ready_queue.tasks[who].task_stack.h_stack[size].TOSH_reg = TOSH;\
        ready_queue.tasks[who].task_stack.h_stack[size].TOSL_reg = TOSL;\
        size++;\
        __asm("POP");\
    }\
    ready_queue.tasks[who].task_stack.stack_level = size - 1;\
}\

#define RESTORE_CONTEXT(state) {\
    who = ready_queue.task_running;\
    STKPTR = 0;\
    if (ready_queue.tasks[who].task_stack.stack_level == 0) {\
        __asm("PUSH");\
        TOS = ready_queue.tasks[who].task_f;\
    }\
    else {\
        ready_queue.tasks[who].task_state = state;\
        WREG = ready_queue.tasks[who].W_reg;\
        STATUS = ready_queue.tasks[who].STATUS_reg;\
        BSR = ready_queue.tasks[who].BSR_reg;\
        size = ready_queue.tasks[who].task_stack.stack_level;\ 
        while(size) {\
            __asm("PUSH");\
            TOSL = ready_queue.tasks[who].task_stack.h_stack[size].TOSL_reg;\
            TOSH = ready_queue.tasks[who].task_stack.h_stack[size].TOSH_reg;\
            TOSU = ready_queue.tasks[who].task_stack.h_stack[size].TOSU_reg;\
            size--;\
        }\
    }\
}\

// Fim das rotinas para tratamento de contexto

// Chamadas de sistema
void lunos_createTask(unsigned int prior, void(*f)());
void lunos_delayTask(unsigned int time);
void lunos_exit();

// Rotinas do S.O
void lunos_start();
void lunos_init();
unsigned int scheduler();
void dispatcher(t_state state);
unsigned int round_robin();
unsigned int priority();
void delay_decrement();
void task_idle();


#endif	/* KERNEL_H */

