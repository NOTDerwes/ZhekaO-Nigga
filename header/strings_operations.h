#ifndef STRINGS_OPERATIONS_H
#define STRINGS_OPERATIONS_H

#include <stddef.h>
#include <stdio.h>

int is_space(int symbol);
int endof_string(char symbol);
int has_alpha(const char* string);
const char* my_strchr(const char* string, int symbol);
size_t my_strlen(const char* string);
int straight_strcmp(const char* first, const char* second);
int reverse_strcmp(const char* first, const char* second);
int print_string(const char* string, FILE* stream);

#endif
