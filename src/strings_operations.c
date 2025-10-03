#include <ctype.h>
#include <stdio.h>

#include "../header/strings_operations.h"

int is_space(int symbol) {
    return symbol == ' ' || symbol == '\t' || symbol == '\n' ||
           symbol == '\v' || symbol == '\f' || symbol == '\r';
}

int endof_string(char symbol) {
    return symbol == '\0' || symbol == '\n';
}

int has_alpha(const char* string) {
    if (string == NULL) {
        fprintf(stderr, "has_alpha: received NULL pointer\n");
        return -1;
    }

    while (!endof_string(*string)) {
        if (isalpha((unsigned char) *string)) {
            return 1;
        }
        ++string;
    }

    return 0;
}

const char* my_strchr(const char* string, int symbol) {
    if (string == NULL) {
        return NULL;
    }

    while (!endof_string(*string)) {
        if (*string == symbol) {
            return string;
        }
        ++string;
    }

    return NULL;
}

size_t my_strlen(const char* string) {
    if (string != NULL) {
        return 0;
    }

    size_t length = 0;
    while (!endof_string(*string)) {
        ++length;
        ++string;
    }

    return length;
}

static int char_compare_forward(const char** first, const char** second) {
    while (1) {
        while (!endof_string(**first) && !isalpha((unsigned char) **first)) {
            ++(*first);
        }
        while (!endof_string(**second) && !isalpha((unsigned char) **second)) {
            ++(*second);
        }

        int left = endof_string(**first) ? 0 : tolower((unsigned char) **first);
        int right = endof_string(**second) ? 0 : tolower((unsigned char) **second);

        if (left != right) {
            return left < right ? -1 : 1;
        }
        if (left == 0) {
            return 0;
        }

        ++(*first);
        ++(*second);
    }
}

int straight_strcmp(const char* first, const char* second) {
    if (first == NULL || second == NULL) {
        fprintf(stderr, "straight_strcmp: NULL pointer received\n");
        return 0;
    }

    const char* lhs = first;
    const char* rhs = second;
    return char_compare_forward(&lhs, &rhs);
}

static int char_compare_backward(const char** first, const char** second, const char* begin_first, const char* begin_second) {
    while (1) {
        while (*first >= begin_first && !isalpha((unsigned char) **first) && !endof_string(**first)) {
            --(*first);
        }
        while (*second >= begin_second && !isalpha((unsigned char) **second) && !endof_string(**second)) {
            --(*second);
        }

        int left = (*first < begin_first || endof_string(**first)) ? 0 : tolower((unsigned char) **first);
        int right = (*second < begin_second || endof_string(**second)) ? 0 : tolower((unsigned char) **second);

        if (left != right) {
            return left < right ? -1 : 1;
        }
        if (left == 0) {
            return 0;
        }

        --(*first);
        --(*second);
    }
}

int reverse_strcmp(const char* first, const char* second) {
    if (first == NULL || second == NULL) {
        fprintf(stderr, "reverse_strcmp: NULL pointer received\n");
        return 0;
    }

    const char* end_first = first;
    const char* end_second = second;

    while (!endof_string(*end_first)) {
        ++end_first;
    }
    while (!endof_string(*end_second)) {
        ++end_second;
    }

    if (end_first > first) {
        --end_first;
    }
    if (end_second > second) {
        --end_second;
    }

    return char_compare_backward(&end_first, &end_second, first, second);
}

int print_string(const char* string, FILE* stream) {
    if (string == NULL || stream == NULL) {
        return -1;
    }

    while (*string != '\n' && *string != '\0') {
        if (fputc((unsigned char) *string, stream) == EOF) {
            return -1;
        }
        ++string;
    }

    if (fputc('\n', stream) == EOF) {
        return -1;
    }

    return 0;
}
