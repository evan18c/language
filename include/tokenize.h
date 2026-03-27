// Tokenizes Raw Text
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef TOKENIZE_H
#define TOKENIZE_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_LITERAL,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_SPECIAL,
    TOKEN_EOF
} TokenType;

typedef enum {

    KEYWORD_MAP,
    KEYWORD_RET,
    KEYWORD_IF,
    KEYWORD_WHILE,
    KEYWORD_FOR,

    KEYWORD_I64,
    KEYWORD_I32,
    KEYWORD_I16,
    KEYWORD_I8,
    KEYWORD_F64,
    KEYWORD_F32,
    KEYWORD_F16,
    KEYWORD_F8,
    KEYWORD_STR,
    KEYWORD_BOOL,

    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_MODULO,
    OPERATOR_EQUAL,
    OPERATOR_PLUSEQUAL,
    OPERATOR_MINUSEQUAL,
    OPERATOR_MULTIPLYEQUAL,
    OPERATOR_DIVIDEEQUAL,
    OPERATOR_MODULOEQUAL,

    OPERATOR_LESS,
    OPERATOR_GREATER,
    OPERATOR_LESSEQUAL,
    OPERATOR_GREATEREQUAL,
    OPERATOR_NOTEQUAL,
    OPERATOR_EQUALEQUAL,

    DELIMITER_LPAREN,
    DELIMITER_RPAREN,
    DELIMITER_LBRACE,
    DELIMITER_RBRACE,
    DELIMITER_COMMA,
    DELIMITER_COLON,
    DELIMITER_SEMICOLON,
    DELIMITER_SINGLEQUOTE,
    DELIMITER_DOUBLEQUOTE

} TokenSubtype;

typedef struct {
    TokenType type;
    TokenSubtype subtype;
    union {
        long long int_value;
        double float_value;
        char *string_value;
    };
    int row, column;
} Token;

Token *Tokenize(char *raw, int *total);

#endif