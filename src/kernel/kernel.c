#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mem.h>
#include <kernel/kerio.h>
#include <kernel/gpu.h>
#include <kernel/lfb.h>
#include <common/stdlib.h>

void int_to_string(uint32_t number, char *str);

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
    void
    kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // Declare as unused
    (void)r0;
    (void)r1;
    (void)atags;

    //Init functions
    lfb_init();
    lfb_print(0, 0, "Hello World!\n");
    lfb_print_c(0, 1, 'h');
    /**
    uart_init();
    puts("uart init complete\n");
    mem_init((atag_t *)atags);
    puts("mem init complete\n");
    //gpu_init();
    puts("gpu init complete\n");
    puts("Welcome to PiKernel!\r\n");
    puts("Initilizing Memory\n");
    */

    uint32_t mem_size = get_mem_size((atag_t *)atags);
    char mem_size_str[1024];
    int_to_string(mem_size, mem_size_str);
    uart_puts(mem_size_str);

    while (1)
    {
        uart_putc(uart_getc());
        uart_putc('\n');
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