#include <stdint.h>
#include <common/stdlib.h>
#include <kernel/lfb.h>
#include <kernel/peripheral.h>
#include <kernel/mbox.h>
#include <kernel/timer.h>

const unsigned int interval = 200000;
unsigned int curVal = 0;

void timer_init(uint32_t period_ms)
{
    unsigned long int divisor;
    // Make sure clock is stopped, illegal to change anything while running
    // Get GPU clock (it varies between 200-450Mhz)
    *ARMTIMER_CONTROL &= ~(RPI_ARMTIMER_CTRL_ENABLE);

    mbox[0] = (5 + 3) * 4;  // length of the message
    mbox[1] = MBOX_REQUEST; // this is a request message

    mbox[2] = MBOX_TAG_GET_CLOCK_RATE;
    mbox[3] = 8; // buffer size
    mbox[4] = 8;
    mbox[5] = 4; // CLOCK ID: CORE
    mbox[6] = 0; // Clock Frequency

    mbox[7] = MBOX_TAG_LAST;

    unsigned long int period_in_us = period_ms * 1000;

    // send the message to the GPU and receive answer
    if (mbox_call(MBOX_CH_PROP))
    {

        lfb_print("\nclock freq: ");
        lfb_print(itoa(mbox[6], 10));
        lfb_print("\n");

        // The prescaler divider is set to 250 (based on GPU=250MHz to give 1Mhz clock)
        mbox[6] /= 250;
        // Divisor we would need at current clock speed
        divisor = ((unsigned long int)period_in_us * mbox[6]) / 1000000;
        lfb_print("\nclock divisor: ");
        lfb_print(itoa(divisor, 10));
        lfb_print("\n");

        // Enable the timer interrupt IRQ
        *IRQ_ENABLE_BASIC_IRQS |= SYSTEM_TIMER_IRQ_0;

        // Set the load value to divisor
        *ARMTIMER_LOAD = divisor;

        *ARMTIMER_CONTROL |= RPI_ARMTIMER_CTRL_23BIT | RPI_ARMTIMER_CTRL_PRESCALE_1 | RPI_ARMTIMER_CTRL_IRQ_ENABLE | RPI_ARMTIMER_CTRL_ENABLE;
        lfb_print("irq_setup finished. Control: ");
        lfb_print(itoa(*ARMTIMER_CONTROL, 16));
        lfb_print("\n");
    }
}

void enable_timer_irq(void)
{
    asm volatile("msr daifclr,#2");
}

void timer_handler(void)
{
    *ARMTIMER_ACQ = 1;
    lfb_print("Timer went off!\n");
}