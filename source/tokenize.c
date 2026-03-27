// Tokenizes Raw Text
// Author: Evan Cassidy
// Date: 3/27/2026

// Includes
#include "tokenize.h"
#include <stdlib.h>
#include <string.h>

// Converts Raw Text -> Tokens
Token *Tokenize(char *raw, int *total) {

    // Tokens
    Token *tokens = malloc(sizeof(Token) * 1000);
    int token_i = 0;

    // Dictionary
    const char *dict_kw[] = {"i64", "f64", "str", "bool", "map", "ret", "->"};

    // Text Iterator
    int i = 0;
    int length = strlen(raw);

    // Row + Column
    int row = 0;
    int column = 0;

    // Working Token
    char current_token[100] = {0};
    int cti = 0;

    while (i < length) {

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
                tokens[token_i] = (Token){TOKEN_OPERATOR, OPERATOR_MINUS, 0, row, column};
                token_i++;
                column++;
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

            default:
                current_token[cti] = raw[i];
                cti++;
                column++;
                break;
        }

        i++;
    }

    *total = token_i;
    return tokens;
}