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
    const char dict_ch[] = " =+-*/:;\n\t";
    const char *dict_kw[] = {"i64", "str", "bool", "map", "ret", "->"};
    const int dict_kw_length = 6;

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
            }

            // Check if token is an integer
            else if (is_integer(current_token)) {
                tokens[token_i] = (Token){.type=TOKEN_LITERAL, .subtype=LITERAL_INTEGER, .value.int_value=atoi(current_token), .row=row, .column=column};
            }

            // Checks for Float/String/Bool here...

            // Token is an identifier
            else {
                char *str = malloc(strlen(current_token) + 1);
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
                tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_EQUAL, 0, row, column};
                token_i++;
                column++;
                break;

            case '+':
                tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_PLUS, 0, row, column};
                token_i++;
                column++;
                break;

            case '-':
                if (raw[i+1] != '>') {
                    tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MINUS, 0, row, column};
                    token_i++;
                    column++;
                } else {
                    tokens[token_i] = (Token){TOKEN_KEYWORD, KEYWORD_ARROW, 0, row, column};
                    token_i++;
                    column+=2;
                    i++;
                }
                break;

            case '*':
                tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MULTIPLY, 0, row, column};
                token_i++;
                column++;
                break;

            case '/':
                tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_DIVIDE, 0, row, column};
                token_i++;
                column++;
                break;

            // Delimiters
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

            // Special
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

const char *TokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_LITERAL: return "LITERAL";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

const char *TokenSubtypeToString(TokenSubtype type) {
    switch (type) {
        case NONE: return "NONE";
        case KEYWORD_MAP: return "MAP";
        case KEYWORD_RET: return "RET";
        case KEYWORD_ARROW: return "ARROW";
        case KEYWORD_I64: return "I64";
        case KEYWORD_STR: return "STR";
        case KEYWORD_BOOL: return "BOOL";
        case LITERAL_INTEGER: return "INTEGER";
        case LITERAL_FLOAT: return "FLOAT";
        case LITERAL_STRING: return "STRING";
        case LITERAL_BOOL: return "BOOL";
        case OPERATOR_EQUAL: return "EQUAL";
        case OPERATOR_PLUS: return "PLUS";
        case OPERATOR_MINUS: return "MINUS";
        case OPERATOR_MULTIPLY: return "MULTIPLY";
        case OPERATOR_DIVIDE: return "DIVIDE";
        case DELIMITER_COLON: return "COLON";
        case DELIMITER_SEMICOLON: return "SEMICOLON";
        default: return "UNKNOWN";
    }
}

void PrintTokens(Token *tokens, int total) {
    for (int i=0; i<total; i++) {
        Token token = tokens[i];
        
        printf("[%s:%s", TokenTypeToString(token.type), TokenSubtypeToString(token.subtype));

        if (token.type == TOKEN_LITERAL && token.subtype == LITERAL_INTEGER) {
            printf("(%d)", token.value.int_value);
        }
        else if (token.type == TOKEN_LITERAL && token.subtype == LITERAL_FLOAT) {
            printf("(%f)", token.value.float_value);
        }
        else if (token.type == TOKEN_LITERAL && token.subtype == LITERAL_STRING) {
            printf("(%s)", token.value.string_value);
        }
        else if (token.type == TOKEN_LITERAL && token.subtype == LITERAL_BOOL) {
            printf("(%hhi)", token.value.int_value);
        }
        
        if (token.type == TOKEN_IDENTIFIER) {
            printf("(%s)", token.value.string_value);
        }

        //printf(" @ %d:%d]\n", token.row, token.column);
        printf("]\n");
    }
}