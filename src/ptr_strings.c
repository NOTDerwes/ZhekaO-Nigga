#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/ptr_strings.h"
#include "../header/strings_operations.h"
#include "../header/info_struct.h"

static void log_info_failure(logs_t* log_info, InfoErrno error, const information* info) {
    if (log_info == NULL || log_info->file == NULL) {
        return;
    }

    WRITE_ERR(InfoErrMsg(error), log_info->file);
    show_info(info, log_info->file);
}

static void log_text_failure(logs_t* log_info, TextErrno error, const char* context) {
    if (log_info == NULL || log_info->file == NULL) {
        return;
    }

    WRITE_ERR(TextErrMsg(error), log_info->file);
    if (context != NULL) {
        fprintf(log_info->file, "\tContext: %s\n", context);
    }
}

static TextErrno allocate_string_slots(text_t* text, size_t count, logs_t* log_info) {
    if (text == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "allocate_string_slots");
        return TextInvalidArgs;
    }

    free(text->str_array);
    text->str_array = NULL;
    text->size = 0;

    if (count == 0) {
        return TextOk;
    }

    text->str_array = (string*) calloc(count, sizeof(string));
    if (text->str_array == NULL) {
        log_text_failure(log_info, TextAllocFail, "allocate_string_slots");
        return TextAllocFail;
    }

    text->size = count;
    return TextOk;
}

static size_t count_meaningful_strings(char* buffer, size_t size) {
    if (buffer == NULL) {
        return 0;
    }

    size_t count = 0;
    size_t line_start = 0;

    for (size_t i = 0; i <= size; ++i) {
        if (i == size || buffer[i] == '\n' || buffer[i] == '\0') {
            if (has_alpha(buffer + line_start) > 0) {
                ++count;
            }
            line_start = i + 1;
        }
    }

    return count;
}

static TextErrno fill_string_slots(text_t* text, char* buffer, size_t size) {
    if (text == NULL || buffer == NULL) {
        return TextInvalidArgs;
    }

    size_t pos = 0;
    size_t line_start = 0;

    for (size_t i = 0; i <= size && pos < text->size; ++i) {
        if (i == size || buffer[i] == '\n' || buffer[i] == '\0') {
            if (has_alpha(buffer + line_start) > 0) {
                text->str_array[pos].ptr = buffer + line_start;
                text->str_array[pos].size = i >= line_start ? i - line_start : 0;
                ++pos;
            }
            line_start = i + 1;
        }
    }

    if (pos < text->size) {
        text->size = pos;
    }

    return TextOk;
}

const char* TextErrMsg(TextErrno error) {
    switch (error) {
        case TextOk:            return "No text error";
        case TextInvalidArgs:   return "Invalid arguments provided";
        case TextInfoFailure:   return "Information verification failed";
        case TextNullArray:     return "Text array is not initialised";
        case TextAllocFail:     return "Failed to allocate text resources";
        case TextIoError:       return "I/O error during text processing";
        default:                return "Unknown text error";
    }
}

TextErrno init_text(text_t* text) {
    if (text == NULL) {
        return TextInvalidArgs;
    }

    text->size = 0;
    text->str_array = NULL;
    return TextOk;
}

void destruct_text(text_t* text) {
    if (text == NULL) {
        return;
    }

    free(text->str_array);
    text->str_array = NULL;
    text->size = 0;
}

TextErrno read_text(text_t* text, information* info, logs_t* log_info) {
    if (text == NULL || info == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "read_text");
        return TextInvalidArgs;
    }

    InfoErrno info_error = verify_info(info);
    if (info_error != InfoOk) {
        log_info_failure(log_info, info_error, info);
        return TextInfoFailure;
    }

    FILE* stream = fopen(info->input_file_path, "rb");
    if (stream == NULL) {
        log_info_failure(log_info, MissInpF, info);
        return TextIoError;
    }

    free(info->buffer);
    info->buffer = NULL;

    size_t size = (size_t) info->inp_file_stat.st_size;
    info->buffer = (char*) calloc(size + 1, sizeof(char));
    if (info->buffer == NULL) {
        fclose(stream);
        log_text_failure(log_info, TextAllocFail, "read_text::buffer");
        return TextAllocFail;
    }

    if (size > 0) {
        size_t read = fread(info->buffer, sizeof(char), size, stream);
        if (read != size) {
            fclose(stream);
            log_text_failure(log_info, TextIoError, "read_text::fread");
            free(info->buffer);
            info->buffer = NULL;
            return TextIoError;
        }
    }

    info->buffer[size] = '\0';
    fclose(stream);

    size_t strings_count = count_meaningful_strings(info->buffer, size);
    TextErrno allocation_status = allocate_string_slots(text, strings_count, log_info);
    if (allocation_status != TextOk) {
        return allocation_status;
    }

    TextErrno fill_status = fill_string_slots(text, info->buffer, size);
    if (fill_status != TextOk) {
        log_text_failure(log_info, fill_status, "read_text::fill");
        return fill_status;
    }

    return TextOk;
}

