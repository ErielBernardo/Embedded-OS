/**
 * LunOS - Luan Operating System
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

#define STACK_SIZE 31
#define PACK_SIZE 6

typedef unsigned int tid;
typedef unsigned char byte;
typedef enum {READY = 0, RUNNING, WAITING, FINISHED, WAITING_PIPE, WAITING_SEM} t_state;
typedef enum {EMPTY = 0, FULL, WAITING_PACKING, PACKED} bottle_state;
typedef enum {FREE_ = 0, BUSY_} tap_state;
typedef enum {EMPTY_ = 0, COMPLETED} pack_state;

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
    t_bottle pack[PACK_SIZE];
    pack_state p_state;
} bottle_pack;

typedef struct {
    t_bottle bottle;
    tap_state filler_state;
} filler;

typedef struct {
    t_bottle bottle;
} put_cover_machine;

#endif	/* TYPES_H */


