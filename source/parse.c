// Converts Tokens -> AST
// Author: Evan Cassidy
// Date: 3/27/2026
#include "parse.h"
#include <stdlib.h>

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

// Parse Definition
Node *ParseDefinition(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_DEFINITION;

    node->data.definition.var = consume(parser).value.string_value; // identifier

    consume(parser); // :

    node->data.definition.type = consume(parser).value.string_value; // type

    consume(parser); // =

    // Consume Expression

    consume(parser); // ;

    // Return Node
    return node;
}

Node **Parse(Token *tokens) {

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
}