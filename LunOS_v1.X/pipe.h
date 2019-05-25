/**
 * LunOS - Luan Operating System
 */

#ifndef PIPE_H
#define	PIPE_H

#include "semaphore.h"

#define PIPE_SIZE 2

typedef struct pipe {
  unsigned int pipe_data[PIPE_SIZE];
  unsigned int index_write;
  unsigned int index_read;
  sem_t *w;
  sem_t *r;
} pipe_t;

void pipe_create(pipe_t *p, sem_t *w, sem_t *r);
void pipe_write(pipe_t *p, unsigned int data);
void pipe_read(pipe_t *p, unsigned int *data);


#endif	/* PIPE_H */

