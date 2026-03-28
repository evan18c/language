// Converts Tokens -> AST
// Author: Evan Cassidy
// Date: 3/27/2026
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Parser Class
typedef struct Parser_t {
    Token *tokens;
    int pos;
} Parser;

// Parser.peek
Token peek(Parser *parser) {
    return parser->tokens[parser->pos];
}

// Parser.peekn
Token peekn(Parser *parser, int n) {
    return parser->tokens[parser->pos + n];
}

// Parser.consume
Token consume(Parser *parser) {
    Token token = parser->tokens[parser->pos];
    parser->pos++;
    return token;
}

// Parse Expression (Primary)
Node *ParseExpressionPrimary(Parser *parser) {
    
    Token token = consume(parser);

    if (token.type == TOKEN_LITERAL) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_LITERAL;
        node->data.literal.val = token.value.int_value;
        return node;
    }

    if (token.type == TOKEN_IDENTIFIER) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->data.identifier.id = token.value.string_value;
    }

    printf("Unexpected token...");
}

// Parse Expression (Multiplication/Division)
Node *ParseExpressionMulDiv(Parser *parser) {
    Node *expr = ParseExpressionPrimary(parser);

    while(peek(parser).subtype == OPERATOR_MULTIPLY || peek(parser).subtype == OPERATOR_DIVIDE) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_BINARY;

        node->data.binary.l = expr;
        node->data.binary.op = consume(parser).subtype;
        node->data.binary.r = ParseExpressionPrimary(parser);

        expr = node;
    }

    return expr;
}

// Parse Expression (Addition/Subtraction)
Node *ParseExpressionAddSub(Parser *parser) {
    Node *expr = ParseExpressionMulDiv(parser);

    while(peek(parser).subtype == OPERATOR_ADD || peek(parser).subtype == OPERATOR_SUBTRACT) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_BINARY;

        node->data.binary.l = expr;
        node->data.binary.op = consume(parser).subtype;
        node->data.binary.r = ParseExpressionMulDiv(parser);

        expr = node;
    }

    return expr;
}

// Parse Expression
Node *ParseExpression(Parser *parser) {
    return ParseExpressionAddSub(parser);
}

// Parse Definition
Node *ParseDefinition(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_DEFINITION;

    node->data.definition.var = consume(parser).value.string_value; // identifier

    consume(parser); // :

    node->data.definition.type = consume(parser).subtype;

    consume(parser); // =

    node->data.definition.expr = ParseExpression(parser);

    consume(parser); // ;

    // Return Node
    return node;
}

Node **Parse(Token *tokens, int *total) {

    // Create Parser
    Parser parser;
    parser.tokens = tokens;
    parser.pos = 0;

    // Output
    Node **output = malloc(sizeof(Node *) * 1000);
    int output_pos = 0;

    // Main Parse Loop
    while (peek(&parser).type != TOKEN_EOF) {

        // Definitions
        if (peek(&parser).type == TOKEN_IDENTIFIER && peekn(&parser, 1).subtype == DELIMITER_COLON) {
            output[output_pos] = ParseDefinition(&parser);
            output_pos++;
        }

    }

    // Return
    *total = output_pos;
    return output;
}