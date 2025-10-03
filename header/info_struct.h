#ifndef INFO_STRUCT_H
#define INFO_STRUCT_H

#include <stdio.h>
#include <sys/stat.h>

#include "logs.h"

typedef enum {
    InfoOk = 0,
    NullInfoPtr = 1,
    MissInpF = 2,
    MissOutF = 3,
    NullOutFPtr = 4,
    NoStat = 5,
    NullBuffPtr = 6,
    WrongSize = 7,
} InfoErrno;

typedef struct {
    const char* input_file_path;
    const char* output_file_path;
    char* buffer;
    FILE* output_stream;
    struct stat inp_file_stat;
} information;

const char* InfoErrMsg(InfoErrno error);
InfoErrno show_info(const information* info, FILE* stream);
InfoErrno verify_info(information* info);
InfoErrno init_info(information* info, logs_t* log_info);
void destruct_info(information* info);

#endif
