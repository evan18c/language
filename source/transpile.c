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
        case OPERATOR_LESS: return "<";
        case OPERATOR_GREATER: return ">";
        case OPERATOR_LESSEQUAL: return "<=";
        case OPERATOR_GREATEREQUAL: return ">=";
        case OPERATOR_NOTEQUAL: return "!=";
        case OPERATOR_EQUALEQUAL: return "==";
    }
}

// Converts custom type to C type
char *TypeToC(TokenSubtype type) {
    switch (type) {
        case KEYWORD_I64: return "long long";
        case KEYWORD_I32: return "int";
        case KEYWORD_I16: return "short";
        case KEYWORD_I8: return "short";
        case KEYWORD_F64: return "double";
        case KEYWORD_F32: return "float";
        case KEYWORD_F16: return "float";
        case KEYWORD_F8: return "float";
        case KEYWORD_STR: return "char*";
        case KEYWORD_BOOL: return "bool";
        case KEYWORD_VOID: return "void";
    }
}

// Converts a Node to C code
char *NodeToC(Node *node) {

    // Allocate string
    char *string = malloc(100);
    memset(string, 0, 100);
    char buffer[1000] = {0}; // temp

    // Conversion
    switch (node->type) {

        case NODE_DEFINITION:
            sprintf(string, "%s %s=%s;", TypeToC(node->data.definition.type), node->data.definition.var, NodeToC(node->data.definition.expr));
            break;

        case NODE_ASSIGNMENT:
            sprintf(string, "%s=%s;", node->data.assignment.var, NodeToC(node->data.assignment.expr));
            break;

        case NODE_FUNCTION:
            sprintf(string, "%s %s(", TypeToC(node->data.function.ret_type), node->data.function.name);
            for (int i=0; i<node->data.function.args_total; i++) {
                sprintf(buffer, "%s%s %s", (i > 0 ? "," : ""), TypeToC(node->data.function.arg_types[i]), node->data.function.args[i]);
                strcat(string, buffer);
            }
            strcat(string, "){");
            for (int i=0; i<node->data.function.nodes_total; i++) {
                sprintf(buffer, "%s", NodeToC(node->data.function.nodes[i]));
                strcat(string, buffer);
            }
            strcat(string, "}");
            break;

        case NODE_CALL:
            char buffer[1000] = {0};
            sprintf(string, "%s(", node->data.call.func);
            for (int i=0; i<node->data.call.args_total; i++) {
                sprintf(buffer, "%s%s", (i > 0 ? "," : ""), NodeToC(node->data.call.args[i]));
                strcat(string, buffer);
            }
            if (node->data.call.statement) strcat(string, ");");
            else strcat(string, ")");
            break;

        case NODE_RETURN:
            sprintf(string, "return %s;", NodeToC(node->data.return_t.ret));
            break;

        case NODE_IF:
            sprintf(string, "if(%s){", NodeToC(node->data.if_.cond));
            for (int i=0; i<node->data.if_.nodes_total; i++) {
                strcat(string, NodeToC(node->data.if_.nodes[i]));
            }
            strcat(string, "}");
            for (int i=0; i<node->data.if_.else_nodes_total; i++) {
                if (i==0) strcat(string, "else{");
                strcat(string, NodeToC(node->data.if_.else_nodes[i]));
                if (i==node->data.if_.else_nodes_total-1) strcat(string, "}");
            }
            break;

        case NODE_WHILE:
            sprintf(string, "while(%s){", NodeToC(node->data.if_.cond));
            for (int i=0; i<node->data.if_.nodes_total; i++) {
                strcat(string, NodeToC(node->data.if_.nodes[i]));
            }
            strcat(string, "}");
            break;

        case NODE_LITERAL:
            if (node->data.literal.type == LITERAL_INTEGER) sprintf(string, "%d", node->data.literal.val.int_value);
            if (node->data.literal.type == LITERAL_FLOAT) sprintf(string, "%f", node->data.literal.val.float_value);
            if (node->data.literal.type == LITERAL_STRING) sprintf(string, "%s", node->data.literal.val.string_value);
            if (node->data.literal.type == LITERAL_BOOL) sprintf(string, "%f", node->data.literal.val.bool_value);
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