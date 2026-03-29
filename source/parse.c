// Converts Tokens -> AST
// Follows Recursive Descent
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
// eType: expected token type.
// eSubtype: expected token subtype.
Token consume(Parser *parser, TokenType eType, TokenSubtype eSubtype) {
    Token token = parser->tokens[parser->pos];
    parser->pos++;

    // Validate TokenType
    if (eType != ANY && token.type != eType) {
        printf("Expected: %s. Received: ", TokenTypeToString(eType));
        PrintToken(token);
        exit(EXIT_FAILURE);
    }

    // Validate TokenSubtype
    if (eSubtype != ANY && token.subtype != eSubtype) {
        printf("Expected: %s. Received: ", TokenSubtypeToString(eSubtype));
        PrintToken(token);
        exit(EXIT_FAILURE);
    }

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

    // Function Call (as a statement)
    if (peek(parser).type == TOKEN_IDENTIFIER && peekn(parser, 1).subtype == DELIMITER_LPAREN)
        return ParseFunctionStatement(parser);

    // Return
    if (peek(parser).type == TOKEN_KEYWORD && peek(parser).subtype == KEYWORD_RET)
        return ParseReturn(parser);

    // If
    if (peek(parser).type == TOKEN_KEYWORD && peek(parser).subtype == KEYWORD_IF)
        return ParseIf(parser);

    // While
    if (peek(parser).type == TOKEN_KEYWORD && peek(parser).subtype == KEYWORD_WHILE)
        return ParseWhile(parser);
}

// Parse Expression
Node *ParseExpression(Parser *parser) {
    return ParseExpressionAddSub(parser);
}





// Parse Expression (Primary)
Node *ParseExpressionPrimary(Parser *parser) {
    
    Token token = consume(parser, ANY, ANY);

    // Call
    if (token.type == TOKEN_IDENTIFIER && peek(parser).subtype == DELIMITER_LPAREN) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_CALL;
        node->data.call.args = malloc(sizeof(Node *) * 1000);
        node->data.call.func = token.value.string_value;
        node->data.call.args_total = 0;
        node->data.call.statement = false;
        consume(parser, TOKEN_DELIMITER, DELIMITER_LPAREN); // (
        while (peek(parser).subtype != DELIMITER_RPAREN) {
            if (node->data.call.args_total != 0) consume(parser, TOKEN_DELIMITER, DELIMITER_COMMA); // ,
            node->data.call.args[node->data.call.args_total] = ParseExpression(parser);
            node->data.call.args_total++;
        }
        consume(parser, TOKEN_DELIMITER, DELIMITER_RPAREN); // )
        return node;
    }

    // Literal
    if (token.type == TOKEN_LITERAL) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_LITERAL;
        node->data.literal.type = token.subtype;
        if (token.subtype == LITERAL_INTEGER) {
            node->data.literal.val.int_value = token.value.int_value;
        }
        else if (token.subtype == LITERAL_FLOAT) {
            node->data.literal.val.float_value = token.value.float_value;
        }
        else if (token.subtype == LITERAL_STRING) {
            node->data.literal.val.string_value = token.value.string_value;
        }
        else if (token.subtype == LITERAL_BOOL) {
            node->data.literal.val.bool_value = token.value.bool_value;
        }
        return node;
    }

    // Identifier
    if (token.type == TOKEN_IDENTIFIER) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_IDENTIFIER;
        node->data.identifier.id = token.value.string_value;
        return node;
    }

    // Parenthesis
    if (token.subtype == DELIMITER_LPAREN) {
        Node *node = ParseExpression(parser);
        consume(parser, TOKEN_DELIMITER, DELIMITER_RPAREN); // )
        return node;
    }

    printf("Unexpected token. Type:%d Subtype:%d\n", token.type, token.subtype);
}

// Parse Expression (Multiplication/Division) peMDas
Node *ParseExpressionMulDiv(Parser *parser) {
    Node *expr = ParseExpressionPrimary(parser);

    while(peek(parser).subtype == OPERATOR_MULTIPLY || peek(parser).subtype == OPERATOR_DIVIDE) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_BINARY;

        node->data.binary.l = expr;
        node->data.binary.op = consume(parser, TOKEN_OPERATOR, ANY).subtype;
        node->data.binary.r = ParseExpressionPrimary(parser);

        expr = node;
    }

    return expr;
}

// Parse Expression (Addition/Subtraction) (Includes Comparison Operators) pemdAS
Node *ParseExpressionAddSub(Parser *parser) {
    Node *expr = ParseExpressionMulDiv(parser);

    while(
        peek(parser).subtype == OPERATOR_ADD ||
        peek(parser).subtype == OPERATOR_SUBTRACT ||
        peek(parser).subtype == OPERATOR_LESS ||
        peek(parser).subtype == OPERATOR_GREATER ||
        peek(parser).subtype == OPERATOR_LESSEQUAL ||
        peek(parser).subtype == OPERATOR_GREATEREQUAL ||
        peek(parser).subtype == OPERATOR_NOTEQUAL ||
        peek(parser).subtype == OPERATOR_EQUALEQUAL
    ) {
        Node *node = malloc(sizeof(Node));
        node->type = NODE_BINARY;

        node->data.binary.l = expr;
        node->data.binary.op = consume(parser, TOKEN_OPERATOR, ANY).subtype;
        node->data.binary.r = ParseExpressionMulDiv(parser);

        expr = node;
    }

    return expr;
}

