/// @file    LogRecord.h
/// @brief   LogRecord.h Operations about recording log
/// @author  birdyfj@gmail.com
/// @version 1.0
/// @date    2012-02-08
#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <stdio.h>
#include <semaphore.h>

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

/// @brief Log level defined
typedef enum LogLevel
{
    LOG_DEBUG = 0,  ///< Debug message
    LOG_INFOR,      ///< Information
    LOG_WARNG,      ///< Warning message
    LOG_ERROR,      ///< Common error
    LOG_FATAL       ///< Fatal error
}LogLevel;
/// @brief Log information
typedef struct LogInfor
{
    FILE*           fileptr;    ///< Log file pointer
    I8*             path;       ///< Log file path
    LogLevel        level;      ///< Current file level
    I32             maxlen;     ///< Maximum log file length
    sem_t           logging;    ///< Is logging
}LogInfor;

#define DEFAULT_LOG_LEVEL  LOG_DEBUG
#define MAX_LOG_FILE_LENS  0x200000L

/// @brief Initialize logger
LogInfor* InitiLogger (char* logpath, LogLevel level, I32 maxlen);
/// @brief Close logger
LogInfor* CloseLogger (LogInfor* logger);
/// @brief Record a piece of log
void RecordALog (LogInfor* logger, LogLevel level, const char* format, ...);
/// @brief Clear log file
void ClrLogFile (LogInfor* logger);

#endif // LOGRECORD_H
