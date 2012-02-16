#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "BitBuffer.h"

/// 传入AAC数据, 即FLV裸数据从第3个字节开始
int  AudioSpecificConfigParse(UI8* data, I32 length)
{
    int ret                     = SUCCESS;
    BitBuffer* bitbuf           = NULL;
    UI8  audio_object_type      = 0U;
    UI8  audio_object_type_ext  = 0U;
    UI8  sampling_frequency_ind = 0U;
    UI32 sampling_frequency     = 0U;
    UI8  channel_configuration  = 0U;

    bitbuf = InitiBitBuffer(data, length);
    if (bitbuf == NULL)
    {
        return FAIL;
    }

    ret = GetDataFromBitBuffer(bitbuf, 5, (I64*)&audio_object_type);
    if (ret == FAIL)
    {
        return FAIL;
    }

    if (audio_object_type == 0x1F)
    {
        ret = GetDataFromBitBuffer(bitbuf, 6, (I64*)&audio_object_type_ext);
        if (ret == FAIL)
        {
            return FAIL;
        }
    }

    ret = GetDataFromBitBuffer(bitbuf, 4, (I64*)&sampling_frequency_ind);
    if (ret == FAIL)
    {
        return FAIL;
    }
    if (sampling_frequency_ind == 0x0F)
    {
        ret = GetDataFromBitBuffer(bitbuf, 24, (I64*)&sampling_frequency);
        if (ret == FAIL)
        {
            return FAIL;
        }
    }

}
