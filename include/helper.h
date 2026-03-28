// Contains helper functions.
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef HELPER_H
#define HELPER_H

// Returns true if str in arr otherwise false.
bool in_string_array(const char **arr, const int len, const char *str);

// Returns true if str is an integer otherwise false.
bool is_integer(const char *str);

// Returns true if str is a float otherwise false.
bool is_float(const char *str);

// Reads file into buffer.
char *read_file(const char *path);

#endif