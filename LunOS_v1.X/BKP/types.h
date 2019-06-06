/**
 * LunOS - Luan Operating System
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

#define STACK_SIZE 31
#define MAX_BOTTLES 3

typedef unsigned int tid;
typedef unsigned char byte;
typedef enum {READY = 0, RUNNING, WAITING, FINISHED, WAITING_PIPE, WAITING_SEM} t_state;
typedef enum {EMPTY = 0, FULL, CLOSED, FAIL} bottle_state;
typedef enum {FREE_ = 0, BUSY_} buffer_state;

typedef struct data_stack {
  byte TOSU_reg;
  byte TOSL_reg;
  byte TOSH_reg;    
} t_data_stack;

typedef struct stack {
  //__uint24 h_stack[STACK_SIZE];
  t_data_stack h_stack[STACK_SIZE];  
  int stack_level;
} t_stack;

typedef struct pcb {
  tid task_id;
  unsigned int task_prior;
  byte PC_reg;
  byte W_reg;
  byte STATUS_reg;
  byte BSR_reg;
  t_state task_state;
  t_stack task_stack;
  void(*task_f)();
  unsigned int task_delay_time;
} t_pcb;

typedef struct r_queue {
  t_pcb tasks[MAX_TASKS+1];
  unsigned int tasks_installed;
  int task_running;
} t_r_queue;

typedef struct {
    bottle_state bottle_state;
} t_bottle;

typedef struct {
    int count;
    t_bottle bottles[MAX_BOTTLES];
    buffer_state p_state;
} t_buffer;

#endif	/* TYPES_H */


