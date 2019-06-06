/**
 * LunOS - Luan Operating System
 */

#include "types.h"
#include "kernel.h"
#include "sralloc.h"

// Fila de aptos
t_r_queue ready_queue;

// Chamadas de sistema

void lunos_createTask(unsigned int prior, void(*f)()) {
  t_pcb new_task;

  // Preenche as informações da TCB da nova tarefa
  new_task.task_id = ready_queue.tasks_installed + 1;
  new_task.task_prior = prior;
  new_task.task_state = READY;
  new_task.task_f = f;
  new_task.task_stack.stack_level = 0;
  new_task.task_delay_time = 0;
  new_task.task_installed = 0; // adicionada **********************************

  // Inserir nova tarefa na fila de aptos
  ready_queue.tasks[ready_queue.tasks_installed] = new_task;
  ready_queue.tasks_installed++;
}

void lunos_delayTask(unsigned int time) {
  DISABLE_ALL_INTERRUPTS();

  ready_queue.tasks[ready_queue.task_running].task_delay_time = time;
  dispatcher(WAITING);

  ENABLE_ALL_INTERRUPTS();
}

void lunos_exit() {

}

// Rotinas do S.O

void lunos_start() {
  lunos_init();  
  while (1);
}

void lunos_init() {
  SRAMInitHeap();
  ready_queue.task_running = 0;
  ready_queue.tasks_installed = 0;
  lunos_createTask(0, &task_idle);
}

unsigned int scheduler() {
#if RR_SCH
  return round_robin();
#elif PRIOR_SCH 
  return priority();
#endif
}

void dispatcher(t_state state) {
  DISABLE_ALL_INTERRUPTS();

  SAVE_CONTEXT(state);

  ready_queue.task_running = scheduler();

  RESTORE_CONTEXT(RUNNING);

  ENABLE_ALL_INTERRUPTS();
}

unsigned int round_robin() {
  int next_task_to_run = ready_queue.task_running;

  do {
    next_task_to_run = (next_task_to_run + 1) % ready_queue.tasks_installed;
  } while (ready_queue.tasks[next_task_to_run].task_state != READY);

  return next_task_to_run;
}

unsigned int priority() {
  int i, maior_index = 0;
  for (i = 1; i < ready_queue.tasks_installed; i++) {
    if (ready_queue.tasks[maior_index].task_prior < 
        ready_queue.tasks[i].task_prior && 
        ready_queue.tasks[i].task_state == READY) {
      maior_index = i;
    }
  }
  
  return maior_index;
}

void delay_decrement() {
  int i;

  for (i = 0; i < ready_queue.tasks_installed; i++) {
    if (ready_queue.tasks[i].task_state == WAITING) {
      ready_queue.tasks[i].task_delay_time--;
      if (ready_queue.tasks[i].task_delay_time == 0) {
        ready_queue.tasks[i].task_state = READY;
#if PRIOR_SCH
        dispatcher(READY);
#endif
      }
    }
  }
}

void task_idle()
{
  while (1) Nop();
}
