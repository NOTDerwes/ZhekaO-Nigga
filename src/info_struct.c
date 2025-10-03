#include <stdlib.h>
#include <string.h>

#include "../header/info_struct.h"

static void log_info_state(logs_t* log_info, InfoErrno error, const information* info) {
    if (log_info == NULL || log_info->file == NULL) {
        return;
    }

    WRITE_ERR(InfoErrMsg(error), log_info->file);
    show_info(info, log_info->file);
}

const char* InfoErrMsg(InfoErrno error) {
    switch (error) {
        case InfoOk:        return "No error";
        case NullInfoPtr:   return "Information structure pointer is NULL";
        case MissInpF:      return "Missing input file path";
        case MissOutF:      return "Missing output file path";
        case NullOutFPtr:   return "Failed to open output file";
        case NoStat:        return "Unable to obtain input file statistics";
        case NullBuffPtr:   return "Failed to allocate buffer";
        case WrongSize:     return "Read size does not match file size";
        default:            return "Unknown error";
    }
}

InfoErrno show_info(const information* info, FILE* stream) {
    if (stream == NULL) {
        stream = stderr;
    }

    fprintf(stream, "Info [%p]:\n", (const void*) info);

    if (info == NULL) {
        fprintf(stream, "\tNo information provided\n");
        return InfoOk;
    }

    fprintf(stream, "\tInput File:\n");
    fprintf(stream, "\t\tPath [%p]: %s\n",
            (const void*) info->input_file_path,
            info->input_file_path != NULL ? info->input_file_path : "-");

    fprintf(stream, "\t\tStats: ");
    if (info->inp_file_stat.st_mode == 0 && info->inp_file_stat.st_size == 0) {
        fprintf(stream, "Not available\n");
    } else {
        fprintf(stream, "size=%lld bytes\n", (long long) info->inp_file_stat.st_size);
    }

    fprintf(stream, "\tOutput File Path: %s\n",
            info->output_file_path != NULL ? info->output_file_path : "-");

    fprintf(stream, "\tBuffer [%p]: %s\n",
            (const void*) info->buffer,
            info->buffer != NULL ? info->buffer : "<empty>");

    fprintf(stream, "\tOutput Stream [%p]: %s\n",
            (void*) info->output_stream,
            info->output_stream != NULL ? "Opened" : "Closed");

    return InfoOk;
}

InfoErrno verify_info(information* info) {
    if (info == NULL) {
        return NullInfoPtr;
    }
    if (info->input_file_path == NULL || info->input_file_path[0] == '\0') {
        return MissInpF;
    }
    if (info->output_file_path == NULL || info->output_file_path[0] == '\0') {
        return MissOutF;
    }
    if (info->output_stream == NULL) {
        return NullOutFPtr;
    }

    if (stat(info->input_file_path, &(info->inp_file_stat)) == -1) {
        memset(&(info->inp_file_stat), 0, sizeof(struct stat));
        return NoStat;
    }

    return InfoOk;
}

InfoErrno init_info(information* info, logs_t* log_info) {
    if (info == NULL) {
        return NullInfoPtr;
    }

    info->input_file_path = "text.txt";
    info->output_file_path = "output.txt";
    info->buffer = NULL;
    memset(&(info->inp_file_stat), 0, sizeof(struct stat));

    info->output_stream = fopen(info->output_file_path, "w");
    if (info->output_stream == NULL) {
        log_info_state(log_info, NullOutFPtr, info);
        return NullOutFPtr;
    }

    InfoErrno status = verify_info(info);
    if (status != InfoOk) {
        log_info_state(log_info, status, info);
        fclose(info->output_stream);
        info->output_stream = NULL;
    }

    return status;
}

void destruct_info(information* info) {
    if (info == NULL) {
        return;
    }

    info->input_file_path = NULL;
    info->output_file_path = NULL;

    free(info->buffer);
    info->buffer = NULL;

    if (info->output_stream != NULL) {
        fclose(info->output_stream);
        info->output_stream = NULL;
    }

    memset(&(info->inp_file_stat), 0, sizeof(struct stat));
}
