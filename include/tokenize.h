// Tokenizes Raw Text
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef TOKENIZE_H
#define TOKENIZE_H

typedef enum TokenType_t {
    TOKEN_KEYWORD,
    TOKEN_LITERAL,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_EOF
} TokenType;

typedef enum TokenSubtype_t {

    NONE,

    KEYWORD_MAP,
    KEYWORD_RET,
    KEYWORD_ARROW,
    KEYWORD_IF,
    KEYWORD_WHILE,
    KEYWORD_ELSE,

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
    KEYWORD_VOID,

    LITERAL_INTEGER,
    LITERAL_FLOAT,
    LITERAL_STRING,
    LITERAL_BOOL,

    OPERATOR_EQUAL,
    OPERATOR_ADD,
    OPERATOR_SUBTRACT,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_MODULO,
    OPERATOR_ADDEQUAL,
    OPERATOR_SUBTRACTEQUAL,
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
    DELIMITER_SEMICOLON

} TokenSubtype;

typedef struct {
    TokenType type;
    TokenSubtype subtype;
    union value_t {
        long long int_value;
        double float_value;
        char *string_value;
        bool bool_value;
    } value;
    int line, column;
} Token;

// Converts raw -> Tokens.
Token *Tokenize(char *raw, int *total);

// Converts TokenType -> string.
char *TokenTypeToString(TokenType type);

// Converts TokenSubtype -> string.
char *TokenSubtypeToString(TokenSubtype type);

// Prints a token.
void PrintToken(Token token);

#endif