TextErrno swap_strings(text_t* text, size_t first, size_t second, logs_t* log_info) {
    if (text == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "swap_strings");
        return TextInvalidArgs;
    }
    if (text->str_array == NULL) {
        log_text_failure(log_info, TextNullArray, "swap_strings");
        return TextNullArray;
    }
    if (first >= text->size || second >= text->size) {
        log_text_failure(log_info, TextInvalidArgs, "swap_strings::range");
        return TextInvalidArgs;
    }

    string tmp = text->str_array[first];
    text->str_array[first] = text->str_array[second];
    text->str_array[second] = tmp;

    return TextOk;
}

TextErrno print_strings(const text_t* text, logs_t* log_info) {
    if (text == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "print_strings");
        return TextInvalidArgs;
    }
    if (text->str_array == NULL) {
        log_text_failure(log_info, TextNullArray, "print_strings");
        return TextNullArray;
    }

    for (size_t i = 0; i < text->size; ++i) {
        if (text->str_array[i].ptr == NULL || text->str_array[i].size == 0) {
            continue;
        }
        print_string(text->str_array[i].ptr, stdout);
    }

    return TextOk;
}

TextErrno write_strings(const text_t* text, FILE* stream) {
    if (text == NULL) {
        return TextInvalidArgs;
    }
    if (text->str_array == NULL) {
        return TextNullArray;
    }
    if (stream == NULL) {
        WRITE_ERR(TextErrMsg(TextIoError), stderr);
        return TextIoError;
    }

    for (size_t i = 0; i < text->size; ++i) {
        if (text->str_array[i].ptr == NULL || text->str_array[i].size == 0) {
            continue;
        }

        const char* str_ptr = text->str_array[i].ptr;
        while (*str_ptr != '\n' && *str_ptr != '\0') {
            if (fputc(*str_ptr, stream) == EOF) {
                return TextIoError;
            }
            ++str_ptr;
        }
        if (fputc('\n', stream) == EOF) {
            return TextIoError;
        }
    }

    if (fprintf(stream, "===========================================================================\n") < 0) {
        return TextIoError;
    }

    return TextOk;
}

int straight_comparator(const void* lhs, const void* rhs) {
    const string first = *(const string*) lhs;
    const string second = *(const string*) rhs;
    return straight_strcmp(first.ptr, second.ptr);
}

int reverse_comparator(const void* lhs, const void* rhs) {
    const string first = *(const string*) lhs;
    const string second = *(const string*) rhs;
    return reverse_strcmp(first.ptr, second.ptr);
}

TextErrno bubble_sort_strings(text_t* text, int is_backward, logs_t* log_info) {
    if (text == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "bubble_sort_strings");
        return TextInvalidArgs;
    }
    if (text->str_array == NULL) {
        log_text_failure(log_info, TextNullArray, "bubble_sort_strings");
        return TextNullArray;
    }

    for (size_t r = 0; r < text->size; ++r) {
        for (size_t l = 1; l < text->size - r; ++l) {
            int cmp = is_backward ?
                reverse_strcmp(text->str_array[l].ptr, text->str_array[l - 1].ptr) :
                straight_strcmp(text->str_array[l].ptr, text->str_array[l - 1].ptr);

            if (cmp < 0) {
                TextErrno swap_status = swap_strings(text, l, l - 1, log_info);
                if (swap_status != TextOk) {
                    return swap_status;
                }
            }
        }
    }

    return TextOk;
}

TextErrno make_sortings(text_t* text, information* info, logs_t* log_info) {
    if (text == NULL || info == NULL) {
        log_text_failure(log_info, TextInvalidArgs, "make_sortings");
        return TextInvalidArgs;
    }
    if (text->str_array == NULL) {
        log_text_failure(log_info, TextNullArray, "make_sortings");
        return TextNullArray;
    }
    if (info->output_stream == NULL) {
        log_info_failure(log_info, NullOutFPtr, info);
        return TextIoError;
    }

    qsort(text->str_array, text->size, sizeof(string), straight_comparator);

    TextErrno write_status = write_strings(text, info->output_stream);
    if (write_status != TextOk) {
        log_text_failure(log_info, write_status, "make_sortings::straight");
        return write_status;
    }

    qsort(text->str_array, text->size, sizeof(string), reverse_comparator);

    write_status = write_strings(text, info->output_stream);
    if (write_status != TextOk) {
        log_text_failure(log_info, write_status, "make_sortings::reverse");
        return write_status;
    }

    return TextOk;
}

TextErrno show_text(const text_t* text, FILE* stream) {
    if (stream == NULL) {
        stream = stderr;
    }

    fprintf(stream, "Text [%p]:\n", (const void*) text);

    if (text == NULL) {
        fprintf(stream, "\tNo text provided\n");
        return TextInvalidArgs;
    }

    fprintf(stream, "\tStrings count: %zu\n", text->size);
    fprintf(stream, "\tStrings array [%p]:\n", (const void*) text->str_array);

    if (text->str_array == NULL) {
        fprintf(stream, "\t\tNot initialised\n");
        return TextNullArray;
    }

    for (size_t i = 0; i < text->size; ++i) {
        if (text->str_array[i].ptr != NULL) {
            fprintf(stream, "\t\t[%zu] %.*s\n", i, (int) text->str_array[i].size, text->str_array[i].ptr);
        }
    }

    return TextOk;
}
