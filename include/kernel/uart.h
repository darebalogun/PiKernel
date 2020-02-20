#include <stddef.h>
#include <stdint.h>
#include <kernel/peripheral.h>

#ifndef UART_H
#define UART_H

/* PL011 UART registers */
#define UART0_DR ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201000))
#define UART0_FR ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201018))
#define UART0_IBRD ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201024))
#define UART0_FBRD ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201028))
#define UART0_LCRH ((volatile unsigned int *)(PERIPHERAL_BASE + 0x0020102C))
#define UART0_CR ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201030))
#define UART0_IMSC ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201038))
#define UART0_ICR ((volatile unsigned int *)(PERIPHERAL_BASE + 0x00201044))

void uart_init();
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(char *s);
void uart_hex(unsigned int d);

#endif