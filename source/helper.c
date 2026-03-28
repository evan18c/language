// Contains helper functions.
// Author: Evan Cassidy
// Date: 3/27/2026

#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool in_string_array(const char **arr, const int len, const char *str) {
    for (int i=0; i<len; i++) {
        if (strcmp(arr[i], str) == 0) {
            return true;
        }
    }
    return false;
}

bool is_integer(const char *str) {
    int i=0;
    while (str[i] != 0) {
        if (!isdigit(str[i]) && str[i] != '-') {
            return false;
        }
        i++;
    }
    return true;
}

bool is_float(const char *str) {
    int i=0;
    while (str[i] != 0) {
        if (!isdigit(str[i]) && str[i] != '-' && str[i] != '.') {
            return false;
        }
        i++;
    }
    return true;
}

bool is_bool(const char *str) {
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0) {
        return true;
    }
    return false;
}

bool atob(const char *str) {
    if (strcmp(str, "true") == 0) {
        return true;
    }
    return false;
}

char *read_file(const char *path) {
    FILE *f;
    f = fopen(path, "r");

    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);

    char *data = malloc(size + 1);
    memset(data, 0, size + 1);
    fread(data, 1, size, f);

    fclose(f);
    return data;
}