#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../header/ptr_strings.h"
#include "../header/strings_operations.h"

typedef struct {
    const char* input_file_path;
    const char* output_file_path;
    char* buffer;
    struct stat inp_file_stat;
} information;


int main() {
    information info = {
        .input_file_path = "text.txt",
        .output_file_path = "output.txt",
        .buffer = NULL
    };
    if (stat(info.input_file_path, &(info.inp_file_stat)) == -1) return -fprintf(stderr, "ERROR OF GETTING FILE INFORMATION\n");

    FILE* f = fopen(info.input_file_path, "rb");

    if (f == NULL) return -fprintf(stderr, "FILE READING ERROR");

    text text = {
        .str_array = NULL,
        .size = 0
    };

    if (read_text(&text, &info.buffer, (size_t)info.inp_file_stat.st_size, f)) return fprintf(stderr, "ERROR WHILE READING FILE\n");
    fclose(f);

    FILE* output = fopen(info.output_file_path, "w");
    qsort(text.str_array, text.size, sizeof(string), straight_comparator);
    // bubble_sort_strings(&text, 0);
    write_strings(&text, output);

    qsort(text.str_array, text.size, sizeof(string), reverse_comparator);
    // bubble_sort_strings(&text, 1);
    write_strings(&text, output);

    fprintf(output, "%s\n", info.buffer);
    fclose(output);

    free(info.buffer);
    free(text.str_array);
    return printf("DONE\n") && 0;
}
