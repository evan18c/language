// Converts Nodes -> C
// Author: Evan Cassidy
// Date: 3/27/2026

// Imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Imports
#include "helper.h"
#include "tokenize.h"
#include "parse.h"

// Converts custom type to C type
char *TypeToC(TokenSubtype type) {
    switch (type) {
        case KEYWORD_I64: return "long long";
        default: return "<unknown>";
    }
}

// Converts a Node to C code
char *NodeToC(Node *node) {

    // Allocate string
    char *string = malloc(100);
    memset(string, 0, 100);

    // Conversion
    switch (node->type) {
        case NODE_DEFINITION:
            sprintf(string, "%s %s = %s;", TypeToC(node->data.definition.type), node->data.definition.var, NodeToC(node->data.definition.expr));
            break;
            break;
        case NODE_ASSIGNMENT:
            break;
        case NODE_FUNCTION:
            break;
        case NODE_CALL:
            break;
        case NODE_RETURN:
            break;
        case NODE_LITERAL:
            sprintf(string, "%d", node->data.literal.val);
            break;
        case NODE_IDENTIFIER:
            sprintf(string, "%s", node->data.identifier.id);
            break;
        case NODE_BINARY:
            break;
    }

    // Return
    return string;

}