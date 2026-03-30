// Converts Nodes -> C
// Author: Evan Cassidy
// Date: 3/27/2026
#ifndef TRANSPILE_H
#define TRANSPILE_H

// Imports
#include "helper.h"
#include "tokenize.h"
#include "parse.h"

// Converts a Node to C code
char *NodeToC(Node *node);

// Converts an operator to C code
char *OperatorToC(TokenSubtype type);

// Converts custom type to C type
char *TypeToC(TokenSubtype type);

#endif