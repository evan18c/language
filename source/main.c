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

    printf("\nC: \n%s\n\n", NodeToC(nodes[0]));

    return 0;
}