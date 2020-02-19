#include <kernel/peripheral.h>

#ifndef _TIMER_H
#define _TIMER_H

#define ARMTIMER_CONTROL ((volatile unsigned int *)(PERIPHERAL_BASE + 0xB408))
#define IRQ_ENABLE_BASIC_IRQS ((volatile unsigned int *)(ENABLE_BASIC_IRQS))
#define ARMTIMER_LOAD ((volatile unsigned int *)(PERIPHERAL_BASE + 0xB400))
#define RPI_ARMTIMER_CTRL_ENABLE (1 << 7)
#define ARMTIMER_CONTROL ((volatile unsigned int *)(PERIPHERAL_BASE + 0xB408))
#define RPI_ARMTIMER_CTRL_23BIT (1 << 1)
#define RPI_ARMTIMER_CTRL_PRESCALE_1 (0 << 2)
#define RPI_ARMTIMER_CTRL_IRQ_ENABLE (1 << 5)
#define RPI_ARMTIMER_CTRL_ENABLE (1 << 7)
#define ARMTIMER_ACQ ((volatile unsigned int *)(PERIPHERAL_BASE + 0xB40C))

void timer_init(uint32_t period_ms);
void enable_timer_irq(void);

#endif /*_TIMER_H */