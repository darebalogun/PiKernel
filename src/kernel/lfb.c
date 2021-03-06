// Modified by Dare Balogun (C) 2020
/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <kernel/mbox.h>
#include <common/stdlib.h>
#include <kernel/kerio.h>

static unsigned int x, y, numrows, numcolumns;
static char *screenbuff;
static int index;

/* PC Screen Font as used by Linux Console */
typedef struct
{
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_font_psf_start;

unsigned int width, height, pitch; /* dimensions and channel order */
unsigned char *lfb;                /* raw frame buffer address */

/**
 * Set screen resolution to 1024x768
 */
void lfb_init()
{
    psf_t *font = (psf_t *)&_binary_font_psf_start;

    mbox[0] = 35 * 4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003; //set phy wh
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = 1024; //FrameBufferInfo.width
    mbox[6] = 768;  //FrameBufferInfo.height

    mbox[7] = 0x48004; //set virt wh
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1024; //FrameBufferInfo.virtual_width
    mbox[11] = 768;  //FrameBufferInfo.virtual_height

    mbox[12] = 0x48009; //set virt offset
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0; //FrameBufferInfo.x_offset
    mbox[16] = 0; //FrameBufferInfo.y.offset

    mbox[17] = 0x48005; //set depth
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32; //FrameBufferInfo.depth

    mbox[21] = 0x48006; //set pixel order
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1; //RGB, not BGR preferably

    mbox[25] = 0x40001; //get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096; //FrameBufferInfo.pointer
    mbox[29] = 0;    //FrameBufferInfo.size

    mbox[30] = 0x40008; //get pitch
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0; //FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_LAST;

    //this might not return exactly what we asked for, could be
    //the closest supported resolution instead
    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0)
    {
        mbox[28] &= 0x3FFFFFFF; //convert GPU address to ARM address
        width = mbox[5];        //get actual physical width
        height = mbox[6];       //get actual physical height
        pitch = mbox[33];       //get number of bytes per line
        lfb = (void *)((unsigned long)mbox[28]);

        numrows = height / font->height;
        numcolumns = width / font->width;

        char screenbuffer[numrows * numcolumns];
        screenbuff = screenbuffer;
    }
    else
    {
        puts("Unable to set screen resolution to 1024x768x32\n");
    }
}

void lfb_print(char *s)
{
    // draw next character if it's not zero
    while (*s)
    {
        lfb_print_c(*s);
        // next character
        s++;
    }
}

void lfb_print_c(char c)
{
    if (y == numrows - 1)
    {
        screenbuff[index] = 0;
        char *tmpbuf;

        for (int j = 0; j < numrows * numcolumns; j++)
        {
            tmpbuf[j] = screenbuff[j];
            screenbuff[j] = 0;
            if (tmpbuf[j] == 0)
            {
                break;
            }
        }

        x = 0;
        y = 0;
        index = 0;
        int i;
        for (i = 0; i < numcolumns * numrows; i++)
        {
            if (tmpbuf[i] == '\n')
            {
                break;
            }
        }

        for (; i <= numcolumns * numrows; i++)
        {
            lfb_print_c(tmpbuf[i]);
            if (tmpbuf[i] == 0)
            {
                break;
            }
        }
    }

    // Font
    psf_t *font = (psf_t *)&_binary_font_psf_start;

    unsigned char *glyph = (unsigned char *)&_binary_font_psf_start +
                           font->headersize + (*((unsigned char *)(uint64_t)&c) < font->numglyph ? c : 0) * font->bytesperglyph;
    // calculate the offset on screen
    int offs = (y * font->height * pitch) + (x * (font->width + 1) * 4);
    // variables
    unsigned int i, j, line, mask, bytesperline = (font->width + 7) / 8;

    // Save to buffer
    screenbuff[index++] = c;

    // handle carrige return
    if (c == '\r')
    {
        x = 0;
        y++;
    }
    else if (c == '\n')
    {
        x = 0;
        y++;
    }
    else if (c == '\b')
    {
        x--;
        lfb_print_c(' ');
        x--;
    }
    else
    {
        // display a character
        for (j = 0; j < font->height; j++)
        {
            // display one row
            line = offs;
            mask = 1 << (font->width - 1);
            for (i = 0; i < font->width; i++)
            {
                // if bit set, we use white color, otherwise black
                *((unsigned int *)(lfb + line)) = ((int)*glyph) & mask ? 0xFFFFFF : 0;
                mask >>= 1;
                line += 4;
            }
            // adjust to next line
            glyph += bytesperline;
            offs += pitch;
        }
        x++;
    }
}