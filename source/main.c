// Custom Programming Language
// Author: Evan Cassidy
// Date: 3/27/2026

#include <stdio.h>
#include "helper.h"
#include "tokenize.h"

int main() {
    
    printf("Tokenizing...\n");
    int total;
    Tokenize("====", &total);

    printf("Tokens created: %i\n", total);

    return 0;
}