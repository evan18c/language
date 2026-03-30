// Tokenizes Raw Text
// Author: Evan Cassidy
// Date: 3/27/2026

// Includes
#include "tokenize.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Token *Tokenize(char *raw, int *total) {

    // Tokens
    Token *tokens = malloc(sizeof(Token) * 1000);
    int token_i = 0;

    // Dictionary
    const char dict_ch[] = " =+-*/<>!(){},:;%\n\t";
    const char *dict_kw[] = {
        "i64", "i32", "i16", "i8", "f64", "f32", "f16", "f8", "str", "bool", "map", "ret", "->", "if", "while", "void", "else", "bus", "stop", "item"
    };
    const int dict_kw_length = sizeof(dict_kw) / sizeof(char *);

    // Text Iterator
    int i = 0;
    int length = strlen(raw);

    // Line + Column
    int line = 1;
    int column = 0;

    // Working Token
    char current_token[100] = {0};
    int cti = 0;
    bool bs = false; // building string flag

    // Loop through text
    while (i < length) {

        // Handle current token
        if (strchr(dict_ch, raw[i]) != NULL && cti != 0 && !bs) {

            // Check if token is a keyword
            if (in_string_array(dict_kw, dict_kw_length, current_token)) {
                if (strcmp(current_token, "i64") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I64, 0, line, column};
                }
                if (strcmp(current_token, "i32") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I32, 0, line, column};
                }
                if (strcmp(current_token, "i16") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I16, 0, line, column};
                }
                if (strcmp(current_token, "i8") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I8, 0, line, column};
                }
                if (strcmp(current_token, "f64") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F64, 0, line, column};
                }
                if (strcmp(current_token, "f32") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F32, 0, line, column};
                }
                if (strcmp(current_token, "f16") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F16, 0, line, column};
                }
                if (strcmp(current_token, "f8") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F8, 0, line, column};
                }
                if (strcmp(current_token, "str") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_STR, 0, line, column};
                }
                if (strcmp(current_token, "bool") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_BOOL, 0, line, column};
                }
                if (strcmp(current_token, "map") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_MAP, 0, line, column};
                }
                if (strcmp(current_token, "ret") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_RET, 0, line, column};
                }
                if (strcmp(current_token, "if") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_IF, 0, line, column};
                }
                if (strcmp(current_token, "while") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_WHILE, 0, line, column};
                }
                if (strcmp(current_token, "void") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_VOID, 0, line, column};
                }
                if (strcmp(current_token, "else") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_ELSE, 0, line, column};
                }
                if (strcmp(current_token, "bus") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_BUS, 0, line, column};
                }
                if (strcmp(current_token, "stop") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_STOP, 0, line, column};
                }
                if (strcmp(current_token, "item") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_ITEM, 0, line, column};
                }
            }

            // Check if token is an integer
            else if (is_integer(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_INTEGER, .value.int_value=atoi(current_token), .line=line, .column=column};
            }

            // Check if token is a float
            else if (is_float(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_FLOAT, .value.float_value=atof(current_token), .line=line, .column=column};
            }

            // Check if token is a string
            else if (is_string(current_token)) {
                char *str = malloc(strlen(current_token) + 1);
                memset(str, 0, strlen(current_token) + 1);
                strcpy(str, current_token);
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_STRING, .value.string_value=str, .line=line, .column=column};
            }

            // Check if token is a bool
            else if (is_bool(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_BOOL, .value.bool_value=atob(current_token), .line=line, .column=column};
            }

            // Token is an identifier
            else {
                char *str = malloc(strlen(current_token) + 1);
                memset(str, 0, strlen(current_token) + 1);
                strcpy(str, current_token);
                tokens[token_i] = (Token){.type=TOKEN_IDENTIFIER, .subtype=NONE, .value.string_value=str, .line=line, .column=column};
            }

            // Increment
            token_i++;
            memset(current_token, 0, 100);
            cti = 0;
        }

        // Handle individual character
        switch (raw[i]) {

            // Operators
            case '=':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_EQUALEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_EQUAL, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '+':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_ADDEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_ADD, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '-':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '>') {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_ARROW, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_SUBTRACTEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_SUBTRACT, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '*':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MULTIPLYEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MULTIPLY, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '/':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_DIVIDEEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_DIVIDE, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '%':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MODULOEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MODULO, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '<':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_LESSEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_LESS, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '>':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_GREATEREQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_GREATER, 0, line, column};
                    token_i++;
                    column++;
                }
                break;

            case '!':
                if (bs) goto DEFAULT;
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_NOTEQUAL, 0, line, column};
                    token_i++;
                    column+=2;
                    i++;
                }
                break;

            // Delimiters
            case '(':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_LPAREN, 0, line, column};
                token_i++;
                column++;
                break;

            case ')':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_RPAREN, 0, line, column};
                token_i++;
                column++;
                break;

            case '{':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_LBRACE, 0, line, column};
                token_i++;
                column++;
                break;
            
            case '}':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_RBRACE, 0, line, column};
                token_i++;
                column++;
                break;

            case ',':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_COMMA, 0, line, column};
                token_i++;
                column++;
                break;

            case ':':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_COLON, 0, line, column};
                token_i++;
                column++;
                break;

            case ';':
                if (bs) goto DEFAULT;
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_SEMICOLON, 0, line, column};
                token_i++;
                column++;
                break;

            // Special
            case '\t':
                if (bs) goto DEFAULT;
                column += 4;
                break;

            case '\n':
                if (bs) goto DEFAULT;
                line++;
                column = 0;
                break;

            case ' ':
                if (bs) goto DEFAULT;
                column++;
                break;

            case '"':
                bs = !bs;
                goto DEFAULT;
                break;

            // Default
            default:
                DEFAULT:
                current_token[cti] = raw[i];
                cti++;
                column++;
                break;
        }

        i++;
    }

    // EOF
    tokens[token_i] = (Token){TOKEN_EOF, NONE, 0, line, column};
    token_i++;

    // Returns
    *total = token_i;
    return tokens;
}

