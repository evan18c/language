// Wrapper for full compilation
// Author: Evan Cassidy
// Date: 3/29/2026

#include "compile.h"

// Compiles E into C
void compile(const char *code) {
    
    // Tokenize
    printf("Tokenizing...");
    int total_tokens;
    Token *tokens = Tokenize((char *)code, &total_tokens);
    printf("Done!\n");
    
    // Parse
    printf("Parsing...");
    int total_nodes;
    Node **nodes = Parse(tokens, &total_nodes);
    printf("Done!\n");
    
    // Transpile
    printf("Compiling...");
    FILE *f = fopen("data/code.c", "w");
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include <stdlib.h>\n");
    fprintf(f, "#include <math.h>\n");
    fprintf(f, "#include <string.h>\n");
    for (int i=0; i<total_nodes; i++) {
        fprintf(f, "%s\n", NodeToC(nodes[i]));
    }
    fclose(f);
    printf("Done!\n\n");
    
}