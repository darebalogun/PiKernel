#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <kernel/kerio.h>
#include <kernel/lfb.h>
#include <common/stdlib.h>
#include <kernel/timer.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
    void
    main(uint64_t r0, uint64_t r1, uint64_t r2)
{
    uint64_t atags;
    if (r0 == 0)
    { // device tree disabled
        atags = 0x100;
    }

    //Init functions
    lfb_init();
    printf("Welcome to PiKernel!\n");

    printf("Initializing UART...\n");
    uart_init();
    printf("UART Init complete\n");

    printf("Initializing memory...\n");
    mem_init((atag_t *)atags);
    printf("Memory init complete\n");

    //timer_init(1000);
    //enable_timer_irq();
    printf("Timer init complete!\n");

    while (1)
    {
        char c = uart_getc();
        putc(c);
        uart_send(c);
    }
}