char *TokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_LITERAL: return "TOKEN_LITERAL";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
        case TOKEN_DELIMITER: return "TOKEN_DELIMITER";
        case TOKEN_EOF: return "TOKEN_EOF";
        default: return "";
    }
}

char *TokenSubtypeToString(TokenSubtype type) {
    switch (type) {
        case NONE: return "NONE";
        case KEYWORD_MAP: return "KEYWORD_MAP";
        case KEYWORD_RET: return "KEYWORD_RET";
        case KEYWORD_ARROW: return "KEYWORD_ARROW";
        case KEYWORD_IF: return "KEYWORD_IF";
        case KEYWORD_WHILE: return "KEYWORD_WHILE";
        case KEYWORD_ELSE: return "KEYWORD_ELSE";
        case KEYWORD_BUS: return "KEYWORD_BUS";
        case KEYWORD_STOP: return "KEYWORD_STOP";
        case KEYWORD_ITEM: return "KEYWORD_ITEM";
        case KEYWORD_I64: return "KEYWORD_I64";
        case KEYWORD_I32: return "KEYWORD_I32";
        case KEYWORD_I16: return "KEYWORD_I16";
        case KEYWORD_I8: return "KEYWORD_I8";
        case KEYWORD_F64: return "KEYWORD_F64";
        case KEYWORD_F32: return "KEYWORD_F32";
        case KEYWORD_F16: return "KEYWORD_F16";
        case KEYWORD_F8: return "KEYWORD_F8";
        case KEYWORD_STR: return "KEYWORD_STR";
        case KEYWORD_BOOL: return "KEYWORD_BOOL";
        case KEYWORD_VOID: return "KEYWORD_VOID";
        case LITERAL_INTEGER: return "LITERAL_INTEGER";
        case LITERAL_FLOAT: return "LITERAL_FLOAT";
        case LITERAL_STRING: return "LITERAL_STRING";
        case LITERAL_BOOL: return "LITERAL_BOOL";
        case OPERATOR_EQUAL: return "OPERATOR_EQUAL";
        case OPERATOR_ADD: return "OPERATOR_ADD";
        case OPERATOR_SUBTRACT: return "OPERATOR_SUBTRACT";
        case OPERATOR_MULTIPLY: return "OPERATOR_MULTIPLY";
        case OPERATOR_DIVIDE: return "OPERATOR_DIVIDE";
        case OPERATOR_MODULO: return "OPERATOR_MODULO";
        case OPERATOR_ADDEQUAL: return "OPERATOR_ADDEQUAL";
        case OPERATOR_SUBTRACTEQUAL: return "OPERATOR_SUBTRACTEQUAL";
        case OPERATOR_MULTIPLYEQUAL: return "OPERATOR_MULTIPLYEQUAL";
        case OPERATOR_DIVIDEEQUAL: return "OPERATOR_DIVIDEEQUAL";
        case OPERATOR_MODULOEQUAL: return "OPERATOR_MODULOEQUAL";
        case OPERATOR_LESS: return "OPERATOR_LESS";
        case OPERATOR_GREATER: return "OPERATOR_GREATER";
        case OPERATOR_LESSEQUAL: return "OPERATOR_LESSEQUAL";
        case OPERATOR_GREATEREQUAL: return "OPERATOR_GREATEREQUAL";
        case OPERATOR_NOTEQUAL: return "OPERATOR_NOTEQUAL";
        case OPERATOR_EQUALEQUAL: return "OPERATOR_EQUALEQUAL";
        case DELIMITER_LPAREN: return "DELIMITER_LPAREN";
        case DELIMITER_RPAREN: return "DELIMITER_RPAREN";
        case DELIMITER_LBRACE: return "DELIMITER_LBRACE";
        case DELIMITER_RBRACE: return "DELIMITER_RBRACE";
        case DELIMITER_COMMA: return "DELIMITER_COMMA";
        case DELIMITER_COLON: return "DELIMITER_COLON";
        case DELIMITER_SEMICOLON: return "DELIMITER_SEMICOLON";
        default: return "";
    }
}

void PrintToken(Token token) {
    switch (token.type) {
        case TOKEN_KEYWORD:
            printf("[%s.%s @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.line);
            break;
        case TOKEN_LITERAL:
            if (token.subtype == LITERAL_INTEGER)
                printf("[%s.%s(%d) @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.value.int_value, token.line);
            if (token.subtype == LITERAL_FLOAT)
                printf("[%s.%s(%f) @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.value.float_value, token.line);
            if (token.subtype == LITERAL_STRING)
                printf("[%s.%s(%s) @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.value.string_value, token.line);
            if (token.subtype == LITERAL_BOOL)
                printf("[%s.%s(%lli) @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.value.bool_value, token.line);
            break;
        case TOKEN_IDENTIFIER:
            printf("[%s(%s) @ Line %d]\n", TokenTypeToString(token.type), token.value.string_value, token.line);
            break;
        case TOKEN_OPERATOR:
            printf("[%s.%s @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.line);
            break;
        case TOKEN_DELIMITER:
            printf("[%s.%s @ Line %d]\n", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype), token.line);
            break;
        case TOKEN_EOF:
            printf("[%s @ Line %d]\n", TokenTypeToString(token.type), token.line);
            break;
    }

}
