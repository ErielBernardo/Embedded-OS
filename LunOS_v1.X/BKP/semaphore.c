/**
 * LunOS - Luan Operating System
 */

#include "semaphore.h"
#include "kernel.h"

extern t_r_queue ready_queue;

void sem_init(sem_t *s, int valor) {
  DISABLE_ALL_INTERRUPTS();
  
  s->contador = valor;
  s->bloqued_size = 0;
  s->task_to_ready = 0;
  
  ENABLE_ALL_INTERRUPTS();
}

void sem_wait(sem_t *s) {
  DISABLE_ALL_INTERRUPTS();
  s->contador -= 1;
  if (s->contador < 0) {
    //s->contador = 0;
    s->bloqued_Queue[s->bloqued_size] = ready_queue.task_running;
    s->bloqued_size = (s->bloqued_size + 1) % MAX_TASKS;
    dispatcher(WAITING_SEM);
  }
  ENABLE_ALL_INTERRUPTS();
}

void sem_post(sem_t *s) {
  DISABLE_ALL_INTERRUPTS();
  s->contador += 1;
  if (s->contador < 0) {
    ready_queue.tasks[s->bloqued_Queue[s->task_to_ready]].task_state = READY;
    s->task_to_ready = (s->task_to_ready + 1) % MAX_TASKS;
    dispatcher(READY);
  }
  ENABLE_ALL_INTERRUPTS();
}

int sem_get_value(sem_t s) {
  return s.contador;
}
