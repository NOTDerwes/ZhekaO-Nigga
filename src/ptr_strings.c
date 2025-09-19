#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "../header/ptr_strings.h"


char* read_text(const char* buffer, size_t size, FILE* stream) {
    assert(stream);
    const char* buffer = (const char*) calloc(size + 1, sizeof(char));

    if (buffer == NULL) return -fprint(stderr, "ERROR OF ALLOCATING BUFFER IN FUNC \"read_text\"\n");

    if (read(stream, buffer, sizeof(buffer)) == -1) return -fprintf(stderr, "ERROR OF READING FILE IN FUNC \"read_text\"\n");
    if (strchr(buffer, '\0') != buffer + size - 1) return -fprintf(stderr, "ERROR OF READING FILE IN FUCK \"read_text\": READ SIZE DOESN'T MATCH FILE SIZE\n");

    fseek(stream, 0, SEEK_SET);
    size_t amount = 1;
    for (size_t i = 0; i < size && buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') amount++;
    }

    size_t pos = 0;

    const char* text[] = (const char* *) calloc(amount, sizeof(char*));
    for (size_t i = 0; i < size && buffer[i] != '\0' && pos < amount; i++) {
        if (buffer[i] == '\n') {
            text[pos] = buffer + i + 1;
            pos++;
    }
}


int swap_strings(char* text[], size_t s1_ptr, size_t s2_ptr) {
    assert(text);

    char* tmp = NULL;
    tmp = text[s1_ptr - 1];
    text[s1_ptr - 1] = text[s2_ptr - 1];
    text[s2_ptr - 1] = tmp;
    printf("Strings №%lu and №%lu were swapped\n", s1_ptr, s2_ptr);
    return 0;
}

int print_strings(char* text[]) {
    assert(text);

    for (size_t i = 0; i < AMOUNT; i++) {
        printf("String №%lu: %s", i + 1, text[i]);
    }
    return 0;
}

int q_sort_strings(char* text[]) {
    assert(text);
    return 0;
}