// Parse Definition
Node *ParseDefinition(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_DEFINITION;

    node->data.definition.var = consume(parser, TOKEN_IDENTIFIER, ANY).value.string_value; // identifier

    consume(parser, TOKEN_DELIMITER, DELIMITER_COLON); // :

    node->data.definition.type = consume(parser, TOKEN_KEYWORD, ANY).subtype; // type

    consume(parser, TOKEN_OPERATOR, OPERATOR_EQUAL); // =

    node->data.definition.expr = ParseExpression(parser); // expr

    consume(parser, TOKEN_DELIMITER, DELIMITER_SEMICOLON); // ;

    // Return Node
    return node;
}

// Parse Definition
Node *ParseAssignment(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_ASSIGNMENT;

    node->data.assignment.var = consume(parser, TOKEN_IDENTIFIER, ANY).value.string_value; // identifier

    consume(parser, TOKEN_OPERATOR, OPERATOR_EQUAL); // =

    node->data.assignment.expr = ParseExpression(parser); // expr

    consume(parser, TOKEN_DELIMITER, DELIMITER_SEMICOLON); // ;

    // Return Node
    return node;
}

// Parse Function
Node *ParseFunction(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_FUNCTION;

    consume(parser, TOKEN_KEYWORD, KEYWORD_MAP); // map

    node->data.function.name = consume(parser, TOKEN_IDENTIFIER, ANY).value.string_value; // name

    consume(parser, TOKEN_DELIMITER, DELIMITER_LPAREN); // (

    node->data.function.args = malloc(sizeof(char *) * 1000);
    node->data.function.arg_types = malloc(sizeof(TokenSubtype *) * 1000);
    node->data.function.args_total = 0;
    while (peek(parser).subtype != DELIMITER_RPAREN) {
        if (node->data.function.args_total != 0) consume(parser, TOKEN_DELIMITER, DELIMITER_COMMA); // ,
        node->data.function.args[node->data.function.args_total] = consume(parser, TOKEN_IDENTIFIER, ANY).value.string_value;
        consume(parser, TOKEN_DELIMITER, DELIMITER_COLON); // :
        node->data.function.arg_types[node->data.function.args_total] = consume(parser, TOKEN_KEYWORD, ANY).subtype; // arg type
        node->data.function.args_total++;
    }

    consume(parser, TOKEN_DELIMITER, DELIMITER_RPAREN); // )

    consume(parser, TOKEN_KEYWORD, KEYWORD_ARROW); // ->

    node->data.function.ret_type = consume(parser, TOKEN_KEYWORD, ANY).subtype; // ret type

    consume(parser, TOKEN_DELIMITER, DELIMITER_LBRACE); // {

    node->data.function.nodes = malloc(sizeof(Node *) * 1000);
    node->data.function.nodes_total = 0;
    while (peek(parser).subtype != DELIMITER_RBRACE) {
        node->data.function.nodes[node->data.function.nodes_total] = ParseStatement(parser);
        node->data.function.nodes_total++;
    }

    consume(parser, TOKEN_DELIMITER, DELIMITER_RBRACE); // }

    // Return Node
    return node;
}

// Parse Return
Node *ParseFunctionStatement(Parser *parser) {
    Node *node = ParseExpressionPrimary(parser);
    node->data.call.statement = true;
    consume(parser, TOKEN_DELIMITER, DELIMITER_SEMICOLON); // ;
    return node;
}

// Parse Return
Node *ParseReturn(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_RETURN;

    consume(parser, TOKEN_KEYWORD, KEYWORD_RET); // ret

    node->data.return_t.ret = ParseExpression(parser);

    consume(parser, TOKEN_DELIMITER, DELIMITER_SEMICOLON); // ;

    // Return Node
    return node;
}

// Parse If
Node *ParseIf(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_IF;

    consume(parser, TOKEN_KEYWORD, KEYWORD_IF); // if

    node->data.if_.cond = ParseExpression(parser);

    consume(parser, TOKEN_DELIMITER, DELIMITER_LBRACE); // {

    node->data.if_.nodes = malloc(sizeof(Node *) * 1000);
    node->data.if_.nodes_total = 0;
    while (peek(parser).subtype != DELIMITER_RBRACE) {
        node->data.if_.nodes[node->data.if_.nodes_total] = ParseStatement(parser);
        node->data.if_.nodes_total++;
    }

    consume(parser, TOKEN_DELIMITER, DELIMITER_RBRACE); // }

    // Return Node
    return node;
}

// Parse While
Node *ParseWhile(Parser *parser) {

    // Create Node
    Node *node = malloc(sizeof(Node));
    node->type = NODE_IF;

    consume(parser, TOKEN_KEYWORD, KEYWORD_WHILE); // if

    node->data.while_.cond = ParseExpression(parser);
    
    consume(parser, TOKEN_DELIMITER, DELIMITER_LBRACE); // {

    node->data.while_.nodes = malloc(sizeof(Node *) * 1000);
    node->data.while_.nodes_total = 0;
    while (peek(parser).subtype != DELIMITER_RBRACE) {
        node->data.while_.nodes[node->data.while_.nodes_total] = ParseStatement(parser);
        node->data.while_.nodes_total++;
    }

    consume(parser, TOKEN_DELIMITER, DELIMITER_RBRACE); // }

    // Return Node
    return node;
}