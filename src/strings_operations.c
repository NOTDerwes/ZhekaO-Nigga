#include <ctype.h>
#include <assert.h>
#include <stdio.h>

#include "../header/strings_operations.h"

int is_space(char symb) {
    return symb == ' ' ||
           symb == '\t' ||
           symb == '\n' ||
           symb == '\v' ||
           symb == '\f' ||
           symb == '\r';
}

int endof_string(char symb) {
    return symb == '\0' || symb == '\n';
}

int has_alpha(char* s) {
    if (!s) {
        return -fprintf(stderr, "ERROR IN FUNC \"has_alpha\": GOT NULL POINTER\n");
    }

    while (!endof_string(*s)) {
        if (isalpha(*s)) {
            return 1;
        }
        s++;
    }
    return 0;
}

const char* my_strchr(const char *str, int symbol) {
    assert(str);

    while (!endof_string(*str)) {
        if (*str == symbol) {
            return str;
        }
        str++;
    }
    return 0;
}


size_t my_strlen(const char* str) {
    assert(str);

    size_t size = 0;
    while (!endof_string(*str)) {
        size ++;
        str++;
    }
    return size;
}



int straight_strcmp(char* s1, char* s2) {
    if (s1 == NULL || s2 == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"my_strcmp\": NULL POINTER GOT\n");
    }

    char* ptr1 = s1;
    char* ptr2 = s2;

    while (!isalpha(*ptr1) && !endof_string(*ptr1)) ptr1++;
    while (!isalpha(*ptr2) && !endof_string(*ptr2)) ptr2++;

    while (!endof_string(*ptr1) && !endof_string(*ptr2)) {
        if (tolower(*ptr1) < tolower(*ptr2)) return -1;
        else if (tolower(*ptr1) > tolower(*ptr2)) return 1;
        ptr1++;
        ptr2++;
    }
    if (tolower(*ptr1) < tolower(*ptr2)) return -1;
    else if (tolower(*ptr1) > tolower(*ptr2)) return 1;
    else return 0;
}

int reverse_strcmp(char* s1, char* s2) {
    if (s1 == NULL || s2 == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"my_strcmp\": NULL POINTER GOT\n");
    }

    char* ptr1 = s1;
    char* ptr2 = s2;

    while (!endof_string(*ptr1)) ptr1++;
    while (!endof_string(*ptr2)) ptr2++;
    ptr1--;
    ptr2--;

    while ((!isalpha(*ptr1)) && (ptr1 > s1)) ptr1--;
    while ((!isalpha(*ptr2)) && (ptr2 > s2)) ptr2--;

    while (ptr1 > s1 && ptr2 > s2) {
        if (tolower(*ptr1) < tolower(*ptr2)) return -1;
        else if (tolower(*ptr1) > tolower(*ptr2)) return 1;
        ptr1--;
        ptr2--;
    }

    if (tolower(*ptr1) < tolower(*ptr2)) return -1;
    else if (tolower(*ptr1) > tolower(*ptr2)) return 1;
    else return 0;
}
