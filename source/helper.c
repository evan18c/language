// Contains helper functions.
// Author: Evan Cassidy
// Date: 3/27/2026

#include "helper.h"
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