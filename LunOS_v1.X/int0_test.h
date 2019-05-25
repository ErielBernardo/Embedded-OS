/**
 * LunOS - Luan Operating System
 */

#ifndef INT0_TEST_H
#define	INT0_TEST_H

#include <xc.h>

void config_INT0();
void config_timer0();
void __interrupt() ISR_Int0(void);
void __interrupt() ISR_timer0();

#endif	/* INT0_TEST_H */

