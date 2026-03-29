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

int main() {
    
    const char *code = read_file("data/code.l");

    printf("Tokenizing...");
    int total_tokens;
    Token *tokens = Tokenize(code, &total_tokens);
    printf("Total: %d\n", total_tokens);

    printf("Parsing...");
    int total_nodes;
    Node **nodes = Parse(tokens, &total_nodes);
    printf("Total: %d\n", total_nodes);

    printf("Writing to C file...");
    FILE *f = fopen("data/code.c", "w");
    for (int i=0; i<total_nodes; i++) {
        fprintf(f, "%s\n", NodeToC(nodes[i]));
    }
    fclose(f);
    printf("Done!\n\n");

    return 0;
}