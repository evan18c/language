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
    const char dict_ch[] = " =+-*/<>!(){},:;'%\"\n\t";
    const char *dict_kw[] = {"i64", "i32", "i16", "i8", "f64", "f32", "f16", "f8", "str", "bool", "map", "ret", "->", "if", "while", "for"};
    const int dict_kw_length = sizeof(dict_kw) / sizeof(char *);

    // Text Iterator
    int i = 0;
    int length = strlen(raw);

    // Row + Column
    int row = 0;
    int column = 0;

    // Working Token
    char current_token[100] = {0};
    int cti = 0;

    // Loop through text
    while (i < length) {

        // Handle current token
        if (strchr(dict_ch, raw[i]) != NULL && cti != 0) {

            // Check if token is a keyword
            if (in_string_array(dict_kw, dict_kw_length, current_token)) {
                if (strcmp(current_token, "i64") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I64, 0, row, column};
                }
                if (strcmp(current_token, "i32") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I32, 0, row, column};
                }
                if (strcmp(current_token, "i16") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I16, 0, row, column};
                }
                if (strcmp(current_token, "i8") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_I8, 0, row, column};
                }
                if (strcmp(current_token, "f64") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F64, 0, row, column};
                }
                if (strcmp(current_token, "f32") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F32, 0, row, column};
                }
                if (strcmp(current_token, "f16") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F16, 0, row, column};
                }
                if (strcmp(current_token, "f8") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_F8, 0, row, column};
                }
                if (strcmp(current_token, "str") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_STR, 0, row, column};
                }
                if (strcmp(current_token, "bool") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_BOOL, 0, row, column};
                }
                if (strcmp(current_token, "map") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_MAP, 0, row, column};
                }
                if (strcmp(current_token, "ret") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_RET, 0, row, column};
                }
                if (strcmp(current_token, "if") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_IF, 0, row, column};
                }
                if (strcmp(current_token, "while") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_WHILE, 0, row, column};
                }
                if (strcmp(current_token, "for") == 0) {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_FOR, 0, row, column};
                }
            }

            // Check if token is an integer
            else if (is_integer(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_INTEGER, .value.int_value=atoi(current_token), .row=row, .column=column};
            }

            // Check if token is a float
            else if (is_float(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_INTEGER, .value.int_value=atof(current_token), .row=row, .column=column};
            }

            // Check if token is a string (INCOMPLETE)

            // Check if token is a bool
            else if (is_bool(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_INTEGER, .value.int_value=atob(current_token), .row=row, .column=column};
            }

            // Token is an identifier
            else {
                char *str = malloc(strlen(current_token) + 1);
                memset(str, 0, strlen(current_token) + 1);
                strcpy(str, current_token);
                tokens[token_i] = (Token){.type=TOKEN_IDENTIFIER, .subtype=NONE, .value.string_value=str, .row=row, .column=column};
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
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_EQUALEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_EQUAL, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '+':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_ADDEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_ADD, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '-':
                if (raw[i+1] == '>') {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_ARROW, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_SUBTRACTEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_SUBTRACT, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '*':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MULTIPLYEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MULTIPLY, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '/':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_DIVIDEEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_DIVIDE, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '%':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MODULOEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MODULO, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '<':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_LESSEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_LESS, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '>':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_GREATEREQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                } else {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_GREATER, 0, row, column};
                    token_i++;
                    column++;
                }
                break;

            case '!':
                if (raw[i+1] == '=') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_NOTEQUAL, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                }
                break;

            // Delimiters
            case '(':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_LPAREN, 0, row, column};
                token_i++;
                column++;
                break;

            case ')':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_RPAREN, 0, row, column};
                token_i++;
                column++;
                break;

            case '{':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_LBRACE, 0, row, column};
                token_i++;
                column++;
                break;
            
            case '}':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_RBRACE, 0, row, column};
                token_i++;
                column++;
                break;

            case ',':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_COMMA, 0, row, column};
                token_i++;
                column++;
                break;

            case ':':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_COLON, 0, row, column};
                token_i++;
                column++;
                break;

            case ';':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_SEMICOLON, 0, row, column};
                token_i++;
                column++;
                break;

            case '\'':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_SINGLEQUOTE, 0, row, column};
                token_i++;
                column++;
                break;

            case '"':
                tokens[token_i] = (Token){TOKEN_DELIMITER, DELIMITER_DOUBLEQUOTE, 0, row, column};
                token_i++;
                column++;
                break;

            // Special
            case '\t':
                column += 4;
                break;

            case '\n':
                row++;
                column = 0;
                break;

            case ' ':
                column++;
                break;

            // Default
            default:
                current_token[cti] = raw[i];
                cti++;
                column++;
                break;
        }

        i++;
    }

    // EOF
    tokens[token_i] = (Token){TOKEN_EOF, NONE, 0, row, column};
    token_i++;

    // Returns
    *total = token_i;
    return tokens;
}