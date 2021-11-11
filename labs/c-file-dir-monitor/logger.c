#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

int sLog = 0;

int initLogger(char *logType) {
    printf("Initializing Logger on: %s\n", logType);
    if (strcmp(logType, "syslog") == 0){
        sLog = 1;
    }else if (strcmp(logType, "stdout") == 0 || strlen(logType) == 0){
        sLog = 0;
    }else{
        printf("Wrong arguments, please insert valid arguments");
    }
    return 0;
}

int infof(const char *format, ...) {
    va_list arg;
    va_start(arg, format);

    if (sLog == 1){
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }else if (sLog == 0){
        vprintf(format, arg);
    }

    va_end(arg);
    return 0;
}

int warnf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);

    if (sLog == 1){
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }else if (sLog == 0){
        vprintf(format, arg);
    }

    va_end(arg);
    return 0;
}

int errorf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);

    if (sLog == 1){
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }else if (sLog == 0){
        vprintf(format, arg);
    }

    va_end(arg);
    return 0;
}
int panicf(const char *format, ...) {
    va_list arg;
    va_start(arg, format);

    if (sLog == 1){
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }else if(sLog == 0){
        vprintf(format, arg);
    }

    va_end(arg);
    return 0;
}

