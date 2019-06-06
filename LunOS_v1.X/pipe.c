/**
 * LunOS - Luan Operating System
 */

#include "pipe.h"
#include "kernel.h"
#include "semaphore.h"

void pipe_create(pipe_t *p, sem_t * w, sem_t * r) {
  p->index_write = 0;
  p->index_read = 0;
  p->w = w;
  p->r = r;
  sem_init(p->w, PIPE_SIZE);
  sem_init(p->r, 0);
}

void pipe_write(pipe_t *p, unsigned int data) {
  //DISABLE_ALL_INTERRUPTS();
  sem_wait(p->w);
  p->pipe_data[p->index_write] = data;
  p->index_write = (p->index_write + 1) % PIPE_SIZE;
  sem_post(p->r);
  //ENABLE_ALL_INTERRUPTS();
}

void pipe_read(pipe_t *p, unsigned int * data) {
  //DISABLE_ALL_INTERRUPTS();
  sem_wait(p->r);
  *data = p->pipe_data[p->index_read];
  p->index_read = (p->index_read + 1) % PIPE_SIZE;
  sem_post(p->w);
  //ENABLE_ALL_INTERRUPTS();
}
