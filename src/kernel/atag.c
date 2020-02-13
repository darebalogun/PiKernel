#include <kernel/atag.h>

// Return the size of memory available, using atag info from bootloader
uint32_t get_mem_size(atag_t *tag)
{
    // Qemu does not emulate bootloader but we can set qemu mem to 1024MB
    // return 1024 * 1024 * 1024;
    while (tag->tag != NONE)
    {
        if (tag->tag == MEM)
        {
            // To test on actual hardware
            return tag->mem.size;
        }
        tag = ((uint32_t *)tag) + tag->tag_size;
    }
    return 0;
}