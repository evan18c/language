// Converts Tokens -> AST
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef PARSE_H
#define PARSE_H
#include "tokenize.h"

#define ANY 0

// Node Types
typedef enum NodeType_t {

    // Statements
    NODE_DEFINITION,
    NODE_ASSIGNMENT,
    NODE_FUNCTION,
    NODE_CALL,
    NODE_RETURN,
    NODE_IF,
    NODE_WHILE,
    NODE_STOP,
    NODE_BUS,

    // Expressions
    NODE_BINARY,

    // Objects
    NODE_LITERAL,
    NODE_IDENTIFIER
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

        struct call_t {
            char *func;
            Node **args;
            int args_total;
            bool statement;
        } call;

        struct return_t {
            Node *ret;
        } return_t;

        struct literal_t {
            TokenSubtype type;
            union val_t {
                long long int_value;
                double float_value;
                char *string_value;
                bool bool_value;
            } val;
        } literal;

        struct identifier_t {
            char *id;
        } identifier;

        struct binary_t {
            Node *l;
            TokenSubtype op;
            Node *r;
        } binary;

        struct if_t {
            Node *cond;
            Node **nodes;
            int nodes_total;
            Node **else_nodes;
            int else_nodes_total;
        } if_;

        struct while_t {
            Node *cond;
            Node **nodes;
            int nodes_total;
        } while_;

        struct stop_t {
            char *label;
        } stop;

        struct bus_t {
            char *dest;
        } bus;

    } data;
    
} Node;

// Parser Class
typedef struct Parser_t {
    Token *tokens;
    int pos;
} Parser;
Token peek(Parser *parser);
Token peekn(Parser *parser, int n);
Token consume(Parser *parser, TokenType eType, TokenSubtype eSubtype);

// Converts Tokens -> Array of Nodes
Node **Parse(Token *tokens, int *total);

// Statement
Node *ParseStatement(Parser *parser);

// Statement Types
Node *ParseDefinition(Parser *parser);
Node *ParseAssignment(Parser *parser);
Node *ParseFunction(Parser *parser);
Node *ParseFunctionStatement(Parser *parser);
Node *ParseReturn(Parser *parser);
Node *ParseIf(Parser *parser);
Node *ParseWhile(Parser *parser);
Node *ParseStop(Parser *parser);
Node *ParseBus(Parser *parser);

// Expression
Node *ParseExpression(Parser *parser);

// Expression Types
Node *ParseExpressionPrimary(Parser *parser);
Node *ParseExpressionMulDiv(Parser *parser);
Node *ParseExpressionAddSub(Parser *parser);

#endif