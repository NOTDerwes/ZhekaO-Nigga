#include <stdio.h>
#include <stdlib.h>

#include "info_struct.h"
#include "logs.h"
#include "ptr_strings.h"

static int initialise(information* info, text_t* text, logs_t* log_info) {
    if (info == NULL || text == NULL || log_info == NULL) {
        fprintf(stderr, "initialise: invalid arguments\n");
        return -1;
    }

    LogsErrno log_status = init_logs(log_info);
    if (log_status != LogsOk) {
        fprintf(stderr, "Logging initialisation warning: %s\n", LogsErrMsg(log_status));
    }

    TextErrno text_status = init_text(text);
    if (text_status != TextOk) {
        fprintf(stderr, "Text initialisation failed: %s\n", TextErrMsg(text_status));
        return -1;
    }

    InfoErrno info_status = init_info(info, log_info);
    if (info_status != InfoOk) {
        return -1;
    }

    return 0;
}

static void teardown(information* info, text_t* text, logs_t* log_info) {
    destruct_text(text);
    destruct_info(info);
    destruct_logs(log_info);
}

int main(void) {
    information info = {};
    text_t text = {};
    logs_t log_info = {};

    if (initialise(&info, &text, &log_info) != 0) {
        teardown(&info, &text, &log_info);
        return EXIT_FAILURE;
    }

    TextErrno text_status = read_text(&text, &info, &log_info);
    if (text_status != TextOk) {
        teardown(&info, &text, &log_info);
        return EXIT_FAILURE;
    }

    text_status = make_sortings(&text, &info, &log_info);
    if (text_status != TextOk) {
        teardown(&info, &text, &log_info);
        return EXIT_FAILURE;
    }

    teardown(&info, &text, &log_info);
    return EXIT_SUCCESS;
}
