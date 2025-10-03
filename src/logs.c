#include <sys/stat.h>

#include "../header/logs.h"

static LogsErrno ensure_directory(const char* path) {
    if (path == NULL) {
        return LogsNullPtr;
    }

    struct stat st = {};
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) ? LogsOk : LogsDirConflict;
    }

    if (mkdir(path, 0755) == -1) {
        return LogsDirCreateFail;
    }

    return LogsOk;
}

const char* LogsErrMsg(LogsErrno error) {
    switch (error) {
        case LogsOk:            return "Logging OK";
        case LogsNullPtr:       return "NULL pointer passed to logging routine";
        case LogsDirConflict:   return "Log directory path exists but is not a directory";
        case LogsDirCreateFail: return "Failed to create log directory";
        case LogsFileOpenFail:  return "Failed to open log file";
        default:                return "Unknown logging error";
    }
}

LogsErrno init_logs(logs_t* log_info) {
    if (log_info == NULL) {
        return LogsNullPtr;
    }

    log_info->owns_file = false;
    log_info->dir = "./logs";
    log_info->path = "./logs/app.log";
    log_info->file = stderr;

    LogsErrno dir_status = ensure_directory(log_info->dir);
    if (dir_status != LogsOk) {
        return dir_status;
    }

    log_info->file = fopen(log_info->path, "a+");
    if (log_info->file == NULL) {
        log_info->file = stderr;
        return LogsFileOpenFail;
    }

    log_info->owns_file = true;
    return LogsOk;
}

void destruct_logs(logs_t* log_info) {
    if (log_info == NULL) {
        return;
    }

    log_info->dir = NULL;
    log_info->path = NULL;

    if (log_info->file != NULL && log_info->owns_file) {
        fclose(log_info->file);
    }
    log_info->file = NULL;
    log_info->owns_file = false;
}

void show_logs(logs_t* log_info, FILE* stream) {
    if (log_info == NULL || stream == NULL) {
        return;
    }

    fprintf(stream, "Log File Info:\n");

    fprintf(stream, "\tDirectory: ");
    if (log_info->dir == NULL) {
        fprintf(stream, "-\n");
    }
    else {
        fprintf(stream, "%s\n", log_info->dir);
    }

    fprintf(stream, "\tPath: ");
    if (log_info->path == NULL) {
        fprintf(stream, "-\n");
    }
    else {
        fprintf(stream, "%s\n", log_info->path);
    }

    fprintf(stream, "\tOwner: %s\n", log_info->owns_file ? "Program" : "External");

    fprintf(stream, "\tFile Stream [%p]: ", (void*) log_info->file);
    if (log_info->file == NULL) {
        fprintf(stream, "Closed\n");
    }
    else {
        fprintf(stream, "Opened\n");
    }
}
