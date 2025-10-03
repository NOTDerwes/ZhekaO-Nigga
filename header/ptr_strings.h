#ifndef PTR_STRINGS_H
#define PTR_STRINGS_H

#include <stddef.h>
#include <stdio.h>

#include "info_struct.h"
#include "logs.h"

#define DEFAULT_SIZE 5

typedef enum {
    TextOk = 0,
    TextInvalidArgs,
    TextInfoFailure,
    TextNullArray,
    TextAllocFail,
    TextIoError,
} TextErrno;

typedef struct {
    char* ptr;
    size_t size;
} string;

typedef struct {
    size_t size;
    string* str_array;
} text_t;

const char* TextErrMsg(TextErrno error);

TextErrno read_text(text_t* text, information* info, logs_t* log_info);
TextErrno swap_strings(text_t* text, size_t s1_ptr, size_t s2_ptr, logs_t* log_info);
TextErrno print_strings(const text_t* text, logs_t* log_info);
TextErrno write_strings(const text_t* text, FILE* stream);
int straight_comparator(const void* par1, const void* par2);
int reverse_comparator(const void* par1, const void* par2);
TextErrno bubble_sort_strings(text_t* text, int is_backward, logs_t* log_info);
TextErrno make_sortings(text_t* text, information* info, logs_t* log_info);
TextErrno init_text(text_t* text);
TextErrno show_text(const text_t* text, FILE* stream);
void destruct_text(text_t* text);

#endif
