#ifndef PTR_STRINGS_H
#define PTR_STRINGS_H

typedef struct {
    char* ptr;
    size_t size;
} string;

typedef struct {
    size_t size;
    string* str_array;
} text;

int read_text(text* text, char** buffer, size_t size, FILE* stream);
int swap_strings(text* text, size_t s1_ptr, size_t s2_ptr);
int print_strings(text* text);
int write_strings(text* text, FILE* stream);
int straight_comparator(const void* par1, const void* par2);
int reverse_comparator(const void* par1, const void* par2);
int bubble_sort_strings(text* text, int is_backwards);

#endif
