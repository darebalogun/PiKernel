#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <kernel/kerio.h>
#include <kernel/lfb.h>
#include <common/stdlib.h>
#include <kernel/timer.h>

void int_to_string(uint32_t number, char *str);

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
    void
    main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    uint32_t atags;
    if (r0 == 0)
    { // device tree disabled
        atags = 0x100;
    }

    //Init functions
    lfb_init();
    lfb_print("Welcome to PiKernel!\n");
    lfb_print("Initializing UART...\n");
    uart_init();
    lfb_print("UART Init complete\n");
    lfb_print("Initializing memory...\n");
    mem_init((atag_t *)atags);
    lfb_print("Memory init complete\n");
    timer_init(1000);
    enable_timer_irq();
    lfb_print("Timer init complete!\n");

    while (1)
    {
        lfb_print_c(uart_getc());
    }
}

void int_to_string(uint32_t number, char *str)
{
    uint32_t n = number;
    int i = 0;

    while (n != 0)
    {
        str[i++] = n % 10 + '0';
        n = n / 10;
    }

    str[i] = '\0';

    for (int t = 0; t < i / 2; t++)
    {
        str[t] ^= str[i - t - 1];
        str[i - t - 1] ^= str[t];
        str[t] ^= str[i - t - 1];
    }

    if (number == 0)
    {
        str[0] = '0';
        str[1] = '\0';
    }
}