#ifndef LOGS_H
#define LOGS_H

#include <stdbool.h>
#include <stdio.h>
#define WRITE_ERR(fmt, stream)                                                                  \
do {                                                                                            \
    fprintf(stream, "%s %s ERROR IN %s %d: %s\n", __DATE__, __TIME__, __FILE__, __LINE__, fmt); \
} while (0)

typedef enum {
    LogsOk = 0,
    LogsNullPtr,
    LogsDirConflict,
    LogsDirCreateFail,
    LogsFileOpenFail,
} LogsErrno;

const char* LogsErrMsg(LogsErrno error);

typedef struct {
    const char* dir;
    const char* path;
    FILE* file;
    bool owns_file;
} logs_t;

LogsErrno init_logs(logs_t* log_info);
void destruct_logs(logs_t* log_info);
void show_logs(logs_t* log_info, FILE* stream);

#endif
