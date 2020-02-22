#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <kernel/kerio.h>
#include <kernel/lfb.h>
#include <common/stdlib.h>
#include <kernel/timer.h>
#include <kernel/pcb.h>
#include <kernel/utils.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for main. */
#endif

    void
    test(void)
{
    while (1)
    {
        printf("test\n");
    }
}
void main(uint32_t r0, uint32_t r1, uint32_t r2)
{
    uint32_t atags;
    if (r0 == 0)
    { // device tree disabled
        atags = 0x100;
    }

    //Init functions
    lfb_init();
    printf("Welcome to PiKernel!\n");

    printf("Initializing UART...\n");
    uart_init();
    printf("UART init complete!\n");

    printf("Initializing memory...\n");
    mem_init((atag_t *)atags);
    printf("Memory init complete!\n");

    process_init();
    //schedule();

    timer_init(1000);
    enable_interrupts();

    create_process(test, "Test", 4);
    printf("Timer init complete!\n");

    printf("main");
    while (1)
    {
        printf("1 \n");
        for (int i = 0; i < 10000000; i++)
        {
            asm volatile("nop");
        }
    }
}