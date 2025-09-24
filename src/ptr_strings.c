#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/ptr_strings.h"
#include "../header/strings_operations.h"

int read_text(text* text, char** buffer, size_t size, FILE* stream) {
    if (stream == NULL) {
        return fprintf(stderr, "ERRROR IN FUNC \"read_text\": NULL POINTER ON IMPUT STREAM\n");
    }
    if (text == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"read_text\": NULL POINTER ON TEXT STRUCTURE\n");
    }

    *buffer = (char*) calloc(size + 1, sizeof(char));

    if (*buffer == NULL) {
        return fprintf(stderr, "ERROR OF ALLOCATING BUFFER IN FUNC \"read_text\"\n");
    }

    if (fread(*buffer, sizeof(*buffer), size, stream) == 0) {
        return fprintf(stderr, "ERROR OF READING FILE IN FUNC \"read_text\"\n");
    }
    //(*buffer)[size] ='\0';

    char* tmp = strchr(*buffer, '\0');
    //printf("tmp: %d\n", *tmp);
    if (tmp != *buffer + size) {
        return fprintf(stderr, "ERROR OF READING FILE IN FUNC \"read_text\": \
        READ SIZE DOESN'T MATCH FILE SIZE:\nEXPECTED: %lu, GOT: %lu\n", size, (size_t)(tmp - *buffer));
    }

    //fseek(stream, 0, SEEK_SET);
    text->size = 0;
    int contains_alpha = has_alpha(*buffer);
    size_t max_string = 0;
    size_t curr_string = 0;
    for (size_t i = 0; i < size; i++) {
        if (contains_alpha < 0) {
            return fprintf(stderr, "ERROR IN FUNC \"read_text\": GOT ERROR WHILE CALLING \"has_alpha\"\n");
        }
        if (endof_string((*buffer)[i])) {
            if (contains_alpha) {
                (text->size)++;
                max_string = max_string < curr_string ? curr_string : max_string;
                curr_string = 0;
            }

            if ((*buffer)[i] != '\0') {
                contains_alpha = has_alpha(*buffer + i + 1);
            }
            else {
                break;
            }
        }
        else {
            curr_string++;
        }
    }
    text->str_array= (string*) calloc(text->size, sizeof(string));

    if (text->str_array == NULL) {
        return fprintf(stderr, "ERROR OF ALLOCATING POINTER ARRAY IN FUNC \"read_text\"\n");
    }

    size_t pos = 0;
    size_t last_ns = 0;
    for (size_t i = 0; (i < size) && (pos < text->size); i++) {
        if (endof_string((*buffer)[i])) {
            if (has_alpha(*buffer + last_ns) > 0) {
                text->str_array[pos].ptr = *buffer + last_ns;
                text->str_array[pos].size = i - last_ns;
                pos++;
            }
            if ((*buffer)[i] != '\0') {
                last_ns = i + 1;
            }
            else {
                break;
            }
        }
    }
    // printf("BUFFER:\n%s\n", *buffer);
    return 0;
}


int swap_strings(text* text, size_t s1_ptr, size_t s2_ptr) {
    if (text->str_array == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"swap_strings\": GOT NULL POINTER ON POINTER ARRAY\n");
    }
    if (s1_ptr > text->size ||
        s2_ptr > text->size) {
            return fprintf(stderr, "ERROR IN FUNC \"swap_strings\": SWAP ELEMENT INDEX OUT OF RANGE\n");
        }

    string tmp = {
        .ptr = NULL,
        .size = 0
    };
    tmp = text->str_array[s1_ptr];
    text->str_array[s1_ptr] = text->str_array[s2_ptr];
    text->str_array[s2_ptr] = tmp;
    //printf("Strings №%lu and №%lu were swapped\n", s1_ptr + 1, s2_ptr + 1);
    return 0;
}

int print_strings(text* text) {
    if (text->str_array == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"print_strings\": NULL POINTER ON POINTERS ARRAY\n");
    }

    for (size_t i = 0; i < text->size; i++) {
        if (text->str_array[i].size == 0) {
            continue;
        }

        char* str_ptr = text->str_array[i].ptr;
        //printf("String №%lu: ", i + 1);
        while (*str_ptr != '\n' && *str_ptr != '\0') putchar(*str_ptr), str_ptr++;
        putchar('\n');
        //printf(" Size: %lu\n", text.str_array[i].size);

    }
    return 0;
}

int write_strings(text* text, FILE* stream) {
    if (text->str_array == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"write_strings\": NULL POINTER ON POINTERS ARRAY\n");
    }
    if (stream == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"write_strings\": NULL POINTER ON OUTPUT FILE\n");
    }

    for (size_t i = 0; i < text->size; i++) {
        if (text->str_array[i].size == 0) {
            continue;
        }

        char* str_ptr = text->str_array[i].ptr;
        //printf("String №%lu: ", i + 1);
        while (*str_ptr != '\n' && *str_ptr != '\0') fputc(*str_ptr, stream), str_ptr++;
        fputc('\n', stream);
        //printf(" Size: %lu\n", text.str_array[i].size);
    }
    fprintf(stream, "===========================================================================\n");
    return 0;
}

int straight_comparator(const void* par1, const void* par2) {
    string s1 = *(string*) par1;
    string s2 = *(string*) par2;
    return straight_strcmp(s1.ptr, s2.ptr);
}

int reverse_comparator(const void* par1, const void* par2) {
    string s1 = *(string*) par1;
    string s2 = *(string*) par2;
    return reverse_strcmp(s1.ptr, s2.ptr);
}

int bubble_sort_strings(text* text, int is_backwards) {
    if (text->str_array == NULL) {
        return fprintf(stderr, "ERROR IN FUNC \"bubble_sort_strings\": NULL POINTER ON POINTERS ARRAY\n");
    }
    for (size_t r = 0; r < text->size; r++) {
        for (size_t l = 1; l < text->size - r; l++) {
            if (is_backwards ?
                reverse_strcmp(text->str_array[l].ptr, text->str_array[l - 1].ptr) < 0 :
                straight_strcmp(text->str_array[l].ptr, text->str_array[l - 1].ptr) < 0) {
                swap_strings(text, l, l - 1);
            }
        }
    }
    return 0;
}








