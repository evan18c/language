// Converts Nodes -> C
// Author: Evan Cassidy
// Date: 3/27/2026

// Imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Imports
#include "helper.h"
#include "tokenize.h"
#include "parse.h"

// Converts an operator to C code
char *OperatorToC(TokenSubtype type) {
    switch (type) {
        case OPERATOR_EQUAL: return "=";
        case OPERATOR_ADD: return "+";
        case OPERATOR_SUBTRACT: return "-";
        case OPERATOR_MULTIPLY: return "*";
        case OPERATOR_DIVIDE: return "/";
        case OPERATOR_MODULO: return "%";
        case OPERATOR_ADDEQUAL: return "+=";
        case OPERATOR_SUBTRACTEQUAL: return "-=";
        case OPERATOR_MULTIPLYEQUAL: return "*-";
        case OPERATOR_DIVIDEEQUAL: return "/=";
        case OPERATOR_MODULOEQUAL: return "%=";
    }
}

// Converts custom type to C type
char *TypeToC(TokenSubtype type) {
    switch (type) {
        case KEYWORD_I64: return "long long";
        case KEYWORD_I32: return "int";
        case KEYWORD_I16: return "short";
        case KEYWORD_I8: return "short";
        case KEYWORD_F64: return "long long";
        case KEYWORD_F32: return "int";
        case KEYWORD_F16: return "short";
        case KEYWORD_F8: return "short";
        case KEYWORD_STR: return "char *";
        case KEYWORD_BOOL: return "bool";
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
            sprintf(string, "%s %s=%s;", TypeToC(node->data.definition.type), node->data.definition.var, NodeToC(node->data.definition.expr));
            break;

        case NODE_ASSIGNMENT:
            break;

        case NODE_FUNCTION:
            char buffer[1000] = {0};
            sprintf(string, "%s %s(", TypeToC(node->data.function.ret_type), node->data.function.name);
            for (int i=0; i<node->data.function.args_total; i++) {
                if (i>0) sprintf(string, ",");
                sprintf(buffer, "%s %s", TypeToC(node->data.function.arg_types[i]), node->data.function.args[i]);
                strcat(string, buffer);
            }
            strcat(string, "){");
            for (int i=0; i<node->data.function.nodes_total; i++) {
                sprintf(buffer, "%s", NodeToC(node->data.function.nodes[i]));
                strcat(string, buffer);
            }
            strcat(string, "};");
            break;

        case NODE_CALL:
            break;

        case NODE_RETURN:
            sprintf(string, "return %s;", NodeToC(node->data.return_t.ret));
            break;

        case NODE_LITERAL:
            sprintf(string, "%d", node->data.literal.val);
            break;

        case NODE_IDENTIFIER:
            sprintf(string, "%s", node->data.identifier.id);
            break;

        case NODE_BINARY:
            sprintf(string, "(%s%s%s)", NodeToC(node->data.binary.l), OperatorToC(node->data.binary.op), NodeToC(node->data.binary.r));
            break;
    }

    // Return
    return string;

}