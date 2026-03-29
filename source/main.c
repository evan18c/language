// Custom Programming Language
// Author: Evan Cassidy
// Date: 3/27/2026

// Standard
#include <stdio.h>
#include <stdlib.h>

// Custom
#include "helper.h"
#include "tokenize.h"
#include "parse.h"
#include "transpile.h"
#include "compile.h"

// Main
int main() {
    const char *code = read("data/code.e");
    compile(code);
    return 0;
}