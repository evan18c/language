// Custom Programming Language
// Author: Evan Cassidy
// Date: 3/27/2026

// Standard
#include <stdio.h>
#include <stdlib.h>

// Custom
#include "helper.h"
#include "tokenize.h"

int main() {
    
    const char *code = "test:i64 = 5;";

    printf("Tokenizing...\n");
    int total;
    Token *tokens = Tokenize(code, &total);

    printf("Tokens:\n");
    PrintTokens(tokens, total);

    return 0;
}