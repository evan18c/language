// Wrapper for full compilation
// Author: Evan Cassidy
// Date: 3/29/2026
#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include "tokenize.h"
#include "parse.h"
#include "transpile.h"

// Compiles E into C
void compile(const char *code);

#endif