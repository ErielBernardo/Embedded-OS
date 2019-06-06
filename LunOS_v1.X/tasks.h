/**
 * LunOS - Luan Operating System
 */
#include "types.h"
#ifndef TASKS_H
#define	TASKS_H

#define MAX_BOTTLES 3

void user_conf();

void count_bottles();
void fill_bottle();
void check_level();
void cover_bottle();
void count_out();

#endif	/* TASKS_H */

typedef enum {EMPTY = 0, FULL, CLOSED, FAIL} bottle_state;
typedef enum {FREE_ = 0, BUSY_} buffer_state;

typedef struct {
    bottle_state bottle_state;
} t_bottle;

typedef struct {
    int count;
    t_bottle bottles[MAX_BOTTLES];
    buffer_state p_state;
} t_buffer;