#include <ctype.h>
#include "../header/strings_opertions.h"

bool is_space(char symb) {
    return symb == ' ' ||
           symb == '\t' ||
           symb == '\n' ||
           symb == '\v' ||
           symb == '\f' ||
           symb == '\r';
}

bool endof_string(char symb) {
    return symb == '\0' || symb == '\n';
}

const char *my_strchr(const char *str, int symbol) {
    assert(str);

    while (!endof_string(*str)) {
        if (*str == symbol) {
            return str;
        }
        str++;
    }
    return NULL;
}


size_t my_strlen(const char *str) {
    assert(str);

    size_t size = 0;
    while (!endof_string(*str)) {
        size ++;
        str++;
    }
    return size;
}

int strcmp(const char* s1, const char* s2, flag = FALSE) {
    while(!endof_string(*s1) && !endof_string(*s2)) {
        if (!
    }
}
