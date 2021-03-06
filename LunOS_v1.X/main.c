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

asm("GLOBAL _task_idle, _count_bottles, _fill_bottle, _check_level, _cover_bottle, _count_out");

void main(void) {

  lunos_init();

  user_conf();
  config_INT0();
  config_timer0();

  // Cria tarefas de usu�rio
    
  lunos_createTask(5, &count_bottles);
  lunos_createTask(4, &fill_bottle);
  lunos_createTask(5, &check_level);
  lunos_createTask(5, &cover_bottle);
  lunos_createTask(4, &count_out);
  
  dispatcher(READY);

  while (1);

  return;
}
