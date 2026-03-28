// Converts Tokens -> AST
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef PARSE_H
#define PARSE_H
#include "tokenize.h"

// Node Types
typedef enum NodeType_t {
    NODE_DEFINITION,
    NODE_ASSIGNMENT,
    NODE_FUNCTION,
    NODE_CALL,
    NODE_RETURN,
    NODE_LITERAL,
    NODE_IDENTIFIER,
    NODE_BINARY
} NodeType;

// Node
typedef struct Node_t Node;
typedef struct Node_t {
    NodeType type;
    union data_t {

        struct definition_t {
            char *var;
            char *type;
            Node *expr;
        } definition;

        struct assignment_t {
            char *var;
            Node *expr;
        } assignment;

        struct binary_t {
            Node *l;
            TokenSubtype op;
            Node *r;
        } binary;

        struct literal_t {
            long long val;
        } literal;

        struct identifier_t {
            char *id;
        } identifier;

    } data;
    
} Node;

// Converts Tokens -> Array of Nodes
Node **Parse(Token *tokens);

#endif