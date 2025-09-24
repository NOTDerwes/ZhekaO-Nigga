#ifndef SQVRT_STRINGS_H
#define SQVRT_STRINGS_H

#define MAX_STRING 14
#define AMOUNT 2

int get_sizes(FILE* stream);
int read_text(char strings_array[][MAX_STRING], FILE* stream);
int swap_strings(char strings_array[][MAX_STRING], size_t s1_ptr, size_t s2_ptr);
int print_strings(char strings_array[][MAX_STRING]);

#endif
