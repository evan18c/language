// Custom Programming Language
// Author: Evan Cassidy
// Date: 3/27/2026

#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "tokenize.h"

int main() {
    
    const char *code = "ret;";

    printf("Tokenizing...\n");
    int total;
    Token *tokens = Tokenize(code, &total);

    printf("Tokens:\n");
    for (int i=0; i<total; i++) {
        Token token = tokens[i];
        printf("[%s:%s @ %d:%d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.row, token.column);
    }

    return 0;
}