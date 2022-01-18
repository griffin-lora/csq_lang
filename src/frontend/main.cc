#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "at.h"
#include "string_util.h"
#include "file_util.h"
#include "backend/compiler.h"

void catch_array_index_out_of_bounds_exception(size_t size, size_t index) {
    printf("Array index out of bounds: %zu >= %zu\n", index, size);
    exit(EXIT_FAILURE);
}

int custom_main(const size_t args_count, const size_t* arg_sizes, char** args) {
    // Create object files for each input file.
    for (size_t i = 1; i < args_count; ++i) {
        mutable_string name = { .size = arg_sizes[i] + 2, .data = args[i] };

        remove_parent_dirs(&name);
        strip_file_extension(&name);

        name.size += 2;
        name.data[name.size - 2] = '.';
        name.data[name.size - 1] = 'o';
        
        auto file = custom_fopen((string)name, construct_string("w"));

        compile_to_object_file(arg_sizes[i], args[i], file);
        fclose(file);
    }
    return 0;
}

int main(int arg_count, char** args) {
    size_t arg_sizes[arg_count];
    for (size_t i = 0; i < (size_t)arg_count; ++i) {
        arg_sizes[i] = strlen(args[i]);
    }
    return custom_main(arg_count, (const size_t*)arg_sizes, args);
}