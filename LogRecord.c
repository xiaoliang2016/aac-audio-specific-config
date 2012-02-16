#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "LogRecord.h"

static const char* loglevelstr[5] = {"DEBUG", "INFOR", "WARNG", "ERROR", "FATAL"};

LogInfor* InitiLogger (char* logpath, LogLevel level, I32 maxlen)
{
    LogInfor* logger = NULL;
    char* path = NULL;
    FILE* fp = NULL;

    logger = (LogInfor*)malloc(sizeof(LogInfor));
    if (logger == NULL)
    {
        return NULL;
    }

    if (logpath == NULL)
    {
        fp = stdout;
        path = NULL;
    }
    else
    {
        path = (char*)malloc(strlen(logpath) + 1);
        strcpy(path, logpath);
        fp = fopen(path, "a+");
        if (fp == NULL)
        {
            free(path); path=NULL;
            return NULL;
        }
    }

    logger->fileptr = fp;
    logger->path    = path;
    logger->level   = level;
    logger->maxlen  = maxlen;
    if (sem_init(&logger->logging, 0, 1) != 0)
    {
        free(logger);logger=NULL;
        return NULL;
    }
    return logger;
}
LogInfor* CloseLogger (LogInfor* logger)
{
    if (logger != NULL)
    {
        fclose(logger->fileptr);logger->fileptr=NULL;
        free(logger->path);logger->path=NULL;
        sem_destroy(&logger->logging);
        free(logger);logger=NULL;
    }
    return NULL;
}
void RecordALog (LogInfor* logger, LogLevel level, const char* format, ...)
{
    time_t     rawTime;
    struct tm *timeInfo;

    if ((logger == NULL) || (logger->fileptr == NULL) || (sem_wait(&logger->logging) < 0)\
        || (level < logger->level))
    {
        return ;
    }

    if ((ftell(logger->fileptr) >= MAX_LOG_FILE_LENS) && (logger->fileptr != stdout))
    {
        ClrLogFile(logger);
        if (logger->fileptr == NULL)
        {
            return ;
        }
    }

    time (&rawTime);
    timeInfo = localtime (&rawTime);
    fprintf (logger->fileptr, "[%02d-%02d %02d:%02d:%02d] %s ", timeInfo->tm_mon+1, timeInfo->tm_mday\
             , timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, loglevelstr[level]);
    va_list args;
    va_start (args, format);
    vfprintf (logger->fileptr, format, args);
    va_end   (args);
    fprintf  (logger->fileptr, "\n");

    sem_post(&logger->logging);
}
void ClrLogFile (LogInfor* logger)
{
    fclose (logger->fileptr);
    logger->fileptr = fopen(logger->path, "w");
    if (logger->fileptr == NULL)
    {
        return ;
    }
    fclose (logger->fileptr);
    logger->fileptr = fopen(logger->path, "a+");
}
