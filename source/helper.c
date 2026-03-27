// Contains helper functions.
// Author: Evan Cassidy
// Date: 3/27/2026

#include "helper.h"
#include <string.h>

bool in_string_array(const char **arr, const int len, const char *str) {
    for (int i=0; i<len; i++) {
        if (strcmp(arr[i], str) == 0) {
            return true;
        }
    }
    return false;
}
