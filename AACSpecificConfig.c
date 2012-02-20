#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "BitBuffer.h"

/// @brief 解析audio_object_type
int ParseAudioObjectType (BitBuffer* bitbuff, UI8*  audio_object_type)
{
    int ret = SUCCESS;
    ret = GetDataFromBitBuffer(bitbuff, 5, (UI64*)audio_object_type);
    if (ret == FAIL)
    {
        return FAIL;
    }
    if (*audio_object_type == 0x1F)
    {
        UI8 audio_object_type_ext;
        ret = GetDataFromBitBuffer(bitbuf, 6, (UI64*)&audio_object_type_ext);
        if (ret == FAIL)
        {
            return FAIL;
        }
        *audio_object_type = 32 + audio_object_type_ext;
    }
    return SUCCESS;
}

/// @brief 解析AudioSpecificConfig
int ParseAudioSpecificConfig(UI8* data, I32 length)
{
    int ret                         = SUCCESS;
    BitBuffer* bitbuf               = NULL;

    UI8  audio_object_type          = 0U;
    UI8  sampling_frequency_index   = 0U;
    UI32 sampling_frequency         = 0UL;
    UI8  channel_configuration      = 0UL;
    I8   sbr_present_flag           = -1;
    I8   ps_present_flag            = -1;
    UI8  ext_audio_object_type      = 0U;
    UI8  ext_sampling_frequency_ind = 0U;
    UI32 ext_sampling_frequency     = 0UL;
    UI8  ext_channel_configuration  = 0U;

    /// Create BitBuffer
    bitbuf = InitiBitBuffer(data, length);
    if (bitbuf == NULL)
    {
        return FAIL;
    }

    ret = ParseAudioObjectType(bitbuf, &audio_object_type);
    if (ret == FAIL)
    {
        return FAIL;
    }

    ret = GetDataFromBitBuffer(bitbuf, 4, (UI64*)&sampling_frequency_index);
    /// @todo

    if (sampling_frequency_index == 0xF)
    {
        ret = GetDataFromBitBuffer(bitbuf, 24, (UI64*)&sampling_frequency);
        /// @todo
    }

    ret = GetDataFromBitBuffer(bitbuf, 4, (UI64*)&channel_configuration);
    /// @todo

    if (audio_object_type == 5 || audio_object_type == 29)
    {
        ext_audio_object_type = 5;
        sbr_present_flag      = 1;
        if (audio_object_type == 29)
        {
            ps_present_flag   = 1;
        }
        ret = GetDataFromBitBuffer(bitbuf, 4, (UI64*)&ext_sampling_frequency_ind);
        /// @todo
        if (ext_sampling_frequency_ind == 0xF)
        {
            ret = GetDataFromBitBuffer(bitbuf, 24, (UI64*)&ext_sampling_frequency);
            /// @todo
            ret = ParseAudioObjectType(bitbuf, &audio_object_type);
            /// @todo
            if (audio_object_type == 0x16)
            {
                ret = GetDataFromBitBuffer(bitbuf, 4, (UI64*)&)
            }
        }
    }

}
