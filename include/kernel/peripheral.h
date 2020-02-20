#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#define PERIPHERAL_BASE 0x3F000000

#define PERIPHERAL_LENGTH 0x01000000

#define MAILBOX_OFFSET 0xB880
#define UART0_OFFSET 0x201000
#define GPIO_OFFSET 0x200000
#define GPIO_BASE (PERIPHERAL_BASE + GPIO_OFFSET)

#define GPFSEL0 ((volatile unsigned int *)(GPIO_BASE + 0x00000000))
#define GPFSEL1 ((volatile unsigned int *)(GPIO_BASE + 0x00000004))
#define GPFSEL2 ((volatile unsigned int *)(GPIO_BASE + 0x00000008))
#define GPFSEL3 ((volatile unsigned int *)(GPIO_BASE + 0x0000000C))
#define GPFSEL4 ((volatile unsigned int *)(GPIO_BASE + 0x00000010))
#define GPFSEL5 ((volatile unsigned int *)(GPIO_BASE + 0x00000014))
#define GPSET0 ((volatile unsigned int *)(GPIO_BASE + 0x0000001C))
#define GPSET1 ((volatile unsigned int *)(GPIO_BASE + 0x00000020))
#define GPCLR0 ((volatile unsigned int *)(GPIO_BASE + 0x00000028))
#define GPLEV0 ((volatile unsigned int *)(GPIO_BASE + 0x00000034))
#define GPLEV1 ((volatile unsigned int *)(GPIO_BASE + 0x00000038))
#define GPEDS0 ((volatile unsigned int *)(GPIO_BASE + 0x00000040))
#define GPEDS1 ((volatile unsigned int *)(GPIO_BASE + 0x00000044))
#define GPHEN0 ((volatile unsigned int *)(GPIO_BASE + 0x00000064))
#define GPHEN1 ((volatile unsigned int *)(GPIO_BASE + 0x00000068))
#define GPPUD ((volatile unsigned int *)(GPIO_BASE + 0x00000094))
#define GPPUDCLK0 ((volatile unsigned int *)(GPIO_BASE + 0x00000098))
#define GPPUDCLK1 ((volatile unsigned int *)(GPIO_BASE + 0x0000009C))

#define SYSTEM_TIMER_OFFSET 0x3000
#define INTERRUPTS_OFFSET 0xB000

#define IRQ_BASIC_PENDING (PERIPHERAL_BASE + 0x0000B200)
#define IRQ_PENDING_1 (PERIPHERAL_BASE + 0x0000B204)
#define IRQ_PENDING_2 (PERIPHERAL_BASE + 0x0000B208)
#define FIQ_CONTROL (PERIPHERAL_BASE + 0x0000B20C)
#define ENABLE_IRQS_1 (PERIPHERAL_BASE + 0x0000B210)
#define ENABLE_IRQS_2 (PERIPHERAL_BASE + 0x0000B214)
#define ENABLE_BASIC_IRQS (PERIPHERAL_BASE + 0x0000B218)
#define DISABLE_IRQS_1 (PERIPHERAL_BASE + 0x0000B21C)
#define DISABLE_IRQS_2 (PERIPHERAL_BASE + 0x0000B220)
#define DISABLE_BASIC_IRQS (PERIPHERAL_BASE + 0x0000B224)

#define SYSTEM_TIMER_IRQ_0 (1 << 0)
#define SYSTEM_TIMER_IRQ_1 (1 << 1)
#define SYSTEM_TIMER_IRQ_2 (1 << 2)
#define SYSTEM_TIMER_IRQ_3 (1 << 3)

#define TIMER_CS (PERIPHERAL_BASE + 0x00003000)
#define TIMER_CLO (PERIPHERAL_BASE + 0x00003004)
#define TIMER_CHI (PERIPHERAL_BASE + 0x00003008)
#define TIMER_C0 (PERIPHERAL_BASE + 0x0000300C)
#define TIMER_C1 (PERIPHERAL_BASE + 0x00003010)
#define TIMER_C2 (PERIPHERAL_BASE + 0x00003014)
#define TIMER_C3 (PERIPHERAL_BASE + 0x00003018)

#define TIMER_CS_M0 (1 << 0)
#define TIMER_CS_M1 (1 << 1)
#define TIMER_CS_M2 (1 << 2)
#define TIMER_CS_M3 (1 << 3)

#endif