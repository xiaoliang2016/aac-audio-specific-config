/// @file     BitBuffer.h
/// @brief    Bit buffer
/// @author   birdyfj@gmail.com
/// @version  1.0
/// @date     2012-02-08

#ifndef BITBUFFER_H
#define BITBUFFER_H

#ifndef _DEFINED_DATA_TYPE_
#define _DEFINED_DATA_TYPE_
typedef char               I8;
typedef short              I16;
typedef long               I32;
typedef long long          I64;
typedef unsigned char      UI8;
typedef unsigned short     UI16;
typedef unsigned long      UI32;
typedef unsigned long long UI64;

#define SUCCESS  0
#define FAIL    -1
#endif/*_DEFINED_DATA_TYPE_*/

/// @brief Bit buffer
typedef struct BitBuffer
{
    UI8* data;      ///< BitBuffer data
    I32  length;    ///< BitBuffer data length
    I32  bytepos;   ///< Current read byte
    I32  bitleft;   ///< Current byte unread bits number
}BitBuffer;

/// @brief Initialize bit buffer
/// @note  If you wanna to initialize a bit buffer for writing data, please memset the data before\n
/// calling this method.
BitBuffer* InitiBitBuffer (UI8* data, I32 length);
/// @brief Close and destroy bit buffer
BitBuffer* CloseBitBuffer (BitBuffer* bitbuf);
/// @brief Check if bit buffer is availalbe before Get or Put data
int  CheckBitBuffer (const BitBuffer* bitbuf, I32 bits);
/// @brief Skip some bits
int  SkipSomeBits (BitBuffer* buf, I32 bits);
/// @brief Get data from bit buffer
int  GetDataFromBitBuffer (BitBuffer* buf, I32 bits, UI64*val);
/// @brief Pub data to bit buffer
int  WriteDataToBitBuffer (BitBuffer* buf, I32 bits, UI64 val);

#endif/*BITBUFFER_H*/
