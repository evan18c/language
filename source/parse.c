// Converts Tokens -> AST
// Author: Evan Cassidy
// Date: 3/27/2026
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        output[output_pos] = ParseStatement(&parser);
        output_pos++;
    }

    // Return
    *total = output_pos;
    return output;
}

// Parse Statement
Node *ParseStatement(Parser *parser) {

    // Definition
    if (peek(parser).type == TOKEN_IDENTIFIER && peekn(parser, 1).subtype == DELIMITER_COLON)
        return ParseDefinition(parser);

    // Assignment
    if (peek(parser).type == TOKEN_IDENTIFIER && peekn(parser, 1).subtype == OPERATOR_EQUAL)
        return ParseAssignment(parser);

    // Function
    if (peek(parser).type == TOKEN_KEYWORD && peek(parser).subtype == KEYWORD_MAP)
        return ParseFunction(parser);

    // Return
    if (peek(parser).type == TOKEN_KEYWORD && peek(parser).subtype == KEYWORD_RET)
        return ParseReturn(parser);

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
        return node;
    }

    printf("Unexpected token: %d\n", token.type);
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

    node->data.definition.type = consume(parser).subtype; // type

    consume(parser); // =

    node->data.definition.expr = ParseExpression(parser); // expr

    consume(parser); // ;

    // Return Node
    return node;
}

// Parse Definition
Node *ParseAssignment(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_ASSIGNMENT;

    node->data.assignment.var = consume(parser).value.string_value; // identifier

    consume(parser); // =

    node->data.definition.expr = ParseExpression(parser); // expr

    consume(parser); // ;

    // Return Node
    return node;
}

// Parse Function
Node *ParseFunction(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_FUNCTION;

    consume(parser); // map

    node->data.function.name = consume(parser).value.string_value; // name

    consume(parser); // (

    node->data.function.args = malloc(sizeof(char *) * 1000);
    node->data.function.arg_types = malloc(sizeof(TokenSubtype *) * 1000);
    node->data.function.args_total = 0;
    while (peek(parser).subtype != DELIMITER_RPAREN) {
        if (node->data.function.args_total != 0) consume(parser); // ,
        node->data.function.args[node->data.function.args_total] = consume(parser).value.string_value;
        consume(parser); // :
        node->data.function.arg_types[node->data.function.args_total] = consume(parser).subtype; // arg type
        node->data.function.args_total++;
    }

    consume(parser); // )

    consume(parser); // ->

    node->data.function.ret_type = consume(parser).subtype; // ret type

    consume(parser); // {

    node->data.function.nodes = malloc(sizeof(Node *) * 1000);
    node->data.function.nodes_total = 0;
    while (peek(parser).subtype != DELIMITER_RBRACE) {
        node->data.function.nodes[node->data.function.nodes_total] = ParseStatement(parser);
        node->data.function.nodes_total++;
    }

    consume(parser); // }

    // Return Node
    return node;
}

Node *ParseReturn(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_RETURN;

    consume(parser); // ret

    node->data.return_t.ret = ParseExpression(parser);

    consume(parser); // ;

    // Return Node
    return node;
}