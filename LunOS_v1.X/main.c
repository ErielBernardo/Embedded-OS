/**
 * LunOS - Luan Operating System
 */

#include <xc.h>
#include "kernel.h"
#include "tasks.h"
#include "int0_test.h"
#include "semaphore.h"
#include "sralloc.h"
#pragma config PBADEN = OFF
#pragma config WDT    = OFF

asm("GLOBAL _task_idle, _task_0, _task_1, _task_2, _task_bozo, _task_xuxa, _count_bottles, _fill_bottle");

void main(void) {

  lunos_init();

  user_conf();
  config_INT0();
  config_timer0();

  // Cria tarefas de usuário
  lunos_createTask(3, &task_0);
  lunos_createTask(4, &task_1);
  lunos_createTask(5, &count_bottles);
 // lunos_createTask(5, &task_2);
  //lunos_createTask(5, &task_bozo);
  //lunos_createTask(5, &task_xuxa);
  
  dispatcher(READY);

  while (1);

  return;
}
