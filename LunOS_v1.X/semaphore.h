/**
 * LunOS - Luan Operating System
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "config.h"
#include "types.h"

// Estrutura de dados de controle do semáforo
typedef struct semaphore {
  int contador;
  unsigned int bloqued_Queue[MAX_TASKS];
  unsigned int bloqued_size;
  unsigned int task_to_ready;
} sem_t;

void sem_init(sem_t *s, int valor);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);
int sem_get_value(sem_t s);

#endif	/* TYPES_H */