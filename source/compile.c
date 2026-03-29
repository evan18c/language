// Wrapper for full compilation
// Author: Evan Cassidy
// Date: 3/29/2026

#include "compile.h"

// Compiles E into C
void compile(const char *code) {
    
    // Tokenize
    int total_tokens;
    Token *tokens = Tokenize((char *)code, &total_tokens);

    // Parse
    int total_nodes;
    Node **nodes = Parse(tokens, &total_nodes);

    // Transpile
    FILE *f = fopen("data/code.c", "w");
    fprintf(f, "#include <stdio.h>\n");
    for (int i=0; i<total_nodes; i++) {
        fprintf(f, "%s\n", NodeToC(nodes[i]));
    }
    fclose(f);

}