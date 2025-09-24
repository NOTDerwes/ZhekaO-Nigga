#ifndef STRINGS_OPERATIONS_H
#define STRINGS_OPERATIONS_H

#define DEFAULT_SIZE 512
#define SIZEUP 2
#define TRUE 1
#define FALSE 0

int is_space(char symb);
int endof_string(char symb);
int has_alpha(char* s);
const char *my_strchr(const char *str, int symbol);
size_t my_strlen(const char *str);
int straight_strcmp(char* s1, char* s2);
int reverse_strcmp(char* s1, char* s2);


#endif
