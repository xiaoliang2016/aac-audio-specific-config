#include <malloc.h>
#include <stddef.h>
#include <string.h>
#include "BitBuffer.h"

static UI8 bytemask[9] =
{
    0x00,
    0x01, 0x03, 0x07, 0x0F,
    0x1F, 0x3F, 0x7F, 0xFF
};

BitBuffer* InitiBitBuffer (UI8* data,I32 len)
{
    BitBuffer* bitbuf;

    if (len == 0L || data == NULL)
    {
        return NULL;
    }

    bitbuf = (BitBuffer*)malloc(sizeof(BitBuffer));
    if (bitbuf == NULL)
    {
        return NULL;
    }

    bitbuf->data    = data;
    bitbuf->length  = len;
    bitbuf->bytepos = 0L;
    bitbuf->bitleft = 8L;

    return bitbuf;
}
BitBuffer* CloseBitBuffer (BitBuffer* bitbuf)
{
    if (bitbuf != NULL)
    {
        if (bitbuf->data != NULL)
        {
            free (bitbuf->data);
            bitbuf->data = NULL;
        }
        free (bitbuf);
    }

    return NULL;
}
int  CheckBitBuffer (const BitBuffer* bitbuf, I32 bits)
{
    I32 bitpos  = 0L;
    I32 bytepos = 0L;

    if ((bitbuf == NULL) || (bitbuf->data == NULL) \
        || (bitbuf->length == bitbuf->bytepos)     \
        || (bits > 64) || (bits <0))
    {
        return FAIL;
    }

    bytepos = bitbuf->bitleft + bits;
    bytepos = bitbuf->bytepos + (bitpos / 8);
    bitpos  = bitpos % 8;

    if ((bytepos > bitbuf->length) \
        || (bytepos == bitbuf->length && bitpos != 0))
    {
        return FAIL;
    }

    return SUCCESS;
}
int  SkipSomeBits   (BitBuffer* buf, I32 bits)
{
    if (CheckBitBuffer(buf, bits) == FAIL)
    {
        return FAIL;
    }

    while (bits > 0)
    {
        if (bits >= buf->bitleft)
        {
            bits -= buf->bitleft;
            buf->bitleft  = 8;
            buf->bytepos += 1;
        }
        else
        {
            bits = 0;
            buf->bitleft -= bits;
        }
    }

    return SUCCESS;
}
int  GetDataFromBitBuffer (BitBuffer* buf, I32 bits, UI64*val)
{
    UI64 d    = 0ULL;

    if (CheckBitBuffer(buf, bits) == FAIL)
    {
        return FAIL;
    }

    while (bits > 0)
    {
        if(bits >= buf->bitleft)
        {
            d |= (buf->data[buf->bytepos] & bytemask[buf->bitleft])\
                << (bits - buf->bitleft);
            bits -= buf->bitleft;
            buf->bitleft  = 8;
            buf->bytepos += 1;
        }
        else
        {
            d |= ((buf->data[buf->bytepos] >> (buf->bitleft - bits)) & bytemask[bits]);
            bits = 0;
            buf->bitleft -= bits;
        }
    }

    *val = d;

    return SUCCESS;
}
int  WriteDataToBitBuffer (BitBuffer* buf, I32 bits, UI64 val)
{
    if (CheckBitBuffer(buf, bits) == FAIL)
    {
        return FAIL;
    }

    while (bits > 0)
    {
        if (bits >= buf->bitleft)
        {
            buf->data[buf->bytepos] |= ((val >> (bits - buf->bitleft)) & bytemask[buf->bitleft]);
            bits -= buf->bitleft;
            buf->bitleft  = 8;
            buf->bytepos += 1;
        }
        else
        {
            buf->data[buf->bytepos] |= (val & bytemask[bits]);
            bits = 0;
            buf->bitleft -= bits;
        }
    }

    return SUCCESS;
}
