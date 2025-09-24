#include <stdio.h>
#include <assert.h>
#include "../header/sqvrt_strings.h"

int get_sizes(FILE* stream) {
    if (!stream) return 1;

    size_t amount = 0, max_string = 0;
    size_t str_size = 0;
    int symb = -2;

    while ((symb = fgetc(stream)) != EOF) {
        if (symb == '\n') {
            amount++;
            str_size = 0;
        }
        else str_size++;
        if (max_string < str_size) max_string = str_size;
    }
    max_string++; // end of the string
    fseek(stream, 0, SEEK_SET);
    if (amount != AMOUNT || max_string != MAX_STRING) {
        fprintf(stderr, "ACTUAL SIZES DOESN'T MATCH DEFINED SIZES:\n");
        fprintf(stderr, "Amount expected: %lu, defined: %d\n", amount, AMOUNT);
        fprintf(stderr, "Max string size expected: %lu, defined: %d\n", max_string, MAX_STRING);
        fprintf(stderr, "PLEASE REDEFINE YOUR PARAMETERS\n");
        return 1;
    }
    return 0;
}

int read_text(char strings_array[][MAX_STRING], FILE* stream) {
    assert(strings_array);
    assert(stream);

    for (size_t i = 0; i < AMOUNT; i++) {
        fscanf(stream, " %s \n", strings_array[i]);
    }
    return 0;
}


int swap_strings(char strings_array[][MAX_STRING], size_t s1_ptr, size_t s2_ptr) {
    assert(strings_array);

    char tmp = 0;
    for (size_t symb = 0; symb < MAX_STRING; symb++) {
        tmp = strings_array[s1_ptr - 1][symb];
        strings_array[s1_ptr - 1][symb] = strings_array[s2_ptr - 1][symb];
        strings_array[s2_ptr - 1][symb] = tmp;
    }
    printf("Strings №%lu and №%lu were swapped\n", s1_ptr, s2_ptr);
    return 0;
}

int print_strings(char strings_array[][MAX_STRING]) {
    assert(strings_array);

    for (size_t i = 0; i < AMOUNT; i++) {
        printf("String №%lu: %s\n", i + 1, strings_array[i]);
    }
    return 0;
}


