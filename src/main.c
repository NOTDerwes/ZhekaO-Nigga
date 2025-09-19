#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

typedef struct {
    const char* input_file_path;
    const char* output_file_path;
    const char* buffer;
    struct stat inp_file_stat;
} information;


int main() {
    information info = {
        .input_file_path = "text.txt",
        .output_file_path = "output.txt",
        .buffer = NULL,
    };
    if (stat(info.input_file_path, &(info.inp_file_stat)) == -1) return -fprintf(stderr, "ERROR OF GETTING FILE INFORMATION\n");

    FILE* f = fopen(info.input_file_path, "rb");

    if (f == NULL) return -fprintf(stderr, "FILE READING ERROR");

    read_text(text, sizes_array, f);
    fclose(f);
    swap_strings(text, 1, 3);
    print_strings(text);
    free_array(text);
}
