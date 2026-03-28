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
            TokenSubtype type;
            Node *expr;
        } definition;

        struct assignment_t {
            char *var;
            Node *expr;
        } assignment;

        struct function_t {
            char *name;
            char **args;
            TokenSubtype *arg_types;
            int args_total;
            TokenSubtype ret_type;
            Node **nodes;
            int nodes_total;
        } function;

        struct return_t {
            Node *ret;
        } return_t;

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

// Parser Class
typedef struct Parser_t {
    Token *tokens;
    int pos;
} Parser;

// Converts Tokens -> Array of Nodes
Node **Parse(Token *tokens, int *total);

// Prototypes
Token peek(Parser *parser);
Token peekn(Parser *parser, int n);
Token consume(Parser *parser);
Node *ParseStatement(Parser *parser);
Node *ParseDefinition(Parser *parser);
Node *ParseAssignment(Parser *parser);
Node *ParseFunction(Parser *parser);
Node *ParseReturn(Parser *parser);
Node *ParseExpressionPrimary(Parser *parser);
Node *ParseExpressionMulDiv(Parser *parser);
Node *ParseExpressionAddSub(Parser *parser);
Node *ParseExpression(Parser *parser);

#endif